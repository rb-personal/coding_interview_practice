#include "util.hh"
#include "book.hh"
#include <algorithm>

void
instr_book_t::update(const message_t &msg)
{
  side_t side = side_t::B;

  switch(msg._msg_type) {
  case 'B': {
    // New Bid
    if (unlikely(_order_map.find(msg._order_id) != _order_map.end())) {
      TERROR("[BUY] Duplicate Order ID: %", msg._order_id);
    }

    _order_map.insert({msg._order_id, {side_t::B, msg._price, msg._qty}});
    auto &order = _order_map[msg._order_id];
    update_book_side(order, msg._qty);

    break;
  }

  case 'S': {
    // New Ask
    if (unlikely(_order_map.find(msg._order_id) != _order_map.end())) {
      TERROR("[SELL] Duplicate Order ID: %", msg._order_id);
    }

    _order_map.insert({msg._order_id, {side_t::S, msg._price, msg._qty}});
    auto &order = _order_map[msg._order_id];
    update_book_side(order, msg._qty);
    side = side_t::S;

    break;
  }

  case 'C': {
    // Decrease/Reduce
    if (unlikely(_order_map.find(msg._order_id) == _order_map.end())) {
      TERROR("[DECREASE] Couldn't find Order ID: %", msg._order_id);
    }

    order_t &order = _order_map[msg._order_id];
    if (unlikely(order._qty < msg._qty)) {
      TERROR("[DECREASE] Current qty (%) < decrease to qty (%)\n", order._qty, msg._qty);
    }

    update_book_side(order, msg._qty - order._qty);
    order._qty = msg._qty;
    side = order._side;

    break;
  }

  case 'D': {
    // Delete/Cancel
    if (unlikely(_order_map.find(msg._order_id) == _order_map.end())) {
      TERROR("[DELETE] Couldn't find Order ID: %", msg._order_id);
    }

    order_t &order = _order_map[msg._order_id];
    update_book_side(order, -order._qty);
    _order_map.erase(msg._order_id);
    side = order._side;

    break;
  }

  case 'E': {
    // Execute
    if (unlikely(_order_map.find(msg._order_id) == _order_map.end())) {
      TERROR("[EXECUTE] Couldn't find Order ID: %", msg._order_id);
    }

    order_t &order = _order_map[msg._order_id];
    if (unlikely(order._qty < msg._qty)) {
      TERROR("[EXECUTE] Current qty (%) < decrease to qty (%)\n", order._qty, msg._qty);
    }

    update_book_side(order, -msg._qty);
    order._qty -= msg._qty;
    if (order._qty == 0) _order_map.erase(msg._order_id);
    side = order._side;

    break;
  }

  case 'F': {
    // Fill
    if (unlikely(_order_map.find(msg._order_id) == _order_map.end())) {
      TERROR("[FILL] Couldn't find Order ID: %", msg._order_id);
    }

    order_t &order = _order_map[msg._order_id];
    update_book_side(order, -order._qty);
    _order_map.erase(msg._order_id);
    side = order._side;

    break;
  }

  default:
    break;
  }

  check_tob(msg._time_ms, side);
}

void
instr_book_t::update_book_side(const order_t &order, const int32_t share_delta = 0)
{
  auto side = order._side;
  auto &book_side = (side == side_t::B) ? _bids : _asks;

  if (book_side.empty()) {
    book_side.insert(book_side.begin(), {order._price, order._qty});
    return;
  }

  auto bid_cmp = [](const price_level_t &pl, const uint32_t op)->bool{return (pl._price > op);};
  auto ask_cmp = [](const price_level_t &pl, const uint32_t op)->bool{return (pl._price < op);};

  auto it = std::lower_bound(book_side.begin(), book_side.end(), order._price, (side == side_t::B) ? bid_cmp : ask_cmp);

  if (it == book_side.end() || it->_price != order._price) {
    if (share_delta > 0)
      book_side.insert(it, {order._price, order._qty});
    else
      TERROR("Something bad happened");
  }
  else {
    it->_qty += share_delta;
    if (it->_qty == 0) book_side.erase(it);
  }
}
