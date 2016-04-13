#include <vector>
#include <utility>
#include <stack>
#include <algorithm>
#include <cmath>

typedef std::vector<std::pair<int, int> > TMove;

//double RunGames(int white_games_number, int black_games_number, int second_strategy = 0, int first_strategy = 0, std::vector <int> state = {}, bool grade = 0);

class IStrategy{

public:

	virtual ~IStrategy(){};
	virtual TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state) = 0;
	//virtual void UpdateState(const TMove& enemyMove) = 0;

};

class CostsStrategy : public IStrategy{

protected:
	std::vector <double> costs;

public:
	virtual std::vector <int> GradeState(std::vector <int> &state) = 0;
	TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state){
		if (possible_moves.size() == 0)
			return{};
		double maximum_quality = 0, best_move = 0, temp;
		std::vector <int> factors_temp;
		for (int i = 0; i < possible_moves.size(); ++i){
			for (int j = 0; j < possible_moves[i].size(); ++j){
				current_state[possible_moves[i][j].first]--;
				if (possible_moves[i][j].second < kFieldSize)
					current_state[possible_moves[i][j].second]++;
			}
			factors_temp = GradeState(current_state);
			temp = 0;
			//if (vc.size() < 2000 && rand() % 10 == 0)
			//	vc.push_back(make_pair(factors_temp, RunGames(50, 50, 0, 0, current_state)));
			for (int j = 0; j < factors_temp.size(); ++j)
				temp += factors_temp[j] * costs[j];
			if (temp < maximum_quality){
				maximum_quality = temp;
				best_move = i;
			}
			for (int j = 0; j < possible_moves[i].size(); ++j){
				current_state[possible_moves[i][j].first]++;
				if (possible_moves[i][j].second < kFieldSize)
					current_state[possible_moves[i][j].second]--;
			}
		}
		return possible_moves[best_move];
	};

};

class FirstStrategy : public CostsStrategy{

private:
	//std::vector <int> costs;

public:
	FirstStrategy(){
		costs = { 10, 100, 100, 1, 50 };
	}
	std::vector <int> GradeState(std::vector <int> &state){
		std::vector <int> choises;
		int max_in_a_row = 0;
		for (int i = 0; i < state.size(); ++i){
			int current_row = 0;
			while (i + current_row < kFieldSize && state[i + current_row] == 0)
				++current_row;
			max_in_a_row = std::max(max_in_a_row, current_row);
		}
		choises.push_back(max_in_a_row);
		choises.push_back(max_in_a_row * max_in_a_row);
		int all = 0, occupied = 0;
		for (int i = 0; i < kFieldSize; ++i) {
			if (state[i] > 0){
				all += state[i];
				++occupied;
			}
		}
		choises.push_back(kChipsOverall - all);
		choises.push_back(occupied);
		int home = 0;
		for (int i = kFieldSize * 3 / 4; i < kFieldSize; ++i) {
			if (state[i] > 0)
				home += state[i];
		}
		choises.push_back(home);
		return choises;
	}
};

class ManyFactorsStrategy : public CostsStrategy{

private:
	//std::vector <double> costs;

public:
	ManyFactorsStrategy(){
		costs = { 1.21074987e-03, -1.16501357e-04, 1.78279961e-06,
			-3.60646616e-05, -7.43443597e-05, 1.53099455e-05,
			-3.50874712e-05, 1.18537311e-04, -5.93237222e-05,
			1.94908402e-04, 5.48193574e-05, 1.35753795e-05,
			1.15254154e-04, 4.08688853e-05, -6.37611702e-05,
			-7.42003739e-04, -3.55182017e-05, 7.22608702e-05,
			-4.37444329e-04, -2.76435635e-04, 1.67347809e-04,
			-1.29910260e-03, 1.38234427e-04, -9.15234939e-07,
			1.67957858e-03, 1.18802112e-03, -3.41230598e-04,
			-8.02878713e-04, -8.99154300e-04, 1.59616060e-04,
			3.25866745e-03, -2.50514052e-03, 2.17596064e-05,
			3.89505436e-02, -5.79589394e-04, -2.38965725e-03,
			-4.73859528e-04, 4.43005866e-05, -1.55974633e+12,
			-8.95125924e-04, 1.86284640e-04, -2.95297731e-05,
			7.52270553e-04, -1.62663239e-04, 3.08226616e-05,
			4.74549051e-04, -3.83041502e-05, -4.67186177e-05,
			1.39550869e-03, -5.78171180e-04, 1.21782870e-04,
			-4.69406429e-04, -1.84907922e-05, 1.32037541e-04,
			1.90288210e-03, 6.02391747e-04, -2.53085646e-04,
			-4.17015871e-03, 3.11404873e-04, 1.69177269e-04,
			-1.96054135e-03, 2.88590814e-04, 1.23574111e-04,
			2.99013786e-03, 3.31546286e-04, -3.30856004e-04,
			-5.50617084e-03, 1.53506153e-03, 2.16896715e-04,
			4.90260139e+11, -4.90260139e+11, -4.49402678e-04,
			0.00000000e+00, 0.00000000e+00, 4.39876268e-04 };
	}
	std::vector <int> GradeState(std::vector <int> &state){
		std::vector <int> choises;
		std::vector <int> blocking_enemies(kFieldSize);
		for (int i = 0; i < state.size() / 2; ++i)
			if (state[i] < 0){
				blocking_enemies[kFieldSize / 2] += state[i];
				blocking_enemies[i] -= state[i];
			}
		for (int i = kFieldSize / 2; i < state.size(); ++i)
			if (state[i] < 0){
				blocking_enemies[i] -= state[i];
				blocking_enemies[kFieldSize / 2] += state[i];
				blocking_enemies[0] -= state[i];
			}
		for (int i = 1; i < kFieldSize; ++i)
			blocking_enemies[i] += blocking_enemies[i - 1];
		int max_in_a_row = 0;
		for (int i = 0; i < state.size(); ++i){
			int current_row = 0;
			while (i + current_row < kFieldSize && state[i + current_row] == 0)
				++current_row;
			choises.push_back(current_row);
			choises.push_back(current_row * current_row);
			choises.push_back(current_row * blocking_enemies[i]);
		}
		choises.push_back(max_in_a_row);
		choises.push_back(max_in_a_row * max_in_a_row);
		int all = 0;
		for (int i = 0; i < kFieldSize; ++i) {
			if (state[i] > 0){
				all += state[i];
			}
		}
		choises.push_back(kChipsOverall - all);
		return choises;
	}
};

class RandomStrategy : public IStrategy{
	virtual TMove Move(std::vector <TMove> possible_moves, std::vector <int> &current_state){
		if (possible_moves.size() == 0)
			return{};
		return possible_moves[rand() % possible_moves.size()];
	};
};