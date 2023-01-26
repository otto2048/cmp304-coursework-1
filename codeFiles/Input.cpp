#include "Input.h"

string Input::mainMenuInteraction()
{
	//add selection options to selection range
	selectionRange.clear();
	selectionRange.push_back("E");
	selectionRange.push_back("D");
	selectionRange.push_back("X");

	do
	{
		system("cls");
		output.outputTitle();
		output.outputMainMenu();

		cin >> userInput;

	} while (!validation.checkSelection(userInput, selectionRange));

	if (userInput == "X" || userInput == "x")
	{
		return "X";
	}
	else if (userInput == "E" || userInput == "e")
	{
		//get file containing level data
		return inputFolder();
	}
	else
	{
		//using default level
		return "default";
	}
}

string Input::inputFolder()
{
	cout << "Enter name of the file containing level data: " << endl;
	cout << "	or enter B to go back to the Main Menu: " << endl;
	cin >> userInput;

	if (userInput == "B" || userInput == "b")
	{
		return mainMenuInteraction();
	}

	return userInput;
}

void Input::startMenuInteraction(FlowGA& flow)
{
	//add selection options to selection range
	selectionRange.clear();
	selectionRange.push_back("S");
	selectionRange.push_back("M");
	selectionRange.push_back("B");
	selectionRange.push_back("X");
	selectionRange.push_back("D");

	do
	{
		do
		{
			system("cls");
			output.outputTitle();

			output.outputStartMenu();

			//display base
			Chromosome c = flow.getBase();
			output.drawChromosome(c, 70, output.getCursorY() - 8, "Current level solution:");
			cout << endl;

			cin >> userInput;

		} while (!validation.checkSelection(userInput, selectionRange));

		if (userInput == "X" || userInput == "x")
		{
			return;
		}
		else if (userInput == "S" || userInput == "s")
		{
			//run GA
			flow.runGA();
			system("pause");

			//reset GA
			flow.reset();
		}
		else if (userInput == "M" || userInput == "m")
		{
			measurementOptionInteration(flow);
		}
		else if (userInput == "D" || userInput == "d")
		{
			//edit GA settings
			editGASettingsInteraction(flow);
		}
	} while (userInput != "B" && userInput != "b" && userInput != "X" && userInput != "x");
	
}

void Input::measurementOptionInteration(FlowGA& flow)
{
	//add selection options to selection range
	selectionRange.clear();
	selectionRange.push_back("1");
	selectionRange.push_back("2");
	selectionRange.push_back("X");
	selectionRange.push_back("B");

	do
	{
		system("cls");
		output.outputTitle();
		output.outputMeasurementOptions();

		cin >> userInput;
	} while (!validation.checkSelection(userInput, selectionRange));

	if (userInput == "X" || userInput == "x")
	{
		return;
	}
	else if (userInput == "1")
	{
		//enter folder to save results to
		inputMeasurementSaveLocation(flow);

		//use Measurements to run algorithm
		Measurements measure(userInput);

		if (measure.runMeasurements(flow, true))
		{
			cout << "Successfully saved measurements" << endl;
		}
		else
		{
			cout << "Failed to save measurements" << endl;
		}
		system("pause");

		measurementOptionInteration(flow);
	}
	else if (userInput == "2")
	{
		//get how many times to run the algorithm
		int runTimes = inputAlgorithmRunTimes();

		//enter folder to save results to
		inputMeasurementSaveLocation(flow);

		//use Measurements to run algorithm
		Measurements measure(userInput, runTimes);

		if (measure.runMeasurements(flow, false))
		{
			cout << "Successfully saved measurements" << endl;
		}
		else
		{
			cout << "Failed to save measurements" << endl;
		}
		system("pause");

		measurementOptionInteration(flow);
	}
	else
	{
		startMenuInteraction(flow);
	}
}

int Input::inputAlgorithmRunTimes()
{
	string intIn;
	do
	{
		cout << "Enter the number of times to run the algorithm: " << endl;
		cin >> intIn;
	} while (!validation.checkInt(intIn));

	return stoi(intIn);
}

void Input::inputMeasurementSaveLocation(FlowGA& flow)
{
	cout << "Enter name of the file to save results to: " << endl;
	cout << "	or enter B to go back to the Start Menu: " << endl;
	cin >> userInput;

	if (userInput == "B" || userInput == "b")
	{
		startMenuInteraction(flow);
	}
}

void Input::editGASettingsInteraction(FlowGA& flow)
{
	//add selection options to selection range
	selectionRange.clear();
	selectionRange.push_back("P");
	selectionRange.push_back("D");
	selectionRange.push_back("X");
	selectionRange.push_back("B");

	do
	{
		system("cls");
		output.outputTitle();
		output.outputGASettings(flow);
		output.outputEditGASettings();

		cin >> userInput;

	} while (!validation.checkSelection(userInput, selectionRange));

	if (userInput == "X" || userInput == "x")
	{
		return;
	}
	else if (userInput == "P" || userInput == "p")
	{
		//edit parameters
		editGAParamterSettingsInteraction(flow);
		editGASettingsInteraction(flow);
	}
	else if (userInput == "D" || userInput == "d")
	{
		//edit display
		editGADisplaySettingsInteraction(flow);
		editGASettingsInteraction(flow);
	}
	else
	{
		startMenuInteraction(flow);
	}
	
}

void Input::editGADisplaySettingsInteraction(FlowGA& flow)
{
	selectionRange.clear();
	selectionRange.push_back("S");
	selectionRange.push_back("T");
	selectionRange.push_back("F");
	selectionRange.push_back("B");

	output.outputEditGADisplaySettings();

	//for each display setting
	for (int i = 0; i < 3; i++)
	{
		do
		{
			switch (i)
			{
				case 0:
					cout << "Draw level whilst running algorithm: " << endl;
					break;
				case 1:
					cout << "Display whole generation: " << endl;
					break;
				case 2:
					cout << "Display the genes of the best chromosome: " << endl;
					break;
			}
			cin >> userInput;
		} while (!validation.checkSelection(userInput, selectionRange));

		if (userInput == "S" || userInput == "s")
		{
			continue;
		}
		else if (userInput == "B" || userInput == "b")
		{
			return;
		}
		else 
		{
			bool newValue = false;
			if (userInput == "T" || userInput == "t")
			{
				newValue = true;
			}

			switch (i)
			{
				case 0:
					flow.setDrawLevel(newValue);
					break;
				case 1:
					flow.setDisplayGeneration(newValue);
					break;
				case 2:
					flow.setDisplayGenes(newValue);
					break;
			}
		}
	}
}

void Input::editGAParamterSettingsInteraction(FlowGA& flow)
{
	selectionRange.clear();
	selectionRange.push_back("S");
	selectionRange.push_back("B");
	selectionRange.push_back("T");
	selectionRange.push_back("F");
	selectionRange.push_back("M");

	output.outputEditGAParamterSettings();

	do
	{
		cout << "Enter number of individuals in a generation: " << endl;
		cin >> userInput;

		if (userInput == "S" || userInput == "s")
		{
			break;
		}

		if (userInput == "B" || userInput == "b")
		{
			return;
		}

	} while (!validation.checkInt(userInput));

	if (userInput != "S" && userInput != "s")
	{
		flow.setPopulationSize(stoi(userInput));
	}

	do
	{
		cout << "Enter a number between -1 and 100 to represent the chance a chromosome will be mutated (0 <= x < 100): " << endl;
		cin >> userInput;

		if (userInput == "S" || userInput == "s")
		{
			break;
		}

		if (userInput == "B" || userInput == "b")
		{
			return;
		}

	} while (!validation.checkValidIntRange(userInput, -1, 100));

	if (userInput != "S" && userInput != "s")
	{
		flow.setMutationRate(stoi(userInput));
	}

	do
	{
		cout << "Enter the number of the maximum amount of generations permitted to run: " << endl;
		cin >> userInput;

		if (userInput == "S" || userInput == "s")
		{
			break;
		}

		if (userInput == "B" || userInput == "b")
		{
			return;
		}
	} while (!validation.checkInt(userInput));

	if (userInput != "S" && userInput != "s")
	{
		flow.setMaxGen(stoi(userInput));
	}

	do
	{
		cout << "Stop mode: " << endl;
		cout << "	Enter T for Minimum Convergence (run until chromosomes converge on identical solution OR maximum number of generations run)" << endl;
		cout << "	Enter F for Maximum Convergence (run until chromosomes converge on the best solution OR maximum number of generations run)" << endl;
		cout << "	Enter M for Reach Maximum Generation (runs until the maximum number of generations run)" << endl;
		cin >> userInput;

		if (userInput == "S" || userInput == "s")
		{
			break;
		}

		if (userInput == "B" || userInput == "b")
		{
			return;
		}
	} while (!validation.checkSelection(userInput, selectionRange));

	if (userInput != "S" && userInput != "s")
	{
		FlowGA::stoppingCondition newValue;

		if (userInput == "T" || userInput == "t")
		{
			newValue = FlowGA::stoppingCondition::minimum;
		}
		else if (userInput == "F" || userInput == "f")
		{
			newValue = FlowGA::stoppingCondition::maximum;
		}
		else
		{
			newValue = FlowGA::stoppingCondition::maxGen;
		}

		flow.setStopMode(newValue);
	}
}

string Input::getUserInput()
{
	return userInput;
}