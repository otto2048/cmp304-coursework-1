#pragma once
#include <windows.h> //for colours
#include <map>
#include <string>

#include <iostream>

#include "Chromosome.h"
#include "FlowGA.h"

using std::map;
using std::string;
using std::cout;
using std::endl;

//forward declaration of FlowGA
class FlowGA;

class Output
{
private:
	map<string, int> colours;

	string findInGenePath(Chromosome&, Astar::coord&);
	bool findGeneStartEnd(Chromosome&, Astar::coord&, string&);

	void drawGene(Chromosome::Gene&, int, int, int, int);

public:
	Output();
	void drawChromosome(Chromosome&, int, int, string, double cost=-1);
	void drawChromosomeGenes(Chromosome&, int, int);

	void outputTitle();
	void outputMainMenu();
	void outputStartMenu();
	void outputGASettings(FlowGA&);
	void outputEditGASettings();
	void outputEditGADisplaySettings();
	void outputEditGAParamterSettings();
	void outputMeasurementOptions();

	//get X position of cursor
	int getCursorX();

	//get Y position of cursor
	int getCursorY();

	void outputSelectionError();
	void outputExitingProgram();

	void outputAllColours();
};

