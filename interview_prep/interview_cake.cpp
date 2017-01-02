#include <iostream>
#include <vector>

extern "C"
{
#include "inttypes.h"
}

using namespace std;

class TempTracker {
public:
  TempTracker(void) {
    _m_temps.clear();
    _m_count = 0.;
    _m_mode_n = 0;
  }

  void insert(uint32_t temp) {
    _m_temps[temp]++;
    _m_count++;

    if (_m_temps[temp] > _m_mode_n) {
      _m_mode = temp;
      _m_mode_n = _m_temps[temp];
    }

    _m_mean = ((static_cast<float>(_m_count)-1.)*_m_mean + static_cast<float>(temp))/t;

    if (temp > get_max()) _m_max = temp;
    if (temp < get_min()) _m_min = temp;
  }

  uint32_t get_max(void) { return _m_max; }
  uint32_t get_min(void) { return _m_min; }
  float    get_mean(void){ return _m_mean;}
  uint32_t get_mode(void){ return _m_mode;}

private:
  TempTracker(TempTracker&) = delete;
  TemplTracker operator=(TempTracker&) = delete;

  // Biggest takeaway from this problem is to read the problem statement
  // carefully - if the input set is constrained then we can size our data
  // structures at design time and use O(1) space as opposed to using
  // containers which may incur overhead from resizing
  uint32_t _m_max;
  uint32_t _m_min;
  float    _m_mean;
  uint32_t _m_mode, _m_mode_n;

  std::array<uint32_t, 111> _m_temps;
  float _m_count;
};

static const uint32_t _UNDEFINED = ~0;
typedef struct {
  uint32_t left_x;
  uint32_t bottom_y;
  uint32_t width;
  uint32_t height;
} rectangle_t;

rectangle_t find_intersection(rectangle_t &a, rectangle_t &b) {

}

// input is an array of stock prices at every minute -- goal is to
// buy low and sell high
int get_max_profit(std::vector<int> &stock_prices_from_yesterday)
{
  // Check that we have enough to go on
  if (stock_prices_from_yesterday.size() < 2) return -1;

  int curr_min = stock_prices_from_yesterday[0];
  int curr_max_profit = stock_prices_from_yesterday[1] - curr_min;

  for (auto iter = stock_prices_from_yesterday.begin()+1; iter != stock_prices_from_yesterday.end(); ++iter) {
    if (*iter < curr_min) curr_min = *iter;
    if ( (*iter - curr_min) > curr_max_profit ) curr_max_profit = (*iter - curr_min);
  }

  return curr_max_profit;
}

int get_product_of_all_ints_except_at_index(std::vector<int> &arr)
{
  vector<int> prod_b, prod_a;
  prod_b.push_back(0);
  prod_a.push_back(0);

  int pb = 1, pa = 1;
  for (unsigned i = 1; i < arr.size(); i++) {
    auto b = arr[i-1];
    auto a = arr[arr.size()-i];
    prod_b[i] = (pb *= b ? b:1);
    prod_a[i] = (pa *= a ? a:1);
  }
  for (unsigned i = 0; i < arr.size(); ++i) {
    arr[i] = max(1, prod_b[i])*max(1,prod_a[i]);
  }

  // Learning
  /*
    As usual, try to use a greedy algoirthm that can get you a lot of
    information in the first sweep while you keep updating some
    important state -- key takeway is to utilize a form of memoization;
    don't do the same work over and over again. Rather, save the results
    a usable format in a LUT and then re-use as needed
   */
}

int main(void)
{

  return 0;
}
