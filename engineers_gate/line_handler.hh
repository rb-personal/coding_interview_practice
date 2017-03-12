#ifndef __LINE_HANDLER_HH__
#define __LINE_HANDLER_HH__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "message.hh"
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>

class line_handler_t {
public:
  line_handler_t(const char *fpath);

  inline bool get_md_message(message_t &msg)
  {
    if (!_istream) return false;

    using namespace std;

    string line;
    string cell;

    do {
      if (unlikely(!_istream))
	return false;

      if (unlikely(!getline(_istream, line)))
	return false;

      if (unlikely(line.empty()))
	return false;

      stringstream linestream(line);

      getline(linestream, cell, ',');
      msg._time_ms = stol(cell);

      getline(linestream, msg._ticker, ',');

      getline(linestream, cell, ',');
      msg._order_id = stol(cell);

      getline(linestream, cell, ',');
      msg._msg_type = cell[0];

      getline(linestream, cell, ',');
      msg._qty = stoi(cell);

      getline(linestream, cell, ',');
      msg._price = stoi(cell);
    } while (msg._msg_type == 'T' ||
	     msg._msg_type == 'X');

    return true;
  }

private:
  line_handler_t(void) = delete;
  line_handler_t(const line_handler_t&) = delete;
  line_handler_t& operator=(const line_handler_t&) = delete;

  std::ifstream _istream;
};

#endif
