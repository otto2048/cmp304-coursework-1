#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class FileIO
{
public:
	//creating a map for a specific gene
	vector<vector<int>> readMapFile(string, char);

	vector<string> readFile(string);
	vector<string> readOptionalFile(string);

	void saveToFile(vector<string>, string);
};

