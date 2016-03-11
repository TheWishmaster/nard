#include <vector>
#include <utility>
#include <stack>
#include <algorithm>
#include <cmath>
#include "field.h"
#include "strategies.h"

class Player{
private:
	bool my_color_;
	Field this_field_;
	IStrategy *chosen_strategy_;

	bool StrangeFirstMove(const Field &this_field, bool my_color, int dice){
		if (dice != 6 && dice != 4 && dice != 3)
			return 0;
		if (!this_field.IsFirstMove(my_color))
			return 0;
		if (dice == 4 && this_field.IsCellOccupied(8))
			return 0;
		return 1;
	}

	void GeneratePossibleMoves(Field &this_field, bool my_color, static std::vector <int> &single_moves, 
		int left, int from_head, TMove &generated_prefix, std::vector <TMove> &destination){
		if (generated_prefix.size() > 0){
			destination.push_back(generated_prefix);
		}
		if (left == 0)
			return;
		for (int i = int(from_head == 0); i < kFieldSize; ++i)
			if (this_field.MoveOneChip(i, i + single_moves[left - 1], my_color)){
				generated_prefix.push_back({ i, i + single_moves[left - 1] });
				GeneratePossibleMoves(this_field, my_color, single_moves, left - 1, from_head - int(i == 0), generated_prefix, destination);
				this_field.UndoLastMove();
				generated_prefix.pop_back();
			}
	}

	std::vector <TMove> GetGoodMoves(Field &this_field, bool my_color, const std::pair<int, int>& dice){
		std::vector<TMove> possible_moves, good_moves;
		TMove buffer;
		std::vector <int> single_moves;
		if (dice.first == dice.second)
			single_moves = { dice.first, dice.first, dice.first, dice.first };
		else
			single_moves = { dice.first, dice.second };
		if (single_moves.size() == 4 && StrangeFirstMove(this_field, my_color, dice.first))
			GeneratePossibleMoves(this_field, my_color, single_moves, single_moves.size(), 2, buffer, possible_moves);
		else
			GeneratePossibleMoves(this_field, my_color, single_moves, single_moves.size(), 1, buffer, possible_moves);
		if (single_moves.size() == 2){
			std::swap(single_moves[0], single_moves[1]);
			GeneratePossibleMoves(this_field, my_color, single_moves, single_moves.size(), 1, buffer, possible_moves);
		}
		if (possible_moves.size() == 0)
			return good_moves;
		int max_way = possible_moves[0].size(), first_move_length = 0;
		for (int i = 1; i < possible_moves.size(); ++i)
			max_way = max(max_way, possible_moves[i].size());
		if (max_way == 0)
			for (int i = 0; i < possible_moves.size(); ++i)
				if (max_way == possible_moves[i].size())
					first_move_length = max(first_move_length, possible_moves[i][0].second - possible_moves[i][0].first);
		for (int i = 0; i < possible_moves.size(); ++i)
			if (max_way == possible_moves[i].size() && possible_moves[i][0].second - possible_moves[i][0].first >= first_move_length)
				good_moves.push_back(possible_moves[i]);
		return good_moves;
	}

public:
	Player(bool color, int strategy_type): this_field_(color){
		my_color_ = color;
		if (strategy_type == 0)
			chosen_strategy_ = new RandomStrategy;
	}
	std::vector <int> GetState(){
		return this_field_.GetField();
	}
	void UpdateState(const TMove& enemyMove){
		for (int i = 0; i < enemyMove.size(); ++i)
			this_field_.MoveOneChipEnemy(this_field_.Convert(enemyMove[i].first, my_color_), this_field_.Convert(enemyMove[i].second, my_color_), !my_color_);
	}
	TMove Move(const std::pair<int, int>& dice){
		std::vector <TMove> good_moves = GetGoodMoves(this_field_, my_color_, dice);
		TMove chosen_move = chosen_strategy_->Move(good_moves, GetState());
		for (int i = 0; i < chosen_move.size(); ++i){
			this_field_.MoveOneChip(chosen_move[i].first, chosen_move[i].second, my_color_);
			chosen_move[i].first = this_field_.Convert(chosen_move[i].first, my_color_);
			chosen_move[i].second = this_field_.Convert(chosen_move[i].second, my_color_);
		}
		return chosen_move;
	}
	bool IWin(){
		return this_field_.AllOutOfBorder(my_color_);
	}
};

class Judge{

private:
	Player first_player_, second_player_;
	int game_length_, first_player_color_;
public:
	Judge(int first_player_color, int first_player_strategy = 0, int second_player_strategy = 0) :
		first_player_(first_player_color, first_player_strategy),
		second_player_(!first_player_color, second_player_strategy),
		first_player_color_(first_player_color),
		game_length_(0){}
	bool Move(){
		if (game_length_ % 2 == 0){
			second_player_.UpdateState(first_player_.Move({rand() % 6 + 1, rand() % 6 + 1}));
			if (first_player_.IWin())
				return 1;
		}
		else{
			first_player_.UpdateState(second_player_.Move({ rand() % 6 + 1, rand() % 6 + 1 }));
			if (second_player_.IWin())
				return 1;
		}
		++game_length_;
		return 0;
	}
	std::vector <int> GetState(){
		if (!first_player_color_)
			return first_player_.GetState();
		return second_player_.GetState();
	}
	bool WhoWon(){
		if (first_player_.IWin())
			return 0;
		return 1;
	}
};

double RunGames(int games_number, int second_player_strategy = 0, int first_player_strategy = 0){
	int first_player_wins = 0;
	for (int i = 0; i < games_number / 2; ++i){
		Judge current_game(0, first_player_strategy, second_player_strategy);
		while (!current_game.Move()){
			first_player_wins += current_game.WhoWon() == 0;
		}
	}
	for (int i = games_number / 2; i < games_number; ++i){
		Judge current_game(1, first_player_strategy, second_player_strategy);
		while (!current_game.Move()){
			first_player_wins += current_game.WhoWon() == 0;
		}
	}
	return 1. * first_player_wins / games_number;
}