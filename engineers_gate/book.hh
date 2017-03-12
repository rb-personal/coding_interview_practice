#ifndef __BOOK_HH__
#define __BOOK_HH__

#include <cstdint>
#include <deque>
#include <unordered_map>
#include "message.hh"
#include <iterator>
#include <algorithm>

enum class side_t : uint8_t
{ B, S };

inline std::ostream& operator<<(std::ostream &out, const side_t s)
{
  out << ((s == side_t::B) ? "Buy" : "Sell");
  return out;
}

class order_t
{
public:
  order_t (void) = default;
  order_t (side_t side, uint32_t price, int32_t qty)
    : _side(side), _price(price), _qty(qty)
  {}

  order_t(const order_t& other)
    : _side(other._side), _price(other._price), _qty(other._qty)
  {}

  order_t& operator=(const order_t& other)
  {
    if (this != &other) {
      _side = other._side;
      _price = other._price;
      _qty = other._qty;
    }
    return *this;
  }

  inline friend std::ostream& operator<<(std::ostream &out, const order_t &o) {
    out << "Side: " << o._side << ", Price: " << o._price << ", Qty: " << o._qty << std::endl;
    return out;
  }

  side_t _side;
  uint32_t _price;
  int32_t _qty;
};

class price_level_t
{
public:
  price_level_t(void) = default;

  price_level_t(uint32_t price, int32_t qty)
    : _price(price), _qty(qty)
  {}

  price_level_t(const price_level_t& other)
    : _price(other._price), _qty(other._qty)
  {}

  price_level_t& operator=(const price_level_t& other)
  {
    if (this != &other) {
      _price = other._price;
      _qty = other._qty;
    }
    return *this;
  }

  inline friend std::ostream& operator<<(std::ostream &out, const price_level_t &pl)
  {
    out << "Price: " << pl._price << ", Qty: " << pl._qty;
    return out;
  }

  inline bool operator==(const price_level_t &rhs) {
    return  (_price == rhs._price) && (_qty == rhs._qty);
  }

  inline bool operator!=(const price_level_t &rhs) {
    return !(*this == rhs);
  }

  uint32_t _price;
  int32_t _qty;
};

class instr_book_t {
public:
  instr_book_t(void) = default;
  instr_book_t(std::string &ticker)
    : _ticker(ticker), _best_bid({0,0}), _best_ask({0,0})
  {}
  void update(const message_t&);

  inline friend std::ostream& operator<<(std::ostream &out, const instr_book_t &ib) {
    out << ib._ticker << std::endl;
    out << "Ask:" << std::endl;
    for(auto riter = ib._asks.rbegin(); riter != ib._asks.rend(); ++riter) out << (*riter) << std::endl;

    out << std::endl << "Bid:" << std::endl;
    for_each(ib._bids.begin(), ib._bids.end(), [&](const price_level_t &pl){ out << pl << std::endl;; });
    return out;
  }

private:
  std::string _ticker;
  price_level_t _best_bid, _best_ask;
  std::deque<price_level_t> _bids, _asks;
  std::unordered_map<uint64_t, order_t> _order_map;

  void update_book_side(const order_t &order, const int32_t share_delta);

  inline bool check_tob(const uint64_t time_ms, side_t side)
  {
    bool tob_changed = false;
    if (side == side_t::B && _best_bid != _bids.front()) {
      _best_bid = _bids.front();
      tob_changed = true;
    }
    else if (side == side_t::S && _best_ask != _asks.front()) {
      _best_ask = _asks.front();
      tob_changed = true;
    }

    if (tob_changed) {
      printf("%lu,%s,",
	     time_ms, _ticker.c_str());
      if (_best_bid._qty)
	printf("%u,%d,", _best_bid._price, _best_bid._qty);
      else
	printf(",,");
      if (_best_ask._qty)
	printf("%u,%d\n", _best_ask._price, _best_ask._qty);
      else
	printf(",\n");
    }

    return tob_changed;
  }
};

#endif
