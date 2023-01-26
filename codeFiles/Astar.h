#pragma once
#include <set>
#include <list>
#include <algorithm>
#include <cstdlib> // random numbers header file//
#include <ctime> // used to get date and time information
#include <vector>

#include <string>

#include <deque>
#include <iterator>

using std::list;
using std::set;
using std::pair;
using std::deque;
using std::vector;
using std::string;

using std::find_if;

class Astar
{
private:

	vector<vector<int>> grid;

	int width, height;

	int nodesExpanded = 0;

public:

	struct coord {
		int x, y;
		int f, g, h;
		int value = -1;
		list<coord> neighbours;
		const coord* parent = nullptr;

		coord();
		coord(int, int);
		coord(int, int, int);

		bool operator!=(const coord&);
		bool operator!=(const coord&) const;
		bool operator==(const coord&) const;
		bool operator<(const coord&) const;
		bool operator>(const coord&) const;

		coord& operator= (const coord& obj);
	};

	struct compare
	{
		//custom comparison function for open and closed sets
		bool operator() (const pair<int, coord> &lhs, const pair<int, coord> &rhs) const;
	};

	bool astar(coord, coord);
	int dist_between();
	int heuristic(coord, coord);

	void findNeighbours(coord&);
	void addToNeighbours(coord&, coord&);

	bool validCoordinates(coord&);
	void setValueWithCoord(coord& coord);

	//getters and setters
	vector<coord> getPath();

	vector<vector<int>> getGrid();
	void setGrid(vector<vector<int>>);

	void resetAlgorithm();

	int getHeight();
	int getWidth();
	void setHeight(int);
	void setWidth(int);

	int getNodesExpanded();

	//contains the points already visited by the algorithm, equivalent to obstacles
	vector<coord> pointsVisited;

private:
	//path found by algorithm
	vector<coord> path;

	//open and closed sets
	set<pair<int, coord>, compare> openSet;
	set<pair<int, coord>, compare> closedSet;

	//container for storing the parents of items in the open and closed sets (reallocation of the open and closed sets can null pointers to parents, so this keeps a copy that can always be referenced)
	deque<pair<int, coord>> parents;

};

