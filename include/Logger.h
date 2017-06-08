#include <iostream>
#include <string>

class Logger {
private:
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
