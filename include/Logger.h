/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <mutex>

class Logger {
private:
  static std::mutex logMut;
  std::string _ownerName;
public:
  static const int INFO = 0;
  static const int DEBUG = 1;
  static const int WARN = 2;
  static const int ERROR = 3;
  static const int FATAL = 4;

  static const std::string errorLevels[];

  Logger(std::string ownerName)
    : _ownerName(ownerName)
  {}
  void log(std::string message, int errorLevel);
};

#endif
