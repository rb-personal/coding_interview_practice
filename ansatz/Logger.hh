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

static const size_t CACHE_LINE_SIZE = 64;

#define _ALIGN alignas(CACHE_LINE_SIZE)

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
  class _ALIGN node_t {
    enum class node_type_t : uint8_t { INT, STRING };

  public:
    node_t(void) = delete;
    node_t(int val)
      : _type(node_type_t::INT), _val_i(val), _id(std::this_thread::get_id()), _next(nullptr) {}

    node_t(std::string *val)
      : _type(node_type_t::STRING), _val(val), _id(std::this_thread::get_id()), _next(nullptr) {}

    node_type_t _type;
    int _val_i;
    std::string *_val;
    std::thread::id _id;
    std::atomic<node_t*> _next;
  };
  static_assert(sizeof(node_t) == CACHE_LINE_SIZE);

  //
  _ALIGN node_t *_first;
  _ALIGN node_t *_last;
  _ALIGN std::atomic<bool> _wr_lock;

  std::string _desc;
  bool _stop_dw;
  std::thread _dw;

  //
  void common_constructor
  (void);

  void start
  (void);

  void write_to_disk
  (void);

  std::deque<node_t> _a;

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

static char const *LogEnd = "\n";

static Logger Log;
static Logger LogINFO("INFO");
static Logger LogDEBUG("DEBUG");
static Logger LogVERBOSE("VERBOSE");

#endif
