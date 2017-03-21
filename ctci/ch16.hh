#ifndef __CH16_HH__
#define __CH16_HH__

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <utility>
#include <cmath>

using namespace std;

size_t get_frequency_v0(vector<string> &book, string &word) {
  size_t ret = 0;
  for_each(book.begin(), book.end(), [&](string &book_word){ if(book_word.compare(word) == 0) ++ret; });
  return ret;
}

map<string, size_t> book_map;
size_t get_frequency_v1(string &word) {
  if (book_map.find(word) == book_map.end()) return 0;
  return book_map[word];
}

size_t get_frequency_v1(vector<string> &book, string &word) {
  book_map.clear();
  for_each(book.begin(), book.end(), [](string &book_word){
      auto book_map_entry = book_map.find(book_word);
      if (book_map_entry == book_map.end()) book_map.insert({book_word, 1});
      book_map_entry->second++;
    });
  return get_frequency_v1(word);
}

typedef struct {
  float x, y;
} coordinate_t;

typedef struct {
  coordinate_t start, end;

  inline float get_slope(void) const noexcept
  {
    return (end.y - start.y)/(end.x - start.x);
  }

  inline float get_yintercept(void) const noexcept
  {
    float s = get_slope();
    return (end.y - s*end.x);
  }

  inline float get_length(void) const noexcept
  {
    return sqrt(pow(end.x - start.x, 2.f) + pow(end.y - start.y, 2.f));
  }
} line_t;


bool get_point_of_intersection(const line_t &line1, const line_t &line2, coordinate_t &intersect) {
  float s1 = line1.get_slope();
  float b1 = line1.get_yintercept();

  float s2 = line2.get_slope();
  float b2 = line2.get_yintercept();

  if (s1 == s2) {
    if (b1 == b2) {
      if (line1.get_length() > line2.get_length()) {
	intersect.x = line2.start.x;
	intersect.y = line2.start.y;
      } else {
	intersect.x = line1.start.x;
	intersect.y = line1.start.y;
      }
      return true;
    }
    return false;
  }

  // s1*x + b1 = s2*x + b2
  // x = (b2-b1)/(s1-s2)
  intersect.x = (b2-b1)/(s1-s2);
  intersect.y = s1*intersect.x + b1;

  return true;
}

enum class tic_tac_toe_entry_t { X, O };

bool tic_tac_toe_win(const vector<tic_tac_toe_entry_t> &board)
{
  if (board[0] == board[1] && board[1] == board[2]) return true;
  if (board[3] == board[4] && board[4] == board[5]) return true;
  if (board[6] == board[7] && board[7] == board[8]) return true;

  if (board[0] == board[3] && board[3] == board[6]) return true;
  if (board[1] == board[4] && board[4] == board[7]) return true;
  if (board[2] == board[5] && board[5] == board[8]) return true;

  if (board[0] == board[4] && board[4] == board[8]) return true;
  if (board[2] == board[4] && board[4] == board[6]) return true;

  return false;
}
#endif
