#include "Farm.h"

void Farm::addTask(Task* task)
{
	//lock access to queue
	unique_lock<mutex> uniqueTaskLock(taskMutex);

	//push new task to the queue
	taskQueue.push(task);
}

void Farm::run()
{
	//fork-join pattern
	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		//create thread to run task
		threads.push_back(new thread([&] {
			while (true)
			{
				taskMutex.lock();

				if (taskQueue.empty())
				{
					taskMutex.unlock();
					return;
				}

				Task* t = taskQueue.front();
				taskQueue.pop();
				taskMutex.unlock();

				t->run();
				delete t;
			}
			}
		));
	}

	//join all threads
	for (auto thread : threads)
	{
		thread->join();
		delete thread;

	}

	threads.clear();
}