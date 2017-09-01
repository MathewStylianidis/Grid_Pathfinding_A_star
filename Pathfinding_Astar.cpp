#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>


int min_element_index(std::vector<int> vec)
{
	std::size_t index = 0;
	int min = vec[0];
	for(int i = 1; i < vec.size(); i++)
		if (vec[i] <= min)
		{
			index = i;
			min = vec[i];
		}

	return static_cast<int>(index);
}

struct ImportantInfo
{
	int playerX, playerY;
	int goalX, goalY;
	bool playerOnGoal;

	ImportantInfo::ImportantInfo()
	{
		playerOnGoal = false;
		playerX = playerY = -1;
	}
};
 
class NodeCoords
{

	public:
	
		NodeCoords::NodeCoords() : x(-1),y(-1) {}
		NodeCoords::NodeCoords(int x, int y) : x(x), y(y) {}

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

		int getX() { return x; }
		int getY() { return y; }

	private:
		int x, y;
};

//returns a reference to a vector of vectors
std::vector<std::vector<char>> &getMap(struct ImportantInfo &info)
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
			{
				info.playerX = i;
				info.playerY = j;
			}
			else if (inputLine[j] == '.')
			{
				info.goalX = i;
				info.goalY = j;
			}
			else if (inputLine[j] == '+')
			{
				info.playerOnGoal = true;
				return *map;
			}
		}

		i++;
		map->push_back(row);
	}

	return *map;
}

int calcScore(NodeCoords &coords, struct ImportantInfo &info)
{
	int g, h;		
	g = abs(coords.getX() - info.playerX) + abs(coords.getY() - info.playerY);
	h = abs(coords.getX() - info.goalX) + abs(coords.getY() - info.goalY);
	return g + h;
}

std::string Astar(std::vector<std::vector<char>> map , struct ImportantInfo &info)
{
	std::vector<NodeCoords *> closedCoordinates, openCoordinates;
	std::vector<int> fScores;
	std::vector<std::string> playerMovements;
	char movementOperators[4] = { 'D','U','R','L' };

	
	openCoordinates.push_back(new NodeCoords(info.playerX, info.playerY));
	fScores.push_back(calcScore(*openCoordinates[0], info));
	playerMovements.push_back("");

	while (!openCoordinates.empty())
	{
		int minIndex = min_element_index(fScores);
		std::string currentPlayerMoves = playerMovements[minIndex];
		NodeCoords *coords = openCoordinates[minIndex];
		NodeCoords **neighbour = new NodeCoords*[4];
		
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
		
		for (int i = 0; i < 4; i++)
		{
		 //what happens if it is contained in open 
			if (neighbour[i]->isContained(closedCoordinates)
				|| map[neighbour[i]->getX()][neighbour[i]->getY()] == '#'
				|| map[neighbour[i]->getX()][neighbour[i]->getY()] == '$')
				continue;

			openCoordinates.push_back(neighbour[i]);
			fScores.push_back(calcScore(*neighbour[i], info));
			playerMovements.push_back(currentPlayerMoves + movementOperators[i]);
		}
	}	 

	return "no path";
}




int main(int argc, char *argv[])
{
	std::string playerMovements;
	int rows = 0, columns = 0;
	std::vector<std::vector<char>> map;
	struct ImportantInfo info;

	
	map = getMap(info);

	
	for (int i = 0; i < map.size(); i++)

	{
		for (int j = 0; j < map[i].size(); j++)
			std::cout << map[i][j]; 
		std::cout << std::endl;
	}

	
	//check if the player or a box was found on the goal
	if (info.playerOnGoal)
	{
		std::cout << std::endl;
		return 0;
	}

	playerMovements = Astar(map, info);
	std::cout << playerMovements << std::endl;

	return 0;
}
