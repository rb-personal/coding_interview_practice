#include <iostream>
#include "ch16.hh"
#include "ch17.hh"

using namespace std;

int main(int argc, char **argv)
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);

  int result;
  add_two(a, b, result);
  cout << result << endl;

  return 0;
}
