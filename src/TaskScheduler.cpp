/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/


#include <thread>
#include <mutex>

#include "../include/TaskScheduler.h"
#include "../include/WorkerThread.h"
#include "../include/ITask.h"

TaskScheduler* TaskScheduler::_instance = nullptr;

TaskScheduler* TaskScheduler::getInstance() {
  if (_instance == nullptr)
    _instance = new TaskScheduler();
  return _instance;
}

TaskScheduler::TaskScheduler() : terminateThreads(false), numLowPri(0), maxLowPri(1) {
  NUM_THREADS = std::thread::hardware_concurrency() - 1;
  for (int i = 0; i < NUM_THREADS; ++i)
    workerThreads.push_back(std::thread(WorkerThread()));
}

void TaskScheduler::addTask(ITask* task, Priority pri) {
  std::unique_lock<std::mutex> taskLock(taskQ_mut);

  if      (pri == Priority::HIGH)
    highPri.push(task);
  else if (pri == Priority::MED)
    medPri.push(task);
  else if (pri == Priority::LOW)
    lowPri.push(task);

  taskLock.unlock();
  needWork.notify_one();
}

ITask* TaskScheduler::getTask(bool highPriOnly /* = false */) {
  //used when the main thread wants to do some work
  if (highPriOnly) {
    std::unique_lock<std::mutex> taskLock(taskQ_mut);
    if (highPri.size() == 0)
      return nullptr;
    ITask* ret = highPri.front();
    highPri.pop();
    return ret;
  }

  std::unique_lock<std::mutex> taskLock(taskQ_mut);
  needWork.wait(taskLock, [&] {
    return highPri.size() > 0 || medPri.size() > 0 || (lowPri.size() > 0 && numLowPri < maxLowPri);
  });
  if (highPri.size() > 0) {
    ITask* ret = highPri.front();
    highPri.pop();
    return ret;
  }
  if (medPri.size() > 0) {
    ITask* ret = medPri.front();
    medPri.pop();
    return ret;
  }
  if (lowPri.size() > 0 && numLowPri < maxLowPri) {
    ITask* ret = lowPri.front();
    lowPri.pop();
    return ret;
  }

  return nullptr;
}

bool TaskScheduler::shouldTerminate() {
  return terminateThreads;
}

void TaskScheduler::terminate() {
  terminateThreads = true;
  for (std::thread &t : workerThreads)
    t.join();
}
