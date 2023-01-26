#include "Validation.h"

//check user input against a collection of valid inputs
bool Validation::checkSelection(string input, vector<string> range)
{
	bool message = false;

	string upperCase = "";

	//create an upperCase version of the input to avoid case-sensitivity
	for (int i = 0; i < input.size(); i++)
	{
		upperCase += toupper(input[i]);
	}

	if (find(range.begin(), range.end(), input) == range.end() && find(range.begin(), range.end(), upperCase) == range.end())
	{
		message = true;
	}

	if (message)
	{
		output.outputSelectionError();
		system("pause");
		return false;
	}

	return true;
}

//validate that the input is an integer
bool Validation::checkInt(string a)
{
	for (int i = 0; i < a.length(); i++)
	{
		if (isdigit(a[i]) == false)
		{
			cout << endl;
			cout << "Error! Please enter a valid integer" << endl;
			cout << endl;
			return false; //isnt an integer
		}
	}
	return true;
}

//validate that the input is an integer and is in within a specified range
bool Validation::checkValidIntRange(string input, int min, int max)
{
	//isnt an integer
	if (!checkInt(input))
	{
		return false;
	}

	//check integer is within valid range
	int inputInt = std::stoi(input);
	if (inputInt < max && inputInt > min)
	{
		return true;
	}

	if (inputInt >= max)
	{
		cout << "Error, input exceeded max value of: " << max << endl;
	}
	else
	{
		cout << "Error, input below minimum value of: " << min << endl;
	}
	cout << endl;
	return false;
}