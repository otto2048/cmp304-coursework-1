#pragma once
#include "Astar.h"
#include "Chromosome.h"
#include <map>

#include <cstdlib> // random numbers header file//
#include <ctime> // used to get date and time information

#include "Output.h"
#include "FileIO.h"
#include "Farm.h"
#include "SimulationTask.h"
#include "CalculateCostTask.h"
#include "RearrangePathsTask.h"

#include <conio.h>
#include <sstream>

#include <iomanip>

#include <fstream>
using std::ofstream;

using std::map;

class FlowGA
{
public:
	//minimum: stop when solutions all converge on an identical solution || maximum num of generations reached
	//maximum: stop when solutions all converge on the best solution || maximum num of generations reached
	//maxGen: stop when the maximum amount of generations is hit
	enum class stoppingCondition { minimum = 0, maximum, maxGen };
private:

	Chromosome baseChromosome;

	Chromosome crossoverFunction(const Chromosome& X1, const Chromosome& X2);
	Chromosome mutationFunction(const Chromosome& X_base);

	//Driving genetic algorithm:
	void generateInitPopulation();
	void calcGenerationCost();
	void crossoverAndMutation();
	void simulatePopulation();
	void rankPopulation();
	bool manageConvergence();

	bool outputGenerationInfo(bool);

	map<int, Chromosome>::iterator selectParent();

	//checking convergence based on stoppingCondition
	bool checkConvergence();
	bool checkSolutionConvergence();

	Chromosome getBestChromosome(int&);

	//hold all the individuals of the current generation
	vector<Chromosome> generation;

	//hold the current rankings of the generation
	map<int, Chromosome> rankings;

	//genetic algorithm settings
	int generationId = 0;
	int maxGenerations = 1000;
	int mutationRate = 40;
	int populationSize = 100;
	bool displayGeneration = true;
	bool displayGenes = true;
	bool drawLevel = true;
	stoppingCondition stopMode = stoppingCondition::maxGen;

	int cumalative = 0;

	FileIO fileIO;
	Astar pathAlgorithm;
public:
	FlowGA();
	~FlowGA();

	//run the algorithm
	void runGA();
	void runGA(ofstream&);

	//set up the algorithm with a certain level
	bool setUp(string);

	//getters and setters
	int getGenerationId();
	int getMaxGen();
	int getMutationRate();
	int getPopulationSize();
	bool getDisplayGeneration();
	bool getDisplayGenes();
	stoppingCondition getStopMode();
	bool getDrawLevel();
	double getAverageCost();
	double getBestCost();

	void setMutationRate(int);
	void setMaxGen(int);
	void setPopulationSize(int);
	void setDisplayGeneration(bool);
	void setDisplayGenes(bool);
	void setStopMode(stoppingCondition);
	void setDrawLevel(bool);

	Chromosome getBase();

	void reset();
};

