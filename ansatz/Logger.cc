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
  _stop_dw = false;
}

Logger::Logger
(char const *dsc)
  : _desc(dsc), _dw()
{
  common_constructor();
  start();
}

Logger::Logger
(void)
  : _desc(""), _dw()
{
  common_constructor();
  start();
}

Logger::~Logger
(void)
{

  _stop_dw = true;
  if (_dw.joinable()) _dw.join();

  while(print());
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
  _print_intro = true;

  while (!_stop_dw) {
    usleep(100);
    print();
  }
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
  auto tmp = new node_t(new string(value));
  while (_wr_lock.exchange(true));
  _last->_next = tmp;
  _last = tmp;
  _wr_lock = false;
  return *this;
}

bool
Logger::print
(void)
{
  auto the_first = _first;
  auto the_next = the_first->_next.load();
  if (the_next != nullptr) {
    if (_print_intro)
      cout << _desc << " " << time(nullptr) << " " << the_next->_id;
    _print_intro = (the_next->is_log_end());

    if (the_next->_type == node_t::node_type_t::INT)
      printf(" %d", the_next->_val_i);
    else
      printf(" %s", the_next->_val->c_str());

    _first = the_next;
    delete the_first;
    return true;
  }

  return false;
}
