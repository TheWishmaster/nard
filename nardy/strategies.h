#include <vector>
#include <utility>
#include <stack>
#include <algorithm>
#include <cmath>

typedef std::vector<std::pair<int, int> > TMove;

class IStrategy{

public:

	virtual ~IStrategy(){};
	virtual TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state) = 0;
	//virtual void UpdateState(const TMove& enemyMove) = 0;

};

class RandomStrategy : public IStrategy{

private:
	std::vector <int> costs;

public:

	int GradeState(std::vector <int> &state){
		std::vector <bool> choises;
		int max_in_a_row = 0;
		for (int i = 0; i < state.size(); ++i){
			int current_row = 0;
			while (state[i] == 0)
				++current_row;
			max_in_a_row = max(max_in_a_row, current_row);
		}
		for (int i = 0; i < 6; ++i)
			choises.push_back(i < max_in_a_row);
		int result = 0;
		for (int i = 0; i < choises.size(); ++i)
			if (choises[i])
				result += costs[i];
		return result;
	}
	virtual TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state){
		if (possible_moves.size() == 0)
			return{};
		int maximum_quality = 0, best_move = 0, temp;
		for (int i = 0; i < possible_moves.size(); ++i){
			for (int j = 0; j < possible_moves[i].size(); ++j){
				current_state[possible_moves[i][j].first]--;
				current_state[possible_moves[i][j].second]++;
			}
			temp = GradeState(current_state);
			if (temp < maximum_quality){
				maximum_quality = temp;
				best_move = i;
			}
			for (int j = 0; j < possible_moves[i].size(); ++j){
				current_state[possible_moves[i][j].first]++;
				current_state[possible_moves[i][j].second]--;
			}
		}
		return possible_moves[best_move];
	};
};

class FirstStrategy : public IStrategy{
	virtual TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state){
		if (possible_moves.size() == 0)
			return{};
		return possible_moves[rand() % possible_moves.size()];
	};
};