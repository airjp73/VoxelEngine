/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

//c++ std
#include <string>
#include <iostream>
//VoxelEngine
#include "../include/Logger.h"

const std::string Logger::errorLevels[] = {
  "INFO", "DEBUG", "WARNING", "ERROR", "FATAL"
};

void Logger::log(std::string message, int errorLevel) {
  if (errorLevel > FATAL || errorLevel < INFO) {
    errorLevel = ERROR;
    log("Logger sent invalid errorLevel", ERROR);
  }
  std::cout << errorLevels[errorLevel] << " -- " << _ownerName << " -- " << message << "\n";
}
