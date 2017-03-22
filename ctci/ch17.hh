#ifndef __CH17_HH__
#define __CH17_HH__

#include <iostream>

using namespace std;

bool add_two(int a, int b, int &c)
{
  cout << "Adding " << a << " and " << b << endl;

  // Write a ripple-carry adder
  int carry = 0;
  int index = 0;
  c = 0;

  while (a != 0 || b != 0) {
    int a_bit = a&1;
    int b_bit = b&1;
    a >>= 1;
    b >>= 1;
    if (a_bit == 0 && b_bit == 0) {
      c |= carry << index;
      carry = 0;
    }
    else if (a_bit ^ b_bit == 1) {
      if (carry == 0) c |= 1 << index;
    }
    else {
      if (carry == 0) carry = 1;
      else c |= 1<<index;
    }
    ++index;
  }
  return (carry == 0);
}

#endif
