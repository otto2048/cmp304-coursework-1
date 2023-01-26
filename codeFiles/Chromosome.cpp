#include "Chromosome.h"

//overloading < operator for Gene struct
bool Chromosome::Gene::operator<(const Gene& other) const
{
	if (path.size() < other.path.size())
	{
		return true;
	}
	return false;
}

//overloading != operator for Chromosome
bool Chromosome::operator!=(const Chromosome& other)
{
	//check paths of all genes
	int equalPaths = 0;
	for (int i = 0; i < this->genes.size(); i++)
	{
		int otherIndex = findGeneInAnotherChromosome(other, genes[i]);
		//check the order of the gene paths
		if (other.genes[otherIndex].path[0] != this->genes[i].path[0])
		{
			auto oCopy = other;

			std::reverse(oCopy.genes[otherIndex].path.begin(), oCopy.genes[otherIndex].path.end());

			//compare paths
			if (this->genes[i].path == oCopy.genes[otherIndex].path)
			{
				equalPaths++;
			}
		}
		else
		{
			//compare paths
			if (this->genes[i].path == other.genes[otherIndex].path)
			{
				equalPaths++;
			}
		}
	}

	if (equalPaths == this->genes.size())
	{
		return false;
	}

	return true;
}

//overloading == operator for Chromosome
bool Chromosome::operator==(const Chromosome& other) const
{
	//check paths of all genes
	int equalPaths = 0;
	for (int i = 0; i < this->genes.size(); i++)
	{
		int otherIndex = findGeneInAnotherChromosome(other, genes[i]);

		//check the order of the gene paths
		if (other.genes[otherIndex].path[0] != this->genes[i].path[0])
		{
			auto oCopy = other;

			std::reverse(oCopy.genes[otherIndex].path.begin(), oCopy.genes[otherIndex].path.end());

			//compare paths
			if (this->genes[i].path == oCopy.genes[otherIndex].path)
			{
				equalPaths++;
			}
		}
		else
		{
			//compare paths
			if (this->genes[i].path == other.genes[otherIndex].path)
			{
				equalPaths++;
			}
		}
	}

	if (equalPaths == this->genes.size())
	{
		return true;
	}

	return false;
}

//used to find matching genes when gene order has changed
int Chromosome::findGeneInAnotherChromosome(const Chromosome& find, const Gene& g) const
{
	//for all the genes in find
	for (int i = 0; i < find.genes.size(); i++)
	{
		//check if this gene matches the gene passed to this function
		if (find.genes[i].name == g.name)
		{
			return i;
		}
	}

	//shouldnt get here
	return 0;
}

//overloading < operator for chromosome, order by cost
bool Chromosome::operator<(const Chromosome& other) const
{
	if (cost < other.cost)
	{
		return true;
	}
	return false;
}

int Chromosome::getHeight() const
{
	return height;
}

int Chromosome::getWidth() const
{
	return width;
}

double Chromosome::getCost() const
{
	return cost;
}

void Chromosome::setHeight(int h)
{
	height = h;
}

void Chromosome::setWidth(int w)
{
	width = w;
}

void Chromosome::setCost(double c)
{
	cost = c;
}