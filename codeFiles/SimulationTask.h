#pragma once
#include "TaskGA.h"
#include "Astar.h"

class SimulationTask : public TaskGA
{
public:
	using TaskGA::TaskGA;

	//override Task run function
	void run();
	bool simulationFunction(Chromosome::Gene& individual, Astar& pathing);
};

