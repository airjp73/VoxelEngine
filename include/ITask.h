/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/


#ifndef ITASK_H
#define ITASK_H

//Used for interfacing with task scheduler
class ITask {
public:
  virtual void execute() = 0;
};

#endif
