#include <vector>
#include <utility>
#include <stack>
#include <algorithm>
#include <cmath>

typedef std::vector<std::pair<int, int> > TMove;

class IStrategy{

public:

	virtual ~IStrategy(){};
	virtual TMove Move(std::vector <TMove> possible_moves) = 0;
	//virtual void UpdateState(const TMove& enemyMove) = 0;

};

class RandomStrategy : public IStrategy{
	virtual TMove Move(std::vector <TMove> possible_moves){
		if (possible_moves.size() == 0)
			return{};
		return possible_moves[rand() % possible_moves.size()];
	};
};