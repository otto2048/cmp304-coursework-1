#pragma once
//abstract base class: task to be executed
class Task
{
public:
	virtual ~Task()
	{

	}

	//run the task
	virtual void run() = 0;
};

