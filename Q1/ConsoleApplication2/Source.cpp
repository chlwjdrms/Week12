#include <iostream>

#define MAX2(a, b)				((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)		    (MAX2(MAX2(a, b), (c)))
#define MAX4(a, b, c, d)	    (MAX2(MAX3(a, b, c), (d)))

using namespace std;

class CellData
{
public:
	double q_[4] = { 0.0, 0.0, 0.0, 0.0 }; // up, down, left, right
	double reward_ = 0.0;

	CellData()
	{}

	double getMaxQ()
	{
		return MAX4(q_[0], q_[1], q_[2], q_[3]);
	}
	void rewardSetting(double reward, double allset) {
		reward_ = reward;
		for (int i = 0; i < 4; i++) {
			q_[i] = allset;
		}
	}
};

class GridWorld
{
public:
	int i_res_, j_res_;
	CellData *q_arr2d_ = nullptr;

	GridWorld(const int& i_res, const int& j_res)
		: i_res_(i_res), j_res_(j_res)
	{
		q_arr2d_ = new CellData[i_res*j_res];
	}

	CellData& getCell(const int& i, const int& j) {
		return q_arr2d_[i + i_res_ * j];
	}

	bool inInside(const int& i, const int&j)
	{
		if (i < 0) return false;
		if (i >= i_res_) return false;

		if (j < 0) return false;
		if (j >= j_res_) return false;

		return true;
	}

	void printSigned(const float& v)
	{
		if (v > 0.0f) printf("+%1.1f", v);
		else if (v < 0.0f) printf("%1.1f", v);
		else
			printf(" 0.0");
	}

	void print()
	{
		for (int j = j_res_ - 1; j >= 0; j--)
		{
			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printf("   "); printSigned(cell.q_[0]); printf("   ");
				printf("   ");
			}

			printf("\n");

			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printSigned(cell.q_[2]); printf("   "); printSigned(cell.q_[3]);
				printf("  ");
			}

			printf("\n");

			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);

				printf("   "); printSigned(cell.q_[1]); printf("   ");
				printf("   ");
			}

			printf("\n\n");
		}
	}
};

class Agent
{
public:
	int i_, j_;    // position in grid world
	double reward_;   // reward sum

	Agent()
		: i_(0), j_(0), reward_(0.0)
	{}
};

int main()
{
	cout << "Hello, grid world!" << endl;
	cout << endl;

	const int world_res_i = 3, world_res_j = 2;

	GridWorld world(world_res_i, world_res_j);




	// reference return type
	/*world.getCell(i, j).reward_ = 10;

	CellData &old_cell = world.getCell(i, j);
	old_cell.reward_ = 10;

	for (int i = 0; j < world_res_j; j++)
	for (int i = 0; i < world_res_i; i++)
	{
	world.getCell(i, j).reward_ = -0.1;
	}*/

	// reward setting
	world.getCell(2, 1).rewardSetting(1, 0.9);
	world.getCell(2, 0).rewardSetting(-1, -0.1);


	Agent my_agent;

	world.print();

	for (int t = 0; t < 1000000; t++)
	{
		const int action = rand() % 4; // random policy

		int i = my_agent.i_, j = my_agent.j_;
		int i_old = i, j_old = j; // 이동하기 전

								  // 다음 확인용
		switch (action)
		{
		case 0:
			j++; //up
			break;
		case 1:
			j--; // down
			break;
		case 2:
			i--; // left
			break;
		case 3:
			i++; //right
			break;
		}

		if (world.inInside(i, j) == true) // move robot if available
		{
			// move agent
			if (world.getCell(i, j).reward_ != -1) {
				my_agent.i_ = i;
				my_agent.j_ = j;

				// update reward (r_t)
				if (world.getCell(i, j).reward_ == 1) {
					my_agent.reward_++;
				}
				else if (world.getCell(i, j).reward_ == -1) {
					my_agent.reward_--;
				}

				// update q values of previous cell (q_t)
				if (action == 0) { // up
					world.getCell(i_old, j_old).q_[0] = world.getCell(i, j).getMaxQ(); // 0: up, 1: down, 2: left, 3:right
				}
				else if (action == 1) { // down
					world.getCell(i_old, j_old).q_[1] = world.getCell(i, j).getMaxQ();
				}
				else if (action == 2) { // left
					world.getCell(i_old, j_old).q_[2] = world.getCell(i, j).getMaxQ();
				}
				else if (action == 3) { // right
					world.getCell(i_old, j_old).q_[3] = world.getCell(i, j).getMaxQ();
				}


				// reset if agent is in final cells
				if (i == 2 && j == 1) {
					// my_agent 위치 초기화
					my_agent.i_ = 0;
					my_agent.j_ = 0;
				}
			}
		}
		else
		{
			// you may give negative reward (penalty) to agent.
		}

		/*cout << "Agent status" << my_agent.i_ << " " << my_agent.j_ << endl;
		cout << "action" << action << endl;

		world.print();*/
	}
	cout << "==================================== result ====================================" << endl;
	world.print();

	return 0;
}

