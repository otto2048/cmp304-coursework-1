#pragma once
#include "Output.h"
#include "Validation.h"
#include "flowGA.h"
#include "Measurements.h"

using std::cin;

class Input
{
private:
	Output output;
	Validation validation;
	vector<string> selectionRange;
	string userInput;

	void editGADisplaySettingsInteraction(FlowGA&);
	void editGAParamterSettingsInteraction(FlowGA&);
	void inputMeasurementSaveLocation(FlowGA&);
	int inputAlgorithmRunTimes();
	string inputFolder();

public:
	string mainMenuInteraction();
	void startMenuInteraction(FlowGA&);
	void editGASettingsInteraction(FlowGA&);
	void measurementOptionInteration(FlowGA&);

	string getUserInput();
};

