#include "CalculateCostTask.h"

//override Task run function
void CalculateCostTask::run()
{
	int quality = 0;
	int space = 0;
	double failedPathLengths = 0;

	//maximum space we can take up
	int maxSpace = individual->getHeight() * individual->getWidth();

	for (int geneIndex = 0; geneIndex < individual->genes.size(); geneIndex++)
	{
		//if path failed
		if (!individual->genes[geneIndex].successful)
		{
			//"increase" quality
			quality++;

			//even though the path failed, start and end points will still take up space
			space += 2;

			//check how far this path got before it failed
			//inverse the number to ensure worst costs for shorter paths
			double inverse = 1.0 / (double)individual->genes[geneIndex].path.size();
			failedPathLengths += inverse;
				
		}
		else
		{
			//path succeeded
			//increase space by the length of this path
			space += individual->genes[geneIndex].path.size();
		}
	}

	//check the quality of all paths
	quality += pathQuality();

	//calculate the number of empty spaces left
	int spaceDiff = maxSpace - space;

	//calculate sigmoid values
	auto qualitySig = sigmoid(quality);
	auto spSig = sigmoid(spaceDiff);
	auto failedPathLengthsSig = sigmoid(failedPathLengths);

	//set cost
	individual->setCost(qualitySig + spSig + failedPathLengthsSig);
}

double CalculateCostTask::sigmoid(double in)
{
	return 1 / (1 + exp(-(in)));
}

//get the quality score
int CalculateCostTask::pathQuality()
{
	int retVal = 0;

	for (int i = 0; i < individual->genes.size(); i++)
	{
		//if the path succeeded
		if (individual->genes[i].successful)
		{
			//check if the path does any unnecessary turns to take up more space
			for (int j = 0; j < individual->genes[i].path.size(); j++)
			{
				if (j + 3 < individual->genes[i].path.size())
				{
					//check the distance between these
					int distanceToNextPoint = pathing.heuristic(individual->genes[i].path[j], individual->genes[i].path[j + 3]);

					//unnecessary turn has been taken since these points are next to eachother
					if (distanceToNextPoint == 1)
					{
						retVal += 1;
					}
				}
			}
		}
		else
		{
			//check if this path failed because the start and end points were directly blocked off by other paths
			//if the neighbours of the start or end points are all other paths, add to cost

			//get neighbours
			Astar::coord start = individual->genes[i].start;
			Astar::coord end = individual->genes[i].end;

			findNeighbours(start);
			findNeighbours(end);

			//if the neighbours of the start or end points are all other paths, add to cost
			if (!checkClearSpaceAroundPoint(start) || !checkClearSpaceAroundPoint(end))
			{
				retVal++;
			}

		}
	}

	return retVal;
}

//check if neighbours around a point are all clear spaces or all paths
bool CalculateCostTask::checkClearSpaceAroundPoint(Astar::coord& point)
{
	for (auto n : point.neighbours)
	{
		int gindex = 0;

		//if we dont find this neighbour in any of the other genes, this is a clear space
		if (!findInGenePath(*individual, n, gindex))
		{
			return true;
		}
	}

	//all the neighbours are in gene paths
	return false;
}

//find the gene that this point is in
bool CalculateCostTask::findInGenePath(Chromosome& c, Astar::coord& currentPoint, int& geneIndex)
{
	for (int i = 0; i < c.genes.size(); i++)
	{
		if (find(c.genes[i].path.begin(), c.genes[i].path.end(), currentPoint) != c.genes[i].path.end())
		{
			geneIndex = i;
			return true;
		}
	}

	//point is not in any gene paths
	return false;
}

//find neighbours of a point
void CalculateCostTask::findNeighbours(Astar::coord& point)
{
	//check each direction
	//if its not an obstacle add to neighbours

	Astar::coord testCoord = point;

	//north
	testCoord.y = point.y + 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		point.neighbours.push_back(testCoord);
	}

	//south
	testCoord.y = point.y - 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		point.neighbours.push_back(testCoord);
	}

	//west
	testCoord.y = point.y;
	testCoord.x = point.x - 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		point.neighbours.push_back(testCoord);
	}

	//east
	testCoord.x = point.x + 1;

	//coordinate is not out of bounds
	if (validCoordinates(testCoord))
	{
		point.neighbours.push_back(testCoord);
	}
}

//check if a point goes out of bounds
bool CalculateCostTask::validCoordinates(Astar::coord& point)
{
	if (point.x > -1 && point.x < individual->getWidth() && point.y > -1 && point.y < individual->getHeight())
	{
		return true;
	}

	return false;
}