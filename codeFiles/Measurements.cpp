#include "Measurements.h"

Measurements::Measurements(string s)
{
	saveFile = s;
}

Measurements::Measurements(string s, int c)
{
	saveFile = s;
	solveCount = c;
}

bool Measurements::runMeasurements(FlowGA& flow, bool measurementOption)
{
	ofstream file(saveFile + ".csv");

	if (file.is_open())
	{
		//write current settings to file
		file << "Genetic algorithm settings:" << endl;

		file << "Population:," << flow.getPopulationSize() << endl;
		file << "Mutation rate:," << flow.getMutationRate() << endl;
		file << "Max num of generations:," << flow.getMaxGen() << endl;

		file << "Stop condition:,";
		switch (flow.getStopMode())
		{
		case FlowGA::stoppingCondition::maxGen:
			file << "Reach maximum generation" << endl;
			break;
		case FlowGA::stoppingCondition::minimum:
			file << "Minimum Convergence" << endl;
			break;
		case FlowGA::stoppingCondition::maximum:
			file << "Maximum convergence" << endl;
			break;
		}

		file << endl;

		if (measurementOption)
		{
			//create [generation vs cost] data
			file << "Generation ID, Best cost, Average cost" << endl;
			flow.runGA(file);

			//reset GA
			flow.reset();
		}
		else
		{
			//create [generations to solve/time to solve] data
			file << "Time, Generations, Average cost, Best cost" << endl;

			for (int i = 0; i < solveCount; i++)
			{
				//start timing
				theClock::time_point start = theClock::now();

				//run GA
				flow.runGA();

				//stop timing
				theClock::time_point end = theClock::now();

				//compute the difference between the two times in milliseconds
				auto timeTaken = duration_cast<milliseconds>(end - start).count();
				std::cout << std::fixed;
				std::cout << std::setprecision(5);
				file << timeTaken << "," << flow.getGenerationId() << "," << flow.getAverageCost() << "," << flow.getBestCost() << endl;

				//reset GA
				flow.reset();
			}
		}

		file.close();
		return true;
	}

	return false;
}