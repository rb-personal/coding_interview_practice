#ifndef __MESSAGE_HH__
#define __MESSAGE_HH__

#include <iostream>
#include "util.hh"

const size_t TICKER_WIDTH = 7;

// Market Data Messages
// struct packing to reduce slop
typedef struct alignas(CACHE_LINE_SIZE) {
 public:
  char _msg_type;
  std::string _ticker;
  uint64_t _time_ms;
  uint64_t _order_id;
  int32_t  _qty;
  uint32_t _price;
} message_t;
//static_assert(sizeof(message_t) == 32); // (Broken by alignas)
static_assert(sizeof(message_t) <= CACHE_LINE_SIZE);

std::ostream& operator<<(std::ostream &out, const message_t &m);

#endif
