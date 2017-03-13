#ifndef __LOGGER_HH__
#define __LOGGER_HH__

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

static const char *LogEnd = "\n";

class Logger {
 private:
  void write_to_disk(void);
  inline void print_intro(void)
  {
    if (_print_intro) {
      ostringstream oss;
      auto result = time(nullptr);
      oss << _desc;
      oss << "Thread ID: 0x"
	  << hex <<  this_thread::get_id() << " ";
      oss << " Time: " << result << ": ";
      _print_intro = false;
      _records.push_back(oss.str());
    }
  }

  string _desc;
  mutex _mutex;
  deque<string> _records;
  bool _print_intro;

  // Note: Not using atomics b/c assuming
  // x86 strong memory ordering
  bool _stop_dw;
  bool _pause_dw;

  thread _dw;

 public:
  Logger(const char *dsc);
  Logger(void);
  ~Logger(void);

  void start(void);

  Logger& operator<<(int value);
  Logger& operator<<(const char *value);
};

static Logger Log;
static Logger LogINFO("INFO");
static Logger LogDEBUG("DEBUG");
static Logger LogVERBOSE("VERBOSE");

#endif
