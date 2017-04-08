#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "PositionService.hh"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Usage <fills> <prices>" << endl;
    exit(EXIT_FAILURE);
  }

  PositionService ps(argv[1], argv[2]);
  ps.process();

  exit(EXIT_SUCCESS);
}
