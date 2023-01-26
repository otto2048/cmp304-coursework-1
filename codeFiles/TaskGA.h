#pragma once
#include "Task.h"
#include "Chromosome.h"

//an abstract class to represent all tasks for the GA
class TaskGA : public Task
{
public: 
	TaskGA(Chromosome*);

protected:
	Chromosome* individual;

};