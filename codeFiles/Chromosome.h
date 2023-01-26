#pragma once
#include "Astar.h"

using std::multiset;

class Chromosome
{
public:

	//holds the information about each path in the solution
	struct Gene
	{
		vector<Astar::coord> path;
		vector<vector<int>> mappedGene;
		string name = "undefined";
		Astar::coord start;
		Astar::coord end;
		bool successful = false;

		bool operator<(const Gene&) const;
	};

	typedef multiset<Gene>::iterator geneIt;
	typedef multiset<Gene>::reverse_iterator rGeneIt;

	vector<Gene> genes;
	bool operator!=(const Chromosome&);
	bool operator==(const Chromosome&) const;
	bool operator<(const Chromosome&) const;

	int getHeight() const;
	int getWidth() const;
	double getCost() const;

	void setHeight(int);
	void setWidth(int);
	void setCost(double);

	int findGeneInAnotherChromosome(const Chromosome&, const Gene&) const;

private:
	int height;
	int width;
	double cost = 0;
};