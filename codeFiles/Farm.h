#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <vector>

#include "Task.h"

using std::queue;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::vector;

class Farm
{
public:
	void addTask(Task* task);
	void run();
	
private:
	queue<Task*> taskQueue;
	mutex taskMutex;
	vector<thread*> threads;
};

