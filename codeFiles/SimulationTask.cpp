#include "SimulationTask.h"

void SimulationTask::run()
{
    Astar simulatePath;

    //set height and width of grid
    simulatePath.setHeight(individual->getHeight());
    simulatePath.setWidth(individual->getWidth());

    //run simulation on all genes that have failed paths
    for (int j = 0; j < individual->genes.size(); j++)
    {
        if (individual->genes[j].successful)
        {
            for (auto point : individual->genes[j].path)
            {
                simulatePath.pointsVisited.push_back(point);
            }
        }
    }

    for (int j = 0; j < individual->genes.size(); j++)
    {
        if (!individual->genes[j].successful)
        {
            individual->genes[j].path.clear();

            //randomise start and end point
            int chance = rand() % 10;

            if (chance < 5)
            {
                std::swap(individual->genes[j].start, individual->genes[j].end);
            }

            individual->genes[j].path.push_back(individual->genes[j].start);
            individual->genes[j].path.push_back(individual->genes[j].end);

            individual->genes[j].successful = simulationFunction(individual->genes[j], simulatePath);
        }
    }
}

bool SimulationTask::simulationFunction(Chromosome::Gene& individual, Astar& pathing)
{
    vector<Astar::coord> path;
    vector<Astar::coord> pointsVistedSave = pathing.pointsVisited;

    //load gene grid into algorithm
    pathing.setGrid(individual.mappedGene);

    //reset algorithm
    pathing.resetAlgorithm();

    //run algorithm
    if (pathing.astar(individual.start, individual.end))
    {
        //add new points to the path
        for (int j = 0; j < pathing.getPath().size(); j++)
        {
            path.push_back(pathing.getPath()[j]);
            pathing.pointsVisited.push_back(pathing.getPath()[j]);
        }

        individual.path = path;
    }
    else
    {
        //reset points visited
        pathing.pointsVisited = pointsVistedSave;

        //add new points to the path
        for (int j = 0; j < pathing.getPath().size(); j++)
        {
            path.push_back(pathing.getPath()[j]);
        }

        individual.path = path;
    }

    //check if we've reached the overall goal
    if (path[path.size() - 1] == individual.end)
    {
        return true;
    }

    return false;
}