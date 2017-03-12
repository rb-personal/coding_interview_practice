#include "message.hh"
#include <cstdio>
#include <cstdlib>
#include <iostream>

std::ostream& operator<<(std::ostream &out, const message_t &m)
{
  out
    << "Time: "       << m._time_ms
    << ", Ticker: "   << m._ticker
    << ", Type: "     << m._msg_type
    << ", Order Id: " << m._order_id
    << ", Qty: "      << m._qty
    << ", Price: "    << m._price
    << std::endl;
  return out;
}
