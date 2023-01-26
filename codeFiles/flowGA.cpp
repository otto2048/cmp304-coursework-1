#include "FlowGA.h"

FlowGA::FlowGA()
{
	//read settings file
	vector<string> settings = fileIO.readOptionalFile("settings.txt");

	if (!settings.empty())
	{
		maxGenerations = stoi(settings[0]);
		mutationRate = stoi(settings[1]);
		populationSize = stoi(settings[2]);
		std::istringstream(settings[3]) >> drawLevel;
		std::istringstream(settings[4]) >> displayGeneration;
		std::istringstream(settings[5]) >> displayGenes;
		stopMode = stoppingCondition(stoi(settings[6]));
	}
}

FlowGA::~FlowGA()
{
	//save settings to settings file
	vector<string> settings;
	settings.push_back(std::to_string(maxGenerations));
	settings.push_back(std::to_string(mutationRate));
	settings.push_back(std::to_string(populationSize));
	settings.push_back(std::to_string(drawLevel));
	settings.push_back(std::to_string(displayGeneration));
	settings.push_back(std::to_string(displayGenes));
	settings.push_back(std::to_string((int)stopMode));

	fileIO.saveToFile(settings, "settings.txt");
}

//set up the algorithm with a certain level
bool FlowGA::setUp(string filePath)
{
	//clear the base chromosome if it already has genes in it
	if (!baseChromosome.genes.empty())
	{
		baseChromosome.genes.clear();
	}

	//get all paths
	vector<string> baseFile = fileIO.readFile(filePath + ".txt");

	if (baseFile.empty())
	{
		return false;
	}

	//move all characters into a single string
	string paths = "";
	for (int y = 0; y < baseFile.size(); y++)
	{
		for (int x = 0; x < baseFile[y].size(); x++)
		{
			paths += baseFile[y][x];
		}
	}

	//remove duplicates from flattened map
	sort(paths.begin(), paths.end());
	auto last = std::unique(paths.begin(), paths.end());
	paths.erase(last, paths.end());

	//remove empty spaces
	paths.erase(remove(paths.begin(), paths.end(), '-'));

	baseChromosome.setHeight(baseFile.size());
	baseChromosome.setWidth(baseFile[0].size());

	for (auto path : paths)
	{
		Chromosome::Gene gene;
		gene.name = path;

		//get map from map file
		gene.mappedGene = fileIO.readMapFile(filePath + ".txt", path);

		//get start and end points
		//find points in map that are -3
		pair<Astar::coord, Astar::coord> points;

		for (int y = 0; y < gene.mappedGene.size(); y++)
		{
			for (int x = 0; x < gene.mappedGene[y].size(); x++)
			{
				if (gene.mappedGene[y][x] == -3)
				{
					//if we havent set the first point yet
					if (points.first.value == 0)
					{
						points.first.x = x;
						points.first.y = y;
						points.first.value = -3;
					}
					else
					{
						points.second.x = x;
						points.second.y = y;
					}
				}
			}
		}

		//set start and end
		gene.start = points.first;
		gene.end = points.second;

		//add start and end to path
		gene.path.push_back(gene.start);
		gene.path.push_back(gene.end);

		gene.successful = false;

		baseChromosome.genes.push_back(gene);
	}

}

Chromosome FlowGA::mutationFunction(const Chromosome& c)
{
	//calculate the cost of the chromosome before mutation
	Chromosome current;
	current = c;

	CalculateCostTask calc(&current);
	calc.run();

	if (current.getCost() == CalculateCostTask::BEST_SOLUTION_COST)
	{
		return c;
	}

	Chromosome mutated = c;

	//mutate random amount of genes
	int genesToMutate = rand() % mutated.genes.size() + 1;

	//get random gene indexs to mutate
	set<int> geneIndexToMutate;

	while (geneIndexToMutate.size() < genesToMutate)
	{
		int gene = rand() % mutated.genes.size();
		geneIndexToMutate.insert(gene);
	}

	//set the paths of the genes to be mutated paths to unsuccessful
	auto it = geneIndexToMutate.begin();
	while (it != geneIndexToMutate.end())
	{
		mutated.genes[*it].successful = false;
		it++;
	}

	//run simulation for this chromosome
	SimulationTask sim(&mutated);
	sim.run();

	Chromosome test;
	test = mutated;

	CalculateCostTask calcTest(&test);
	calcTest.run();

	//if the mutated chromosome is better than the original, keep the mutation
	if (test.getCost() < current.getCost())
	{
		return mutated;
	}

	return c;
}

Chromosome FlowGA::crossoverFunction(const Chromosome& X1, const Chromosome& X2)
{
	Chromosome child = X1;
	child.genes.clear();

	//for each gene in a chromosome
	for (int i = 0; i < X1.genes.size(); i++)
	{
		int geneIndex = child.findGeneInAnotherChromosome(X2, X1.genes[i]);

		int chance = rand() % 10;

		//push back either of the parents gene
		if (chance < 5)
		{
			child.genes.push_back(X1.genes[i]);
		}
		else
		{
			child.genes.push_back(X2.genes[geneIndex]);
		}
	}

	return child;
}

void FlowGA::runGA()
{
	//generate inital population and calculate costs
	cout << "Generating initial population..." << endl;
	generateInitPopulation();

	bool cancel = false;

	cout << "Running algorithm..." << endl;
	cout << "Press E to stop algorithm running" << endl;
	while (!manageConvergence())
	{
		if (drawLevel)
		{
			//Output generation information
			cancel = outputGenerationInfo(false);

			if (!cancel)
			{
				cout << "Algorithm stopped" << endl;
				cout << endl;
				return;
			}
		}
		else
		{
			if (_kbhit())
			{
				if (toupper(_getch()) == 'E')
				{
					cout << "Algorithm stopped" << endl;
					cout << endl;
					return;
				}
			}
		}

		//perform selection
		rankPopulation();

		//perform crossover and mutation
		crossoverAndMutation();

		//rearrange paths of new generation before simulation
		//create tasks
		Farm farm;
		for (int i = 0; i < generation.size(); i++)
		{
			farm.addTask(new RearrangePathsTask(&generation[i]));
		}

		//run farm
		farm.run();

		//simulation new population
		simulatePopulation();

		//compute fitness
		calcGenerationCost();

		generationId++;
	}

	//Output generation information
	outputGenerationInfo(true);
}

void FlowGA::runGA(ofstream& saveResultsFile)
{
	//generate inital population and calculate costs
	cout << "Generating initial population..." << endl;
	generateInitPopulation();

	bool cancel = false;

	cout << "Running algorithm..." << endl;
	cout << "Press E to stop algorithm running" << endl;
	while (!manageConvergence())
	{
		int temp = 0;
		saveResultsFile << generationId << "," << getBestChromosome(temp).getCost() << "," << getAverageCost() << endl;

		if (drawLevel)
		{
			//Output generation information
			cancel = outputGenerationInfo(false);

			if (!cancel)
			{
				cout << "Algorithm stopped" << endl;
				cout << endl;
				return;
			}
		}
		else
		{
			if (_kbhit())
			{
				if (toupper(_getch()) == 'E')
				{
					cout << "Algorithm stopped" << endl;
					cout << endl;
					return;
				}
			}
		}

		//perform selection
		rankPopulation();

		//perform crossover and mutation
		crossoverAndMutation();

		//rearrange paths of new generation before simulation
		//create tasks
		Farm farm;
		for (int i = 0; i < generation.size(); i++)
		{
			farm.addTask(new RearrangePathsTask(&generation[i]));
		}

		farm.run();

		//simulation new population
		simulatePopulation();

		//compute fitness
		calcGenerationCost();

		generationId++;
	}

	//Output generation information
	outputGenerationInfo(true);

	int temp = 0;

	std::cout << std::fixed;
	std::cout << std::setprecision(5);
	saveResultsFile << generationId << "," << getBestChromosome(temp).getCost() << "," << getAverageCost() << endl;
}

bool FlowGA::manageConvergence()
{
	if (generationId >= maxGenerations)
	{
		return true;
	}

	switch (stopMode)
	{
		case stoppingCondition::maxGen:
			return false;
		case stoppingCondition::minimum:
			return checkConvergence();
		case stoppingCondition::maximum:
			return checkSolutionConvergence();
	}
}

bool FlowGA::outputGenerationInfo(bool solutionFound)
{
	Output output;

	//get best chromosome
	int bestCID = 0;
	Chromosome bestChromosome = getBestChromosome(bestCID);

	system("cls");
	
	output.outputTitle();

	cout
		<< "Generation: " << generationId << endl;

	cout << "Flows: " << bestChromosome.genes.size() << endl;

	std::cout << std::fixed;
	std::cout << std::setprecision(5);
	cout << "Best cost: " << bestChromosome.getCost() << endl;
	cout << "Average cost: " << getAverageCost() << endl;
	cout << endl;

	output.drawChromosome(bestChromosome, output.getCursorX(), output.getCursorY(), "Best chromosome:", bestChromosome.getCost());
	cout << endl;

	if (displayGenes)
	{
		//display the genes of the best chromosome
		cout << endl;
		cout << endl;
		cout << "Best chromosome genes: " << endl;
		cout << endl;
		output.drawChromosomeGenes(bestChromosome, output.getCursorX(), output.getCursorY());
		cout << "Press any key to continue or press E to stop algorithm running" << endl;
		if (toupper(_getch()) == 'E')
		{
			return false;
		}
	}

	if (displayGeneration)
	{
		//display whole generation
		cout << endl;
		cout << endl;
		cout << "Last generation: " << endl;
		int yPos = output.getCursorY();
		int xPos = 0;
		for (int i = 0; i < generation.size(); i++)
		{
			if (i % 3 == 0)
			{
				int counter = 0;
				for (int j = i; j < i + 3; j++)
				{
					if (counter == 1)
					{
						xPos += generation[i].getWidth() * 3 + 10;
					}
					else if (counter == 2)
					{
						xPos += generation[i].getWidth() * 4 + 2;
					}
					else
					{
						xPos = 0;

						if (j != 0)
						{
							yPos += generation[i].getHeight() + 5;
						}
					}

					if (j < generation.size())
					{
						output.drawChromosome(generation[j], xPos, yPos, "", generation[j].getCost());
					}
					counter++;
				}
			}
		}
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "Press any key to continue or press E to stop algorithm running" << endl;
		if (toupper(_getch()) == 'E')
		{
			return false;
		}
	}

	if (!solutionFound)
	{
		cout << "Press E to stop algorithm running" << endl;
		if (_kbhit())
		{
			if (toupper(_getch()) == 'E')
			{
				return false;
			}
		}
	}

	return true;
}

Chromosome FlowGA::getBestChromosome(int& bestIndex)
{
	Chromosome ret = generation[0];
	bestIndex = 0;

	for (int i = 0; i < populationSize; i++)
	{
		if (generation[i].getCost() < ret.getCost())
		{
			ret = generation[i];
			bestIndex = i;
		}
	}

	return ret;
}

double FlowGA::getAverageCost()
{
	double totalCost = 0;

	for (auto const& members : generation)
	{
		totalCost += members.getCost();
	}

	return (double)totalCost / (double)populationSize;
}

//rank the population
void FlowGA::rankPopulation()
{
	//sort the population by cost
	sort(generation.begin(), generation.end());

	//rank the population
	for (int i = 0; i < populationSize; i++)
	{
		rankings[populationSize - i] = generation[i];
	}

	//add up all the rankings
	cumalative = 0;
	for (auto const& rank:rankings)
	{
		cumalative += rank.first;
	}
}

//run simulation on all members of the population
void FlowGA::simulatePopulation()
{
	//create tasks
	Farm farm;
	for (int i = 0; i < generation.size(); i++)
	{
		farm.addTask(new SimulationTask(&generation[i]));
	}
	//for all members of the population, run the simulation on them
	//run farm
	farm.run();
}

//check if all the members of the population are identical
bool FlowGA::checkConvergence()
{
	for (int i = 0; i < populationSize; i++)
	{
		if (generation[0] != generation[i])
		{
			return false;
		}
	}

	return true;
}

//check if all the members of the population have reached the best solution
bool FlowGA::checkSolutionConvergence()
{
	for (int i = 0; i < populationSize; i++)
	{
		if (generation[i].getCost() != CalculateCostTask::BEST_SOLUTION_COST)
		{
			return false;
		}
	}

	return true;
}

//generate the initial population
void FlowGA::generateInitPopulation()
{
	do
	{
		generation.clear();

		for (int i = 0; i < populationSize; i++)
		{
			generation.push_back(baseChromosome);
		}

		//simulate population
		simulatePopulation();

		//calculate fitness of initial generation
		calcGenerationCost();

	} while (checkConvergence());
}

//calculate the fitness of each member of the population
void FlowGA::calcGenerationCost()
{
	//create tasks
	Farm farm;
	for (int i = 0; i < generation.size(); i++)
	{
		farm.addTask(new CalculateCostTask(&generation[i]));
	}

	//for all members of the population, calculate their cost
	//run farm
	farm.run();
}

//perform crossover and mutation to generate a new population
void FlowGA::crossoverAndMutation()
{
	vector<Chromosome> newGeneration;

	int childrenCreated = 0;
	while (childrenCreated < populationSize)
	{
		//select parents to crossover
		map<int, Chromosome>::iterator p1 = selectParent();
		map<int, Chromosome>::iterator p2 = selectParent();

		//check selected parents aren't the same chromosome
		if (p1 == p2)
		{
			continue;
		}

		//crossover the chromosomes
		Chromosome child = crossoverFunction(p1->second, p2->second);

		//generate chance to perform mutation
		int mutation = rand() % 100;

		if (mutation < mutationRate)
		{
			child = mutationFunction(child);
		}

		//add child to the new generation
		newGeneration.push_back(child);
		childrenCreated++;
	}

	generation.clear();
	rankings.clear();
	generation = newGeneration;
}

//use rank selection to select a parent
map<int, Chromosome>::iterator FlowGA::selectParent()
{
	//generate random number
	int randomNum = rand() % cumalative;

	int accumalate = 0;
	map<int, Chromosome>::iterator it = rankings.begin();

	//accumulate rankings until we reach the random number to find the chromosome
	while (accumalate <= randomNum)
	{
		accumalate += it->first;
		it++;
	}
	
	it--;

	return it;
}

int FlowGA::getMutationRate()
{
	return mutationRate;
}

int FlowGA::getMaxGen()
{
	return maxGenerations;
}

int FlowGA::getPopulationSize()
{
	return populationSize;
}

bool FlowGA::getDisplayGeneration()
{
	return displayGeneration;
}

bool FlowGA::getDisplayGenes()
{
	return displayGenes;
}

FlowGA::stoppingCondition FlowGA::getStopMode()
{
	return stopMode;
}

bool FlowGA::getDrawLevel()
{
	return drawLevel;
}

int FlowGA::getGenerationId()
{
	return generationId;
}

double FlowGA::getBestCost()
{
	int b = 0;
	return getBestChromosome(b).getCost();
}

void FlowGA::setMutationRate(int input)
{
	mutationRate = input;
}

void FlowGA::setMaxGen(int input)
{
	maxGenerations = input;
}

void FlowGA::setPopulationSize(int input)
{
	populationSize = input;
}

void FlowGA::setDisplayGeneration(bool input)
{
	displayGeneration = input;
}

void FlowGA::setDisplayGenes(bool input)
{
	displayGenes = input;
}

void FlowGA::setStopMode(stoppingCondition input)
{
	stopMode = input;
}

void FlowGA::setDrawLevel(bool input)
{
	drawLevel = input;
}

Chromosome FlowGA::getBase()
{
	return baseChromosome;
}

void FlowGA::reset()
{
	generation.clear();
	rankings.clear();
	generationId = 0;
}