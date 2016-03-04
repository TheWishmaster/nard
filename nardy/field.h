#include <vector>
#include <utility>
#include <stack>
#include <algorithm>
#include <cmath>

typedef std::vector<std::pair<int, int> > TMove;

const int kFieldSize = 24, kChipsOverall = 15;

struct Cell{
	bool is_occupied, color;
	int number;
	Cell() :
		color(false),
		is_occupied(false),
		number(0){};
	Cell(bool owner, int dices_standing) :
		is_occupied(true),
		color(owner),
		number(dices_standing){};
};

struct OneChangeOfField{
	int color, out_of_border, last_cell, starting_cell_number, ending_cell_number;
	Cell starting_cell, ending_cell;
	OneChangeOfField(int color, int out_of_border, int last_cell,
		int starting_cell_number, int ending_cell_number, Cell &starting_cell, Cell &ending_cell) :
		out_of_border(out_of_border),
		color(color),
		ending_cell(ending_cell),
		starting_cell(starting_cell),
		ending_cell_number(ending_cell_number),
		starting_cell_number(starting_cell_number),
		last_cell(last_cell){};
};

class Field{

private:
	std::vector <Cell> cells_;
	int out_of_border_[2], last_cell_[2];
	std::stack <OneChangeOfField> previous_moves_;
	int FirstAtHome(bool color){
		int good = out_of_border_[color], first = kFieldSize;
		for (int i = 0; i < 6; ++i)
			if (cells_[kFieldSize - i - 1].is_occupied && cells_[kFieldSize - i - 1].color == color){
				good += cells_[kFieldSize - i - 1].number;
				first = kFieldSize - i - 1;
			}
		if (good < kChipsOverall)
			return kFieldSize - 7;
		return first;
	}
	void DecreaseFirstCell(int starting_cell, int color){
		cells_[starting_cell].number--;
		if (cells_[starting_cell].number == 0){
			cells_[starting_cell].is_occupied = 0;
		}
	}
public:
	int Convert(int position, bool difference = true){
		if (position >= kFieldSize)
			return position;
		if (difference == false)
			return position;
		if (position < kFieldSize / 2)
			return kFieldSize / 2 + position;
		return position - kFieldSize / 2;
	}
	Field(int my_color) : cells_(kFieldSize){
		out_of_border_[0] = out_of_border_[1] = last_cell_[0] = last_cell_[1] = 0;
		cells_[0] = (Cell(my_color, kChipsOverall));
		cells_[kFieldSize / 2] = Cell(!my_color, kChipsOverall);
	};
	Field(){
		out_of_border_[0] = out_of_border_[1] = 0;
		last_cell_[0] = last_cell_[1] = 0;
	}
	void UndoLastMove(){
		OneChangeOfField temp = previous_moves_.top();
		previous_moves_.pop();
		out_of_border_[temp.color] = temp.out_of_border;
		last_cell_[temp.color] = temp.last_cell;
		cells_[temp.starting_cell_number] = temp.starting_cell;
		if (temp.ending_cell_number < kFieldSize)
			cells_[temp.ending_cell_number] = temp.ending_cell;
	}
	Field(std::vector <int> state, bool my_color){
		out_of_border_[0] = out_of_border_[1] = kChipsOverall;
		for (int i = 0; i < state.size(); ++i){
			if (state[i] == 0)
				cells_[i].is_occupied = false;
			else{
				cells_[i].is_occupied = true;
				cells_[i].number = abs(state[i]);
				if (state[i] < 0)
					cells_[i].color = 1;
				else
					cells_[i].color = 0;
			}
		}
		last_cell_[0] = last_cell_[1] = 0;
		for (int i = 0; i < state.size(); ++i) if (cells_[i].is_occupied){
			out_of_border_[cells_[i].color] -= cells_[i].number;
			last_cell_[cells_[i].color] = max(last_cell_[cells_[i].color], Convert(i, my_color != cells_[i].color));
		}
		if (out_of_border_[0] > 0){
			last_cell_[0] = kFieldSize;
		}
		if (out_of_border_[1] > 0){
			last_cell_[1] = kFieldSize;
		}
	}
	bool MoveOneChip(int starting_cell, int ending_cell, int color){
		// returns 0 if we can't do this move ans changes nothing othervize does that move

		if (cells_[starting_cell].is_occupied == false || cells_[starting_cell].color != color){
			return 0;
		}

		if (ending_cell < kFieldSize && cells_[ending_cell].is_occupied && cells_[ending_cell].color != color){
			return 0;
		}

		int first;
		if (ending_cell >= kFieldSize)
			first = FirstAtHome(color);
		if ((ending_cell > kFieldSize && first != starting_cell) || (ending_cell == kFieldSize && first < kFieldSize - 6)){
			return 0;
		}

		previous_moves_.push(OneChangeOfField(color, out_of_border_[color], last_cell_[color],
			starting_cell, ending_cell, cells_[starting_cell], cells_[ending_cell % kFieldSize]));

		if (ending_cell >= kFieldSize){
			out_of_border_[color]++;
			last_cell_[color] = kFieldSize;
			DecreaseFirstCell(starting_cell, color);
			return 1;
		}
		if (cells_[ending_cell].is_occupied == false){
			cells_[ending_cell].is_occupied = true;
			cells_[ending_cell].color = color;
			if (ending_cell > last_cell_[color])
				last_cell_[color] = ending_cell;
		}
		cells_[ending_cell].number++;
		DecreaseFirstCell(starting_cell, color);

		if (Convert(ending_cell) > last_cell_[!color]){
			int forward = 1, backward = 1, other_view = Convert(ending_cell);
			while (forward < 6 && Convert(other_view + forward) < kFieldSize && cells_[Convert(other_view + forward)].is_occupied
				&& cells_[Convert(other_view + forward)].color == color)
				++forward;
			while (forward < 6 && Convert(other_view - backward) < kFieldSize && cells_[Convert(other_view - backward)].is_occupied
				&& cells_[Convert(other_view - backward)].color == color)
				++backward;
			if (forward + backward - 1 >= 6){
				//we found 6 in a row while there is no chip that went pass them
				//so we must restore everything we've done
				UndoLastMove();
				return 0;
			}
		}
		return 1;
	}
	void MoveOneChipEnemy(int starting_cell, int ending_cell, int color){
		previous_moves_.push(OneChangeOfField(color, out_of_border_[color], last_cell_[color],
			starting_cell, ending_cell, cells_[starting_cell], cells_[ending_cell % kFieldSize]));
		cells_[starting_cell].number--;
		if (ending_cell >= kFieldSize){
			out_of_border_[color]++;
			last_cell_[color] = kFieldSize;
		}
		else{
			cells_[ending_cell].number++;
			if (cells_[ending_cell].is_occupied == false){
				cells_[ending_cell].is_occupied = true;
				cells_[ending_cell].color = color;
				last_cell_[color] = max(last_cell_[color], Convert(ending_cell));
			}
		}
		if (cells_[starting_cell].number == 0){
			cells_[starting_cell].is_occupied = false;
		}
	}
	std::vector <int> GetField(){
		std::vector <int> outcome;
		for (int i = 0; i < kFieldSize; ++i)
			if (cells_[i].is_occupied)
				if (cells_[i].color == 0)
					outcome.push_back(cells_[i].number);
				else
					outcome.push_back(-cells_[i].number);
			else
				outcome.push_back(0);
		return outcome;
	}
	bool IsFirstMove(int color) const{
		return last_cell_[color] == 0;
	}
	bool IsCellOccupied(int number) const{
		return cells_[number].is_occupied;
	}
	bool AllOutOfBorder(bool color){
		return out_of_border_[color] == kChipsOverall;
	}
};