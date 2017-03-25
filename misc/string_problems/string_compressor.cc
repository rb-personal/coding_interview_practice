#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <deque>

using namespace std;

int main(int argc, char **argv)
{
  //Your code here
  char *src = argv[1];
  if (src[1] == 0) {
    cout << src << endl;
    return 0;
  }

  char *wrPtr = src;
  char *rdPtr = src+1;
  char curr = src[0];
  size_t count = 1;

  do {
    while(*rdPtr != 0 && *rdPtr == curr) {
      ++rdPtr;
      ++count;
    }

    *wrPtr++ = curr;
    curr = *rdPtr++;

    if (count > 1) {
      sprintf(wrPtr, "%zu", count);
      auto shift = 1u;
      while (count/=10) ++shift;
      wrPtr += shift;
      //*wrPtr++ = count+'0';
    }
    count = 1;
  } while(curr != '\0');

  *wrPtr = 0;
  cout << src << endl;
  return 0;
}
