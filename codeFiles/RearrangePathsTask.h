#pragma once
#include "TaskGA.h"
#include <functional>

class RearrangePathsTask : public TaskGA
{
public:
	using TaskGA::TaskGA;

	//overriding Task run function
	void run();
};

