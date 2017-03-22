#ifndef __LOGGER_HH__
#define __LOGGER_HH__

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <deque>
#include <ctime>
#include <sstream>
#include <atomic>
#include <tuple>

#ifdef __GNUC__
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

using namespace std;

static char const *LogEnd = "\n";

class Logger {
 private:
  void common_constructor(void);

  void write_to_disk(void);

  inline
  void print(void) noexcept
  {
    if (_records.empty()) return;
    auto &r = _records.front();
    if (_print_intro)
      cout << "[" << _desc         << "] "
	   << "(" << get<1>(r) << ") "
	   << "Thread: " << get<0>(r) << " -- ";
    cout << get<2>(r);
    _records.pop_front();
    if (get<2>(r).compare(LogEnd) == 0) {
      _print_intro = true;
    }
    else {
      cout << " ";
      _print_intro = false;
    }
  }

  string _desc;
  thread _dw;

  recursive_mutex _mutex;
  deque<tuple<thread::id, time_t, string> > _records;

  bool _print_intro;
  bool _stop_dw;
  bool _pause_dw;
  ostringstream _oss;

 public:
  Logger(char const *dsc);
  Logger(void);
  ~Logger(void);

  void start(void);

  Logger& operator<<(const int value) noexcept;
  Logger& operator<<(char const* value) noexcept;
};

static Logger Log;
static Logger LogINFO("INFO");
static Logger LogDEBUG("DEBUG");
static Logger LogVERBOSE("VERBOSE");

#endif
