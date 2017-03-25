#ifndef __LOGGER_HH__
#define __LOGGER_HH__

#include <iostream>
#include <string>
#include <thread>
#include <deque>
#include <ctime>
#include <sstream>
#include <atomic>
#include <tuple>
#include <inttypes.h>

static const size_t CACHE_LINE_SIZE = 64;
static char const *LogEnd = "\n";

inline
uint64_t rdtscp
(void)
{
  uint64_t rax, rdx;
  asm volatile ("rdtscp" : "=a"(rax), "=d"(rdx));
  return (rdx<<32 | rax);
}

class Logger {
private:
  Logger(const Logger &other) = delete;
  Logger& operator=(const Logger &other) = delete;

  //
  class alignas(CACHE_LINE_SIZE) node_t {
  public:
    enum class node_type_t : uint8_t { INT, STRING };

    node_t(void) = delete;
    node_t(int val)
      : _type(node_type_t::INT), _val_i(val), _id(std::this_thread::get_id()), _next(nullptr) {}

    node_t(std::string *val)
      : _type(node_type_t::STRING), _val(val), _id(std::this_thread::get_id()), _next(nullptr) {}

    inline bool is_log_end(void) {
      if (_val == nullptr) return false;
      return (_val->compare(LogEnd) == 0);
    }

    node_type_t _type;
    int _val_i;
    std::string *_val;
    std::thread::id _id;
    std::atomic<node_t*> _next;
  };
  static_assert(sizeof(node_t) == CACHE_LINE_SIZE);

  //
  node_t *_first;
  uint8_t pad0[CACHE_LINE_SIZE - sizeof(node_t*)];

  node_t *_last;
  uint8_t pad1[CACHE_LINE_SIZE - sizeof(node_t*)];

  std::atomic<bool> _wr_lock;
  uint8_t pad2[CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];

  std::atomic<bool> _stop_dw;
  uint8_t pad3[CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];
  //

  std::string _desc;
  std::thread _dw;
  bool _print_intro;

  //
  void common_constructor
  (void);

  void start
  (void);

  void write_to_disk
  (void);

  bool print
  (void);

public:
  Logger
  (char const *dsc);

  Logger
  (void);

  ~Logger
  (void);

  Logger& operator<<(const int value) noexcept;
  Logger& operator<<(char const* value) noexcept;
};

//
static Logger Log;
static Logger LogINFO("INFO");
static Logger LogDEBUG("DEBUG");
static Logger LogVERBOSE("VERBOSE");

#endif
