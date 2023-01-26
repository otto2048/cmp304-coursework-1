#pragma once
#include "TaskGA.h"
#include "Astar.h"

class CalculateCostTask : public TaskGA
{
public:
	using TaskGA::TaskGA;

	//override Task run function
	void run();

	//best possible solution cost due to sigmoid function
	static constexpr double BEST_SOLUTION_COST = 1.5;
private:
	Astar pathing;

	int pathQuality();
	void findNeighbours(Astar::coord&);
	bool validCoordinates(Astar::coord&);
	bool checkClearSpaceAroundPoint(Astar::coord&);
	bool findInGenePath(Chromosome& c, Astar::coord& currentPoint, int& geneIndex);
	double sigmoid(double);
};