#include "Output.h"

Output::Output()
{
	//set up colours map
	colours["a"] = 31;
	colours["b"] = 143;
	colours["c"] = 47;
	colours["d"] = 240;
	colours["e"] = 159;
	colours["f"] = 63;
	colours["g"] = 160;
	colours["h"] = 79;
	colours["i"] = 176;
	colours["j"] = 95;
	colours["k"] = 207;
	colours["l"] = 111;
	colours["m"] = 223;
	colours["n"] = 112;
	colours["o"] = 224;

	colours["white"] = 7;
}

void Output::outputAllColours()
{
	map<string, int>::iterator it = colours.begin();
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	while (it != colours.end())
	{
		SetConsoleTextAttribute(hstdout, (*it).second);
		cout << (*it).first << endl;

		it++;
	}
}

void Output::drawChromosome(Chromosome& c, int xPos, int yPos, string title, double cost)
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD startPos = { xPos, yPos };
	char endLine[] = { '\n' };
	::SetConsoleCursorPosition(hstdout, startPos);

	SetConsoleTextAttribute(hstdout, colours["white"]);

	::WriteConsoleA(hstdout, title.c_str(), title.size(), NULL, NULL);
	::WriteConsoleA(hstdout, endLine, 1, NULL, NULL);
	yPos += 2;
	startPos.Y = yPos;
	::SetConsoleCursorPosition(hstdout, startPos);

	string startSpace = "   |";
	::WriteConsoleA(hstdout, startSpace.c_str(), startSpace.size(), NULL, NULL);

	for (int i = 0; i < c.getWidth(); i++)
	{
		string num = "";
		if (i < 10)
		{
			num += "0";
		}
		num += std::to_string(i);
		num += "|";
		::WriteConsoleA(hstdout, num.c_str(), num.length(), NULL, NULL);
	}
	::WriteConsoleA(hstdout, endLine, 1, NULL, NULL);
	yPos++;
	for (int i = 0; i < c.getHeight(); i++)
	{
		COORD pos = { xPos, yPos++ };
		::SetConsoleCursorPosition(hstdout, pos);

		string num = "";
		if (i < 10)
		{
			num += "0";
		}
		num += std::to_string(i);
		num += " |";
		::WriteConsoleA(hstdout, num.c_str(), num.length(), NULL, NULL);

		for (int j = 0; j < c.getWidth(); j++)
		{
			Astar::coord currentCoord(j, i);

			//check if the current point is in any of the paths in the chromosomes genes
			string colour = findInGenePath(c, currentCoord);

			//check if the current point is the start/end of any of the chromosome genes
			bool isStartOrEnd = findGeneStartEnd(c, currentCoord, colour);

			if (colour != "")
			{
				SetConsoleTextAttribute(hstdout, colours[colour]);
			}
			else
			{
				SetConsoleTextAttribute(hstdout, colours["white"]);
			}

			if (isStartOrEnd)
			{
				string nameOutput = "(" + colour + ")";
				::WriteConsoleA(hstdout, nameOutput.c_str(), nameOutput.length(), NULL, NULL);
			}
			else
			{
				char buff4[] = "   ";
				::WriteConsoleA(hstdout, buff4, 4, NULL, NULL);
			}

			if (j == (c.getWidth() - 1))
			{
				SetConsoleTextAttribute(hstdout, colours["white"]);
				char buff5[] = "|";
				::WriteConsoleA(hstdout, buff5, 2, NULL, NULL);
			}
		}
	}


	if (cost != -1)
	{
		COORD pos = { xPos,  yPos += 2 };
		::SetConsoleCursorPosition(hstdout, pos);

		std::cout << std::fixed;
		std::cout << std::setprecision(5);
		cout << "Cost: " << cost;
	}
	
}

void Output::drawChromosomeGenes(Chromosome& c, int xPos, int yPos)
{
	cout << endl;
	for (int i = 0; i < c.genes.size(); i++)
	{
		if (i % 3 == 0)
		{
			int counter = 0;
			for (int j = i; j < i + 3; j++)
			{
				if (counter == 1)
				{
					xPos += c.getWidth() * 3 + 10;
				}
				else if (counter == 2)
				{
					xPos += c.getWidth() * 4 + 2;
				}
				else
				{
					xPos = 0;
					
					if (j != 0)
					{
						yPos += c.getHeight() + 5;
					}
				}

				if (j < c.genes.size())
				{
					drawGene(c.genes[j], xPos, yPos, c.getHeight(), c.getWidth());
				}
				counter++;
			}
		}
	}
	cout << endl;
}

void Output::drawGene(Chromosome::Gene& g, int xPos, int yPos, int height, int width)
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD startPos = { xPos, yPos };
	::SetConsoleCursorPosition(hstdout, startPos);
	char endLine[] = { '\n' };

	SetConsoleTextAttribute(hstdout, colours["white"]);

	string startSpace = "   |";
	::WriteConsoleA(hstdout, startSpace.c_str(), startSpace.size(), NULL, NULL);

	for (int i = 0; i < width; i++)
	{
		string num = "";
		if (i < 10)
		{
			num += "0";
		}
		num += std::to_string(i);
		num += "|";
		::WriteConsoleA(hstdout, num.c_str(), num.length(), NULL, NULL);
	}
	::WriteConsoleA(hstdout, endLine, 1, NULL, NULL);
	yPos++;
	for (int i = 0; i < height; i++)
	{
		COORD pos = { xPos, yPos++ };
		::SetConsoleCursorPosition(hstdout, pos);
		string num = "";
		if (i < 10)
		{
			num += "0";
		}
		num += std::to_string(i);
		num += " |";
		::WriteConsoleA(hstdout, num.c_str(), num.length(), NULL, NULL);

		for (int j = 0; j < width; j++)
		{
			Astar::coord currentCoord(j, i);

			//check if the current point is the genes path
			string colour = "";

			if (find(g.path.begin(), g.path.end(), currentCoord) != g.path.end())
			{
				colour = g.name;
			}

			if (colour != "")
			{
				SetConsoleTextAttribute(hstdout, colours[colour]);
			}
			else
			{
				SetConsoleTextAttribute(hstdout, colours["white"]);
			}

			if (currentCoord == g.start || currentCoord == g.end)
			{
				char buff4[] = " O ";
				::WriteConsoleA(hstdout, buff4, 4, NULL, NULL);
			}
			else
			{
				char buff4[] = "   ";
				::WriteConsoleA(hstdout, buff4, 4, NULL, NULL);
			}

			if (j == (width - 1))
			{
				SetConsoleTextAttribute(hstdout, colours["white"]);
				char buff5[] = "|";
				::WriteConsoleA(hstdout, buff5, 2, NULL, NULL);
			}
		}
	}
	
	COORD pos = { xPos, yPos+=2 };
	::SetConsoleCursorPosition(hstdout, pos);
	if (g.successful)
	{
		string pathMessage = "Successfully found path ";
		::WriteConsoleA(hstdout, pathMessage.c_str(), pathMessage.size(), NULL, NULL);
	}
	else
	{
		string pathMessage = "Failed to find path ";
		::WriteConsoleA(hstdout, pathMessage.c_str(), pathMessage.size(), NULL, NULL);
	}
}

string Output::findInGenePath(Chromosome& c, Astar::coord& currentPoint)
{
	for (auto& const g : c.genes)
	{
		if (!g.successful && g.start != currentPoint && g.end != currentPoint)
		{
			continue;
		}

		if (find(g.path.begin(), g.path.end(), currentPoint) != g.path.end())
		{
			return g.name;
		}
	}

	return "";
}

bool Output::findGeneStartEnd(Chromosome& c, Astar::coord& currentPoint, string& name)
{
	for (auto& const g : c.genes)
	{
		if (g.start == currentPoint || g.end == currentPoint)
		{
			name = g.name;
			return true;
		}
	}

	return false;
}

void Output::outputTitle()
{
	cout << endl;
	cout << "**Flow Genetic Algorithm**" << endl;
	cout << endl;
}

void Output::outputMainMenu()
{
	cout << "--Main Menu--" << endl;
	cout << endl;
	cout << "Enter:" << endl;
	cout << endl;
	cout << " E to Enter the location of a level file" << endl;
	cout << " D to use the Default level" << endl;
	cout << endl;
	cout << " Or X to Exit" << endl;
}

void Output::outputGASettings(FlowGA& flow)
{
	cout << "--Genetic Algorithm Settings--" << endl;
	cout << endl;
	cout << "	--Genetic Algorithm Parameters--" << endl;
	cout << endl;
	cout << "		Population: " << flow.getPopulationSize() << endl;
	cout << "		Mutation Rate: " << flow.getMutationRate() << "%" << endl;
	cout << "		Maximum number of generations: " << flow.getMaxGen() << endl;
	
	switch (flow.getStopMode())
	{
	case FlowGA::stoppingCondition::maxGen:
		cout << "		Stop condition: reach maximum generation " << endl
			<< "			-run until the maximum number of generations run" << endl;
		break;
	case FlowGA::stoppingCondition::minimum:
		cout << "		Stop condition: Minimum Convergence" << endl
			<< "			-run until chromosomes converge on identical solution OR maximum number of generations run" << endl;
		break;
	case FlowGA::stoppingCondition::maximum:
		cout << "		Stop condition: Maximum convergence" << endl
			<< "			-run until chromosomes converge on the best solution OR maximum number of generations run" << endl;
		break;
	}

	cout << endl;
	cout << "	--Genetic Algorithm Display Settings--" << endl << endl;
	if (flow.getDrawLevel())
	{
		cout << "		Draw level whilst running algorithm: on" << endl;
	}
	else
	{
		cout << "		Draw level whilst running algorithm: off" << endl;
	}
	if (flow.getDisplayGeneration())
	{
		cout << "		Display whole generation: on" << endl;
	}
	else
	{
		cout << "		Display whole generation: off" << endl;
	}
	if (flow.getDisplayGenes())
	{
		cout << "		Display the genes of the best chromosome: on" << endl;
	}
	else
	{
		cout << "		Display the genes of the best chromosome: off" << endl;
	}
	cout << endl;
}

void Output::outputMeasurementOptions()
{
	cout << "--Select measurement option--" << endl;
	cout << endl;
	cout << "Enter: " << endl;
	cout << " 1 to measure [generation vs cost] of a single run of the algorithm" << endl;
	cout << " 2 to measure [generations to solve/time to solve] of several runs of the algorithm" << endl;
	cout << endl;
	cout << " Or B to go back to the Start Menu" << endl;
	cout << " Or X to Exit" << endl;
	cout << endl;
}

void Output::outputEditGASettings()
{
	cout << "--Edit Genetic Algorithm Settings--" << endl;
	cout << endl;
	cout << "Enter:" << endl;
	cout << endl;
	cout << " P to edit Parameters" << endl;
	cout << " D to edit Display settings" << endl;
	cout << endl;
	cout << " Or B to go back to the Start Menu" << endl;
	cout << " Or X to Exit" << endl;
	cout << endl;
}

void Output::outputEditGADisplaySettings()
{
	cout << "--Edit Display Settings--" << endl;
	cout << endl;
	cout << "Enter: " << endl;
	cout << " T/F to turn setting to True (On)/False (Off)" << endl;
	cout << " S to Skip editing this setting" << endl;
	cout << endl;
	cout << " Or B to go Back to Settings" << endl;
	cout << endl;
}

void Output::outputEditGAParamterSettings()
{
	cout << "--Edit Parameter Settings--" << endl;
	cout << endl;
	cout << "Enter: " << endl;
	cout << " S to Skip editing this setting" << endl;
	cout << " Or B to go Back to Settings" << endl;
	cout << endl;
}

void Output::outputStartMenu()
{
	cout << "--Start Menu--" << endl;
	cout << endl;
	cout << "Enter:" << endl;
	cout << endl;
	cout << " S to Start running the genetic algorithm" << endl;
	cout << " M to start Measuring the genetic algorithm" << endl;
	cout << " D to Display and edit the genetic algorithm settings" << endl;
	cout << endl;
	cout << " Or B to go Back to the Main Menu" << endl;
	cout << " Or X to Exit" << endl;
}

//get X position of cursor
int Output::getCursorX()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(hstdout, &cbsi))
	{
		return cbsi.dwCursorPosition.X;
	}

	return 0;
}

//get Y position of cursor
int Output::getCursorY()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(hstdout, &cbsi))
	{
		return cbsi.dwCursorPosition.Y;
	}

	return 0;
}

void Output::outputSelectionError()
{
	cout << endl;
	cout << "Error! Please enter a valid selection" << endl;
	cout << endl;
}

void Output::outputExitingProgram()
{
	cout << "--Closing program--" << endl;
	system("pause");
}