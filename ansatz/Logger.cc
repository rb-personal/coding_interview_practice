#include "Logger.hh"
#include <cstring>
#include <unistd.h>

using namespace std;

void
Logger::common_constructor
(void)
{
  _first = _last = new node_t(nullptr);
  _wr_lock = false;
}

Logger::Logger
(char const *dsc)
  : _desc(dsc), _stop_dw(false), _dw()
{
  common_constructor();
  start();
}

Logger::Logger
(void)
  : _desc(""), _stop_dw(false), _dw()
{
  common_constructor();
  start();
}

Logger::~Logger
(void)
{
  _stop_dw = true;
  if (_dw.joinable()) _dw.join();

  int j = 0;
  while (_first != nullptr) {
    auto tmp = _first;
    _first = _first->_next;
    delete tmp->_val;
    delete tmp;
    ++j;
  }
  cout << "num deleted " << j << endl;
}

void
Logger::start
(void)
{
  _dw = thread(&Logger::write_to_disk, this);
}

void
Logger::write_to_disk
(void)
{
  bool print_intro = true;
  static int i = 0;

  while (!_stop_dw) {
    usleep(100);
    auto the_first = _first;
    auto the_next = the_first->_next.load();

    if (the_next != nullptr) {
      //
      auto v = the_next->_val;
      the_next->_val = nullptr;
      _first = the_next;

      //
      if (print_intro) {
	cout << _desc << " " << time(nullptr) << " " << the_next->_id;
	++i;
      }

      cout << " " << (*v) << endl;
      print_intro = (v->compare(LogEnd) == 0);

      //
      delete v;
      delete the_first;
    }
  }

  cout << i << endl;
}

Logger&
Logger::operator<<
(const int value) noexcept
{
  auto tmp = new node_t(value);
  while (_wr_lock.exchange(true));
  _last->_next = tmp;
  _last = tmp;
  _wr_lock = false;
  return *this;
}

Logger&
Logger::operator<<
(char const *value) noexcept
{
  static size_t i = 0;
  ++i;
  auto tmp = new node_t(new string(value));
  while (_wr_lock.exchange(true));
  _last->_next = tmp;
  _last = tmp;
  _wr_lock = false;
  //cout << i << endl;
  return *this;
}
