#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

class NodeCoords;
class ImportantInfo;

int min_element_index(std::vector<int> vec);
int calcManhattan(const NodeCoords &, const NodeCoords &);
int calcScore(const NodeCoords &, const NodeCoords &, const NodeCoords &);
std::vector<std::vector<char>> &getMap(ImportantInfo &);
std::string Astar(std::vector<std::vector<char>>, ImportantInfo &);

class NodeCoords
{

	public:
	
		NodeCoords() : x(-1),y(-1) {}
		NodeCoords(int x, int y) : x(x), y(y) {}

		bool operator==(const NodeCoords &coords)
		{
			if (this->x == coords.x && this->y == coords.y)
				return true;
			return false;
		}

		bool isContained(std::vector<NodeCoords *> &closedCoordinates)
		{
			for (int i = 0; i < closedCoordinates.size(); i++)
				if (*closedCoordinates[i] == *this)
					return true;
			return false;
		}

		int getX() const { return x; }
		int getY() const { return y; }

	private:
		int x, y;
};


class ImportantInfo
{
	public:
		ImportantInfo()
		{
			playerOnGoal = false;
		}

		void useClosestGoal(const NodeCoords &current)
		{
			if (goals.size() == 0)
				return;

			int min = calcManhattan(current, goals[0]);
			int goalX = goals[0].getX(), goalY = goals[0].getY();
			
			for (int i = 1; i < goals.size(); i++)
			{
				int distance = calcManhattan(current, goals[i]);
				if (distance < min)
				{
					min = distance;
					goalX = goals[i].getX();
					goalY = goals[i].getY();
				}
			}

			goal = NodeCoords(goalX, goalY);
		}

		void setPlayerOnGoal(bool var) { playerOnGoal = var; }
		void setPlayer(NodeCoords player) { this->player = player;}
		void addGoal(NodeCoords goal) { goals.push_back(goal); }
		int getPlayerX() const { return player.getX(); }
		int getPlayerY() const { return player.getY(); }
		NodeCoords getPlayer() const { return player; }
		NodeCoords getGoal() const { return goal; }
		bool getPlayerOnGoal() const { return playerOnGoal; }
		int getGoalCount() const { return goals.size(); }
	private:
		NodeCoords player;
		NodeCoords goal;
		bool playerOnGoal;
		std::vector<NodeCoords> goals;
};




int main(int argc, char *argv[])
{
	std::string playerMovements;
	int rows = 0, columns = 0;
	std::vector<std::vector<char>> map;
	ImportantInfo info;

	
	map = getMap(info);
	
	//check if the player or a box was found on the goal
	if (info.getPlayerOnGoal())
	{
		std::cout << "" << std::endl;
		return 0;
	}
	else if (info.getGoalCount() == 0)
	{
		std::cout << "no path" << std::endl;
		return 0;
	}

	playerMovements = Astar(map, info);
	std::cout << playerMovements << std::endl;

	return 0;
}



int min_element_index(std::vector<int> vec)
{
	std::size_t index = 0;
	int min = vec[0];
	for (int i = 1; i < vec.size(); i++)
		if (vec[i] <= min)
		{
			index = i;
			min = vec[i];
		}

	return static_cast<int>(index);
}



int calcManhattan(const NodeCoords &node1, const NodeCoords &node2)
{
	return abs(node1.getX() - node2.getX()) + abs(node1.getY() - node2.getY());
}



int calcScore(const NodeCoords &coords, const NodeCoords &player, const NodeCoords &goal)
{
	int g, h;
	g = calcManhattan(coords, player);
	h = calcManhattan(coords, goal);
	return g + h;
}


//returns a reference to a vector of vectors
std::vector<std::vector<char>> &getMap(ImportantInfo &info)
{
	std::vector<std::vector<char>> *map = new std::vector<std::vector<char>>;
	std::string inputLine;
	int i = 0;

	while (std::getline(std::cin, inputLine))
	{
		std::vector<char> row;
		for (int j = 0; j < static_cast<int>(inputLine.size()); j++)
		{
			row.push_back(inputLine[j]);
			if (inputLine[j] == '@')
				info.setPlayer(NodeCoords(i, j));
			else if (inputLine[j] == '.')
				info.addGoal(NodeCoords(i, j));
			else if (inputLine[j] == '+')
			{
				info.setPlayerOnGoal(true);
				return *map;
			}
		}

		i++;
		map->push_back(row);
	}

	return *map;
}




std::string Astar(std::vector<std::vector<char>> map, ImportantInfo &info)
{
	std::vector<NodeCoords *> closedCoordinates, openCoordinates;
	std::vector<int> fScores;
	std::vector<std::string> playerMovements;
	char movementOperators[4] = { 'D','U','R','L' };

	info.useClosestGoal(info.getPlayer());

	openCoordinates.push_back(new NodeCoords(info.getPlayerX(), info.getPlayerY()));
	fScores.push_back(calcScore(*openCoordinates[0], info.getPlayer(), info.getGoal()));
	playerMovements.push_back("");

	while (!openCoordinates.empty())
	{
		int minIndex = min_element_index(fScores);
		std::string currentPlayerMoves = playerMovements[minIndex];
		NodeCoords *coords = openCoordinates[minIndex];
		NodeCoords **neighbour = new NodeCoords*[4];

		/*DEBUGGING OUTPUT COMMANDS

		std::cout << "CLOSED SET (";
		for (int i = 0; i < closedCoordinates.size(); i++)
		std::cout << "(" << closedCoordinates[i]->getX() << "," << closedCoordinates[i]->getY() << ") ";
		std::cout << ")" << std::endl;

		std::cout << "OPEN SET (";
		for (int i = 0; i < openCoordinates.size(); i++)
		std::cout << "(" << openCoordinates[i]->getX() << "," << openCoordinates[i]->getY() << ") ";
		std::cout << ")" << std::endl;

		std::cout << "SCORES (";
		for (int i = 0; i < fScores.size(); i++)
		std::cout << " \'" << fScores[i] << "\'";
		std::cout << ")" << std::endl;

		std::cout << " PLAYER MOVEMENTS (";
		for (int i = 0; i < playerMovements.size(); i++)
		std::cout << " \"" << playerMovements[i] << "\"";
		std::cout << ")" << std::endl;

		std::cout << "EXTRACTED NODE: (" << coords->getX() << "," << coords->getY() << ")" << std::endl;
		std::cout << "\n\n\n";
		*/

		openCoordinates.erase(openCoordinates.begin() + minIndex);
		playerMovements.erase(playerMovements.begin() + minIndex);
		fScores.erase(fScores.begin() + minIndex);
		closedCoordinates.push_back(coords);

		//check if coords x y are the same with a goal s of a map (check if the character of the map is a dot '.')
		if (map[coords->getX()][coords->getY()] == '.')
			return currentPlayerMoves;

		neighbour[0] = new NodeCoords(coords->getX() + 1, coords->getY());
		neighbour[1] = new NodeCoords(coords->getX() - 1, coords->getY());
		neighbour[2] = new NodeCoords(coords->getX(), coords->getY() + 1);
		neighbour[3] = new NodeCoords(coords->getX(), coords->getY() - 1);

		//change to the closest goal for the cost calculations
		info.useClosestGoal(*coords);

		for (int i = 0; i < 4; i++)
		{
			//what happens if it is contained in open 
			if (neighbour[i]->isContained(closedCoordinates)
				|| map[neighbour[i]->getX()][neighbour[i]->getY()] == '#'
				|| map[neighbour[i]->getX()][neighbour[i]->getY()] == '$'
				|| map[neighbour[i]->getX()][neighbour[i]->getY()] == '*')
				continue;

			openCoordinates.push_back(neighbour[i]);
			fScores.push_back(calcScore(*neighbour[i], info.getPlayer(), info.getGoal()));
			playerMovements.push_back(currentPlayerMoves + movementOperators[i]);
		}
	}

	return "no path";
}


