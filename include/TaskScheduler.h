/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H


#include "ITask.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

/*
  The TaskScheduler is a singleton whose job it is to multithread the game engine
  Whenever work needs to be done, a task is sent into the scheduler
*/
class TaskScheduler {
private:
  //singleton
  TaskScheduler();
  static TaskScheduler* _instance;

  //threads
  int NUM_THREADS;
  bool terminateThreads;
  std::vector<std::thread> workerThreads;

  //task queues
  std::queue<ITask*> highPri;
  std::queue<ITask*> medPri;
  std::queue<ITask*> lowPri;

  //mutex for task queues
  std::mutex taskQ_mut;
  std::condition_variable needWork;
  int numLowPri;
  int maxLowPri;

public:
  //singleton
  static TaskScheduler* getInstance();

  //add/aqcuire task
  enum Priority {LOW, MED, HIGH};
  void addTask(ITask* task, Priority pri);
  ITask* getTask(bool highPriOnly = false);

  //cleanup when ending process
  bool shouldTerminate();
  void terminate();
};



#endif
