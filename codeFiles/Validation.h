#pragma once
#include <string>
#include <vector>
#include "output.h"

using std::string;
using std::vector;

class Validation
{
private:
	Output output;

public:
	//check user input against a collection of valid inputs
	bool checkSelection(string, vector<string>);

	//validate that the input is an integer
	bool checkInt(string);

	//validate that the input is an integer and is in within a specified range
	bool checkValidIntRange(string, int min, int max);
};

