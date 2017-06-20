/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include "../include/WorkerThread.h"
#include "../include/TaskScheduler.h"

void WorkerThread::operator()() {
  scheduler = TaskScheduler::getInstance();

  while(!scheduler->shouldTerminate()) {
    ITask* myTask = scheduler->getTask();
    myTask->execute();
  }
}
