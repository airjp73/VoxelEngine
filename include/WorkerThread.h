/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "TaskScheduler.h"

/*
  WorkerThread contains the code that the worker threads will be executing
*/

class WorkerThread {
private:
  TaskScheduler* scheduler;
public:
  void operator()();
};

#endif
