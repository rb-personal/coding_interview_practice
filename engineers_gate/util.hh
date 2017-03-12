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

template <class T>
class list_node_t
{
public:
  list_node_t(T val)
    : _val(val), _next(nullptr)
  {}
  T _val;
  list_node_t *_next;
};

template <class T>
class pc_list_t {
public:
  pc_list_t(void)
  {
    _first = new list_node_t<T>(T());
    _marker = _last = _first;
  }

  virtual ~pc_list_t(void)
  {
    while (_first != nullptr) {
      auto tmp = _first;
      _first = tmp->_next;
      delete tmp;
    }
  }

  void produce(const T &t)
  {
    auto new_node = new list_node_t<T>(t);
    _last.load()->_next = new_node;
    _last.store(new_node);
  }

  bool consume(T &out)
  {
    bool ret = false;

    if (_marker.load() != _last.load()) {
      out = _marker.load()->_val;
      _marker.store(_marker.load()->_next);
      ret = true;
    }

    while (_first != _marker.load()) {
      auto tmp = _first;
      _first = _first->_next;
      delete tmp;
    }
    return ret;
  }

  inline bool empty(void)
  {
    return (_marker.load() != _last.load());
  }

private:
  list_node_t<T> *_first;
  std::atomic<list_node_t<T> *> _marker;
  std::atomic<list_node_t<T> *> _last;
};
