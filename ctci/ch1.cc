/*
 * Chapter 1 -- Arrays and Strings
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;

// 1.1 Does a string have all unique characters
// Bad soln -- walk through string, for each character -- check whole string (O(n^2) time, O(1) space)
// Decent soln -- keep a map / hash table of characters. If you get a collision, return false (O(n) time, O(n) space)
// Good soln -- limited set of characters (ASCII has 128) (O(n) time, O(1) space)
// Other possibilities -- sort the string in place and search for neighbors which match
bool is_unique(const char *str)
{
  bool f[128];
  memset(f, 0, sizeof(f));

  auto i = str;
  while ( (*i!=0) ) {
    if (f[*i]) return false;
    f[*i] = true;
    i++;
  }
  return true;
}

int main(int argc, char **argv)
{
  if(is_unique("abc")) cout << "Unique" << endl;
  else cout << "Not Unique" << endl;
  return 0;
}
