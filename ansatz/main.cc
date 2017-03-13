#include "Logger.hh"

int main(int argc, char **argv)
{

  Log << "Hello" << 5 << LogEnd;
  LogINFO << "BLAH" << rand() << "FOO" << LogEnd;
  return 0;
}
