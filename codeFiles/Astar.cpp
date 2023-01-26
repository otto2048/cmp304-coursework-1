#include "Astar.h"

//coord constructors
Astar::coord::coord()
{
	value = 0;
	x = 0;
	y = 0;
}

Astar::coord::coord(int a, int b)
{
	x = a;
	y = b;
	value = 0;
}

Astar::coord::coord(int a, int b, int c)
{
	x = a;
	y = b;
	value = c;
}

//overloading operators for coord struct
Astar::coord& Astar::coord::operator=(const Astar::coord& obj)
{
	x = obj.x;
	y = obj.y;

	f = obj.f;
	g = obj.g;
	h = obj.h;
	value = obj.value;
	neighbours = obj.neighbours;
	parent = obj.parent;

	return *this;
}

bool Astar::coord::operator!=(const coord& other)
{
	if (x == other.x && y == other.y)
	{
		return false;
	}
	return true;
}

bool Astar::coord::operator!=(const coord& other) const
{
	if (x == other.x && y == other.y)
	{
		return false;
	}
	return true;
}

bool Astar::coord::operator==(const coord& other) const
{
	if (x == other.x && y == other.y)
	{
		return true;
	}
	return false;
}

bool Astar::coord::operator<(const coord& other) const
{
	if (g < other.g)
	{
		return true;
	}
	else if (g == other.g)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Astar::coord::operator>(const coord& other) const
{
	if (g > other.g)
	{
		return true;
	}
	else if (g == other.g)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//custom comparison operator
bool Astar::compare::operator() (const pair<int, coord>& lhs, const pair<int, coord>& rhs) const
{
	//only compare coord if the ints are equal
	if (lhs.first == rhs.first) {
		//if g values are equal, check x & y values
		if (lhs.second.g == rhs.second.g)
		{
			if (lhs.second.x == rhs.second.x)
			{
				return lhs.second.y < rhs.second.y;
			}
			return lhs.second.x < rhs.second.x;
		}
		return lhs.second > rhs.second;
	}

	//compare int values
	return lhs.first < rhs.first;
}

//comparing pair<int, Astar::coord>
bool operator==(const pair<int, Astar::coord>& lhs, const pair<int, Astar::coord>& rhs)
{
	return lhs.second == rhs.second;
}

//run A* algorithm
bool Astar::astar(Astar::coord start, Astar::coord end)
{
	//check if start or end point is an obstacle
	if (grid[start.y][start.x] == -2 || grid[end.y][end.x] == -2)
	{
		if (!path.empty())
		{
			path.clear();
		}
		return false;
	}

	//compute f/g/h for start node
	start.g = 0;
	start.f = start.g + heuristic(start, end);

	//put start node in open set
	openSet.insert(std::make_pair(start.f, start));
	int nodesVisited = 1;

	bool check = false;

	coord current;
	//while the open set is not empty
	while (!openSet.empty())
	{
		//pick the most promising node to look at next
		current = (openSet.begin())->second;

		//if current node is end, found the path, break
		if (current == end)
		{
			break;
		}

		//remove from open set and add to closed set

		auto it = find_if(openSet.begin(), openSet.end(), [&current](const pair<int, Astar::coord>& element) {return element.second.x == current.x && element.second.y == current.y; });
		openSet.erase(it);

		closedSet.insert(std::make_pair(current.f, current));
		parents.push_back(std::make_pair(current.f, current));
		nodesVisited = nodesVisited++;

		//set neightbours of current node
		findNeighbours(current);

		for (coord node : current.neighbours)
		{
			int distance = dist_between();
			int new_g = current.g + distance;
			
			auto nodePair = std::make_pair(node.f, node);
			auto closeIt = find_if(closedSet.begin(), closedSet.end(), [&nodePair](const pair<int, Astar::coord>& element) {return element == nodePair;  });

			//if closed set contains item
			if (closeIt != closedSet.end())
			{
				auto test = *closeIt;

				if (test.second.g > new_g)
				{
					//need to look at this node again
					closedSet.erase(closeIt);

					check = true;
				}
				else
				{
					//already finished with this node
					continue;
				}
			}

			//if open set contains item
			auto openIt = find_if(openSet.begin(), openSet.end(), [&nodePair](const pair<int, Astar::coord>& element) {return element == nodePair; });

			if (openIt != openSet.end())
			{
				auto test = *openIt;

				if (test.second.g > new_g)
				{
					openSet.erase(openIt);
				}
				else
				{
					continue;
				}
			}

			//compute f/g/h for this node
			node.g = new_g;
			node.f = node.g + heuristic(node, end);

			//parent is current node
			node.parent = &(find_if(parents.begin(), parents.end(), [&current](const pair<int, Astar::coord>& element) {return element.second.x == current.x && element.second.y == current.y; }))->second;

			openSet.insert(std::make_pair(node.f, node));
			nodesExpanded++;
		}
	}

	vector<coord> coordinates;
	bool retValue = false;

	//if last node looked at was the end node (path successfully found)
	if (current == end)
	{
		retValue = true;
	}

	coord point = current;

	//construct path
	while (point != start)
	{
		coordinates.push_back(point);
		point = *point.parent;
	}

	coordinates.push_back(start);

	reverse(coordinates.begin(), coordinates.end());

	path = coordinates;

	return retValue;
}

//find the distance between two points
int Astar::dist_between()
{
	//randomising path, return random number
	int ret = rand() % 1000;
	return ret;
}

//calculate heuristic between points
//heuristic is manhatten distance as we can only travel in NSEW directions
int Astar::heuristic(Astar::coord start, Astar::coord end)
{
	int D = 1;
	return D * (abs(start.x - end.x) + abs(start.y - end.y));
}

//find the neighbours for a node
void Astar::findNeighbours(coord& node)
{
	//check each direction
	//if its not an obstacle add to neighbours

	coord testCoord = node;

	//north
	testCoord.y = node.y + 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		setValueWithCoord(testCoord);
		addToNeighbours(testCoord, node);
	}

	//south
	testCoord.y = node.y - 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		setValueWithCoord(testCoord);
		addToNeighbours(testCoord, node);
	}

	//west
	testCoord.y = node.y;
	testCoord.x = node.x - 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		setValueWithCoord(testCoord);
		addToNeighbours(testCoord, node);
	}

	//east
	testCoord.x = node.x + 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		setValueWithCoord(testCoord);
		addToNeighbours(testCoord, node);
	}
}

//adding neighbours to node
void Astar::addToNeighbours(coord& toAdd, coord& node)
{
	//if the point isnt an obstacle, or in points visited, add to neighbours
	if (toAdd.value != -2 && find(pointsVisited.begin(), pointsVisited.end(), toAdd) == pointsVisited.end())
	{
		toAdd.parent = &node;
		node.neighbours.push_back(toAdd);
	}
}

//check if a node is a valid coordinate in the grid
bool Astar::validCoordinates(coord& coord)
{
	if (coord.x > -1 && coord.x < width && coord.y > -1 && coord.y < height)
	{
		return true;
	}

	return false;
}

void Astar::setValueWithCoord(Astar::coord& coord)
{
	coord.value = grid[coord.y][coord.x];
}

vector<Astar::coord> Astar::getPath()
{
	return path;
}

vector<vector<int>> Astar::getGrid()
{
	return grid;
}

void Astar::setGrid(vector<vector<int>> grid_)
{
	grid = grid_;
}

void Astar::resetAlgorithm()
{
	openSet.clear();
	closedSet.clear();
	parents.clear();
	nodesExpanded = 0;
}

int Astar::getHeight()
{
	return height;
}

int Astar::getWidth()
{
	return width;
}

void Astar::setHeight(int h)
{
	height = h;
}

void Astar::setWidth(int w)
{
	width = w;
}

int Astar::getNodesExpanded()
{
	return nodesExpanded;
}