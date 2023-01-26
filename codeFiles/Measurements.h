#pragma once
#include "FlowGA.h"

#include <chrono> //timing
#include <fstream> //writing to csv

using std::ofstream;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef std::chrono::steady_clock theClock;

class Measurements
{
private:
	string saveFile;
	int solveCount = 10;
public:
	Measurements(string);
	Measurements(string, int);
	bool runMeasurements(FlowGA& flow, bool);
};