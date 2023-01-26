#include "FileIO.h"

//creating a map for a specific gene
vector<vector<int>> FileIO::readMapFile(string filePath, char pathCode)
{
	std::ifstream mapFile;
	vector<vector<int>> returnVal;

	//open file
	mapFile.open(filePath);

	if (mapFile.is_open())
	{
		vector<string> lines;

		string text;

		//get all lines in file
		while (mapFile >> text)
		{
			lines.push_back(text);
		}

		//set each character to character in grid
		for (int i = 0; i < lines.size(); i++)
		{
			returnVal.push_back(vector<int>());
			for (int j = 0; j < lines[i].size(); j++)
			{
				char val = lines[i][j];

				if (val != '-')
				{
					//check if this is a start/end point for this path
					if (val == pathCode)
					{
						returnVal[i].push_back(-3);
					}
					else
					{
						returnVal[i].push_back(-2);
					}
				}
				else
				{
					returnVal[i].push_back(-1);
				}

			}
		}

		mapFile.close();

		return returnVal;
	}
	cout << "Failed to find map file" << endl;
	return returnVal;
}

vector<string> FileIO::readFile(string filePath)
{
	std::ifstream solution;

	//open file
	solution.open(filePath);

	if (solution.is_open())
	{
		vector<string> lines;

		string text;

		//get all lines in file
		while (solution >> text)
		{
			lines.push_back(text);
		}

		solution.close();

		return lines;
	}

	cout << "Failed to find solution file" << endl;
	return vector<string>();
}

vector<string> FileIO::readOptionalFile(string filePath)
{
	std::ifstream solution;

	//open file
	solution.open(filePath);

	if (solution.is_open())
	{
		vector<string> lines;

		string text;

		//get all lines in file
		while (solution >> text)
		{
			lines.push_back(text);
		}

		solution.close();

		return lines;
	}

	return vector<string>();
}

void FileIO::saveToFile(vector<string> lines, string filePath_)
{
	std::ofstream file(filePath_);

	if (file.is_open())
	{
		for (auto line : lines)
		{
			file << line << endl;
		}

		file.close();
	}
}