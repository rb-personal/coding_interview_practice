#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <atomic>
#include <mutex>

#ifdef __GNUC__
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

const unsigned CACHE_LINE_SIZE = 64;

inline uint64_t rdtscp(void)
{
  uint64_t rax, rdx;
  asm volatile ("rdtscp" : "=a"(rax), "=d"(rdx));
  return (rdx<<32 | rax);
}

class custom_timer_t {
public:
  custom_timer_t(const char *desc)
    : _start (0), _sum(0), _num_obs(0), _timer_started(false), _desc(desc)
  {}

  inline void click(void)
  {
    if (!_timer_started) {
      _timer_started = true;
      _start = rdtscp();
      return;
    }

    _timer_started = false;
    auto now = rdtscp();
    _sum += (now - _start);
    ++_num_obs;
  }

  inline void report(void)
  {
    printf("[Timer -- %s] Obs: %lu, Avg Cycles: %lu\n",
	   _desc.c_str(), _num_obs, (_sum/_num_obs));
  }

  ~custom_timer_t(void)
  { report(); }

 private:
  uint64_t _start;
  uint64_t _sum;
  uint64_t _num_obs;
  bool _timer_started;
  std::string _desc;
};
//static_assert(sizeof(custom_timer_t) <= CACHE_LINE_SIZE);

inline void ERROR(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

inline void TERROR(const char *fmt)
{
  std::cerr << fmt << std::endl;
  exit(EXIT_FAILURE);
}

template <class T, class... Args>
inline void TERROR(const char *fmt, T value, Args... args)
{
  for(; *fmt!=0; fmt++) {
    if (*fmt == '%') {
      std::cerr << value;
      TERROR(fmt+1, args...);
      std::cerr << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cerr << *fmt;
  }
}
