#ifndef __POSITION_SERVICE_HH__
#define __POSITION_SERVICE_HH__

//#define __DEBUG

#include <unordered_map>
#include <deque>
#include <string>
#include <utility>
#include <algorithm>

#include "inttypes.h"

class PositionService {
public:
  PositionService
  (const char *fills,
   const char *prices);

  ~PositionService(void);

  void
  process(void);

private:
  // Utility containers
  typedef struct {
    double price;
    int sqty;
  } position_t;

  typedef struct {
    char type;
    long when;
    double price;
    int qty;
    char dir_c;
    char which[8];
  } fill_read_data_t;

  typedef struct {
    char type;
    long when;
    char which[8];
    double price;
  } price_read_data_t;

  PositionService(void);

  //
  bool
  read_fill_record(void);

  bool
  read_price_record(void);

  void
  process_fill(void);

  void
  process_price(void);

  void
  print_current_positions(void);

  //
  FILE *_ff = nullptr, *_pf = nullptr;

  bool read_fill, fresh_fill, fills_finished;
  bool read_price, fresh_price, prices_finished;

  fill_read_data_t fill_read_data;
  price_read_data_t price_read_data;

  std::unordered_map<std::string, std::pair<double, std::deque<position_t> > > curr_positions;
};

#endif
