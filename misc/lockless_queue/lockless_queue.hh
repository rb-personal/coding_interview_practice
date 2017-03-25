#ifndef __LOCKLESS_QUEUE_HH__
#define __LOCKLESS_QUEUE_HH__

#include <inttypes.h>
#include <atomic>

static const unsigned CACHE_LINE_SIZE = 64;

template <typename t>
class lockless_queue_t {
private:

  class lq_node_t {
  public:
    lq_node_t(t *val) : _val(val), _next(nullptr) {}
    t *_val;
    std::atomic<lq_node_t*> _next;
  private:
    uint8_t pad[CACHE_LINE_SIZE - sizeof(t*) - sizeof(std::atomic<lq_node_t*>)];
  };


  lq_node_t *_first;
  uint8_t pad0[CACHE_LINE_SIZE - sizeof(lq_node_t*)];

  lq_node_t *_last;
  uint8_t pad1[CACHE_LINE_SIZE - sizeof(lq_node_t*)];

  std::atomic<bool> _consumer_lock;
  uint8_t pad2[CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];

  std::atomic<bool> _producer_lock;
  uint8_t pad3[CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];

public:
  lockless_queue_t
  (void)
  {
    _first = _last = new lq_node_t(nullptr);
    _consumer_lock = _producer_lock = false;
  }

  ~lockless_queue_t
  (void)
  {
    while (_first != nullptr) {
      auto tmp = _first;
      _first = _first->_next;
      delete tmp->_val;
      delete tmp;
    }
  }

  void produce
  (const t &value)
  {
    auto tmp = new lq_node_t(new t(value));

    // Spin loop using RMW atomic operation
    while (_producer_lock.exchange(true));

    // Update end of list (critical region)
    _last->_next = tmp;
    _last = tmp;

    // Release the lock
    _producer_lock = false;
  }

  bool consume
  (t &result)
  {
    // Spin loop using RMW atomic operation
    while (_consumer_lock.exchange(true));

    auto the_first = _first;
    auto the_next  = the_first->_next.load();

    if (the_next != nullptr) {
      // queue is not empty
      // Critical region
      auto val = the_next->_val;
      the_next->_val = nullptr;

      // New "first" element
      _first = the_next;

      // Release the lock
      _consumer_lock = false;

      // Do non-critical work outside of critical region
      result = *val; // ... like copying
      delete val;
      delete the_first;
      return true;
    }
    else {
      // queue is empty
      _consumer_lock = false; // release
      return false;
    }
  }

};

#endif
