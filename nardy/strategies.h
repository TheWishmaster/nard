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
	virtual TMove Move(const std::vector<int>& board, const std::vector <TMove> &possible_moves) = 0;

};

class CostsStrategy : public IStrategy{

protected:
	std::vector <double> costs;

public:
	virtual std::vector <int> GradeState(std::vector <int> &state) = 0;
	TMove Move(const std::vector<int>& board, const std::vector <TMove> &possible_moves){
		if (possible_moves.size() == 0)
			return{};
		double maximum_quality = 0, best_move = 0, temp;
		std::vector <int> factors_temp;
		std::vector <int> current_state = board;
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

	std::vector <int> GradeStates(std::vector <int> &state){
		std::vector <int> result = GradeState(state), temp;
		ConvertState(state);
		temp = GradeState(state);
		result.insert(result.end(), temp.begin(), temp.end());
		return result;
	}

};

class FirstStrategy : public CostsStrategy{

private:
	//std::vector <int> costs;

public:
	FirstStrategy(){
		costs = { 0.06251882, -0.00723875, 0.05243943, -0.00972646, 0.00464151 };
	}
	std::vector <int> GradeState(std::vector <int> &state){
		std::vector <int> choises;
		int max_in_a_row = 0;
		for (int i = 0; i < state.size(); ++i){
			int current_row = 0;
			while (i + current_row < kFieldSize && state[i + current_row] == 0)
				++current_row;
			max_in_a_row = max(max_in_a_row, current_row);
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
		costs = { 6.19584686e+11, 1.72751439e-02, -6.61581864e+11,
			4.82509285e-04, -3.58004871e+11, 7.02319150e-04,
			-4.78570971e+11, -3.32518114e-03, 8.22063333e+10,
			3.72369535e-03, -1.00570120e+11, -3.49210267e-03,
			1.38084536e+10, -1.19044236e-03, 1.23465333e+11,
			-4.98309398e-03, -9.86267361e+10, 5.54713305e-03,
			3.06051868e+11, 4.67007078e-03, 3.74659279e+11,
			-6.20537604e-03, 2.68015741e+11, 2.23722077e+11,
			8.24051775e+10, -6.55262637e+12, 1.78283029e+11,
			7.48355357e-04, 6.79259242e+10, -1.35554914e-04,
			-3.34026493e+10, -1.26311134e-03, -1.27294000e+11,
			2.23953997e-03, 1.28675746e+11, -3.18923828e-03,
			4.63132341e+10, 1.91007134e-03, 2.44873086e+11,
			3.29991876e-03, -2.80245540e+11, -1.16324634e-03,
			-6.99015807e+10, 3.44414472e-03, 8.57279328e+11,
			-2.56290850e-03, 7.88882010e+08, 3.07412008e-02,
			-1.14017874e+12, -5.43335868e+12, 9.63742641e-02,
			-6.19584686e+11, 3.59179808e-03, 6.61581864e+11,
			4.10539089e-03, 3.58004871e+11, -5.75421298e-03,
			4.78570971e+11, 8.52301125e-03, -8.22063333e+10,
			-3.07020037e-03, 1.00570120e+11, -3.20587666e-03,
			-1.38084536e+10, -1.03002903e-03, -1.23465333e+11,
			-1.07184348e-03, 9.86267361e+10, -2.14389630e-02,
			-3.06051868e+11, 5.36723637e-03, -3.74659279e+11,
			2.69305581e-02, -3.62384690e+12, -3.10196070e-03,
			-8.24051775e+10, 0.00000000e+00, -1.78283029e+11,
			-1.42566212e-02, -6.79259242e+10, -1.81343743e-02,
			3.34026493e+10, 7.15450593e-03, 1.27294000e+11,
			1.58505693e-02, -1.28675746e+11, -2.44229602e-02,
			-4.63132341e+10, -5.56827603e-03, -2.44873086e+11,
			5.30668926e-03, 2.80245540e+11, -8.58392610e-03,
			6.99015807e+10, 2.32585194e-04, -8.57279328e+11,
			1.36402166e-02, -7.88882010e+08, 4.37961075e-02,
			0.00000000e+00, 0.00000000e+00, -5.68452504e-02 };
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
			choises.push_back(current_row > 0);
			//choises.push_back(current_row * current_row);
			//choises.push_back(current_row * blocking_enemies[i]);
		}
		//choises.push_back(max_in_a_row);
		//choises.push_back(max_in_a_row * max_in_a_row);
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
	virtual TMove Move(const std::vector<int>& board, const std::vector <TMove> &possible_moves){
		if (possible_moves.size() == 0)
			return{};
		return possible_moves[rand() % possible_moves.size()];
	};
};