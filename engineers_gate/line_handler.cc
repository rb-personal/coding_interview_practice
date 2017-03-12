#include "line_handler.hh"

line_handler_t::line_handler_t(const char *fpath)
{
  using namespace std;

  _istream.open(fpath);
  if (!_istream)
    TERROR("Couldn't open file %", fpath);

  string line;
  if(!getline(_istream, line))
    TERROR("Couldn't read first line");
}
