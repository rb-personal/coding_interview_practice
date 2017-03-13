#ifndef __MAIN_HH__
#define __MAIN_HH__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <mutex>
#include <chrono>
#include <thread>
#include <deque>
#include <unistd.h>
#include "util.hh"

using namespace std;

const unsigned NUM_THREADS = 8;

// data structures
// quote
class quote_t {
public:
  quote_t(void) = default;
  quote_t(uint32_t time, float bid, float ask)
    : _time(time), _bid(bid), _ask(ask)
  {}
  quote_t(const quote_t &other)
    : _time(other.get_time()), _bid(other.get_bid()), _ask(other.get_ask())
  {}
  quote_t& operator=(const quote_t &other)
  {
    if (this != &other) {
      // ** primitive types so don't have to worry about performance
      _time = other.get_time();
      _bid = other.get_bid();
      _ask = other.get_ask();
    }
    return *this;
  }

  inline friend ostream&
  operator<<(ostream& out, const quote_t& rhs)
  {
    out << rhs._time << "," << rhs._bid << "," << rhs._ask;
    return out;
  }

  ~quote_t(void) = default;

  // accessors
  inline uint32_t
  get_time(void) const
  { return _time; }

  inline float
  get_bid(void) const
  { return _bid; }

  inline float
  get_ask(void) const
  { return _ask; }

  // mutators
  inline void
  set_time(uint32_t time)
  { _time = time; }

  inline void
  set_bid(float bid)
  { _bid = bid; }

  inline void
  set_ask(float ask)
  { _ask = ask; }

private:
  uint32_t _time;
  float _bid;
  float _ask;
};
//static_assert(sizeof(quote_t) <= CACHE_LINE_SIZE);

// trade
class trade_t {
public:
  trade_t(void) = default;
  trade_t(uint32_t time, char side, float price, int32_t qty)
    : _time(time), _side(side), _price(price), _qty(qty)
  {}

  ~trade_t(void) = default;

  // printer
  inline friend ostream&
  operator<<(ostream &out, const trade_t &rhs)
  {
    out << rhs._time << "," << rhs._side << "," << rhs._price << "," << rhs._qty;
    return out;
  }

  // accessors
  inline uint32_t
  get_time(void) const
  { return _time; }

  inline char
  get_side(void) const
  { return _side; }

  inline float
  get_price(void) const
  { return _price; }

  inline int32_t
  get_qty(void) const
  { return _qty; }

  inline int32_t
  get_sqty(void) const
  { return ( (_side == 'B') ? _qty : -_qty ); }

  inline void
  reduce(int32_t qty)
  { _qty -= qty; }

  // mutators
  inline void
  set_side(char side)
  { _side = side; }

  inline void
  set_time(uint32_t time)
  { _time = time; }

  inline void
  set_qty(uint32_t qty)
  { _qty = qty; }

  inline void
  set_price(float price)
  { _price = price; }

private:
  char _side;
  uint32_t _time;
  int32_t _qty;
  float _price;
};
//static_assert(sizeof(trade_t) <= CACHE_LINE_SIZE);

// Didn't do this b/c I was running into some weird allocator issues
// type aliasing
//using state_t = pair<quote_t, trade_t>;

// function prototypes
void process_quote
(
 const string &instrument,
 const quote_t &q,
 bool &read_quote,
 bool &read_trade,
 bool &fresh_quote,
 bool &fresh_trade
 );

void process_trade
(
 const string &instrument,
 const trade_t &t,
 bool &read_quote,
 bool &read_trade,
 bool &fresh_quote,
 bool &fresh_trade
 );

inline char bid_liquidity
(
 const trade_t &t,
 const quote_t &q
 )
{
  if (t.get_price() <= q.get_bid())
    return 'P';

  else if (t.get_price() >= q.get_ask())
    return 'A';

  // Should be unreachable
  return 'X';
}

inline char ask_liquidity
(
 const trade_t &t,
 const quote_t &q
 )
{
  if (t.get_price() >= q.get_ask())
    return 'P';

  else if (t.get_price() <= q.get_bid())
    return 'A';

  // Should be unreachable
  return 'X';
}

static inline
void generate_report
(
 const string &instrument,
 const quote_t &oq,
 const trade_t &ot,
 const quote_t &cq,
 const trade_t &ct,
 const int32_t sqty
 )
{
  char o_liq, c_liq;

  if (ct.get_side() == 'B') {
    // Buy Execution
    o_liq = ask_liquidity(ot,oq);
    c_liq = bid_liquidity(ct,cq);
  }
  else {
    // Sell Execution
    o_liq = bid_liquidity(ot,oq);
    c_liq = ask_liquidity(ct,cq);
  }

  float paired_qty = static_cast<float>(abs(sqty));
  auto pnl = paired_qty * (ct.get_price() - ot.get_price());

  printf("%u,%u,"
	 "%s,%u,%.2f,"
	 "%c,%c,"
	 "%.2f,%.2f,"
	 "%.2f,%.2f,%.2f,%.2f,"
	 "%c,%c\n",
	 ot.get_time(), ct.get_time(),
	 instrument.c_str(), abs(sqty), pnl,
	 ((sqty > 0) ? 'S':'B'), ((sqty > 0) ? 'B':'S'),
	 ot.get_price(), ct.get_price(),
	 oq.get_bid(), cq.get_bid(), oq.get_ask(), cq.get_ask(),
	 o_liq, c_liq);
}

// globals
unordered_map<string, quote_t> book;
unordered_map<string, deque<quote_t> > open_quotes;
unordered_map<string, deque<trade_t> > open_trades;

#ifdef _MULTITHREADED
unordered_set<string> shared_records;
mutex shared_records_mutex;
#endif
#endif
