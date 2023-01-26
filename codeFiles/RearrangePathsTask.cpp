#include "RearrangePathsTask.h"

void RearrangePathsTask::run()
{
	vector<Astar::coord> pointsSeen;

	//for all genes
	for (int geneIndex = 0; geneIndex < individual->genes.size(); geneIndex++)
	{
		if (!individual->genes[geneIndex].successful)
		{
			continue;
		}

		//iterate through the path taken by this gene, check if the path overlaps with any paths we've already looked at
		for (int j = 0; j < individual->genes[geneIndex].path.size(); j++)
		{
			//check if we have seen this point before
			if (find(pointsSeen.begin(), pointsSeen.end(), individual->genes[geneIndex].path[j]) != pointsSeen.end())
			{
				//set this path to false
				individual->genes[geneIndex].successful = false;
				break;
			}
			else
			{
				individual->genes[geneIndex].successful = true;
				pointsSeen.push_back(individual->genes[geneIndex].path[j]);
			}
		}
	}

	//order genes by path length
	std::sort(individual->genes.begin(), individual->genes.end());
}