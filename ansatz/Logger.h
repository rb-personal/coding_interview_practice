#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <deque>
#include <ctime>
#include <sstream>

#ifdef __GNUC__
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

using namespace std;

string LogEnd("\n");

class Logger : public ostream {
 private:
  void write_to_disk(void);

  string _desc;
  mutex _mutex;
  deque<string> _records;
  bool _print_intro;

  // Note -- not using atomics b/c assuming x86 strong memory ordering
  bool _stop_dw;
  bool _pause_dw;

  thread _dw;

 public:
  Logger(char *dsc);
  Logger(void);
  ~Logger(void);

  void start(void);

  Logger& operator<<(int value);
  Logger& operator<<(string &value);
};

Logger Log;

/* Logger LogINFO("INFO"); */
/* LogINFO.start(); */

/* Logger LogDEBUG("DEBUG"); */
/* LogDEBUG.start(); */

/* Logger LogVERBOSE("VERBOSE"); */
/* LogVERBOSE.start(); */

#endif
