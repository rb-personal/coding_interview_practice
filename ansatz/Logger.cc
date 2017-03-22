#include "Logger.hh"
#include <cstring>

void
Logger::common_constructor(void)
{
  _print_intro = true;
  _stop_dw = false;
  _pause_dw = false;
}

Logger::Logger(char const *dsc)
  : _desc(dsc),
    _dw()
{
  common_constructor();
  start();
}

Logger::Logger(void)
  : _desc(""),
    _dw()
{
  common_constructor();
  start();
}

Logger::~Logger(void)
{
  _stop_dw = true;
  if (_dw.joinable()) _dw.join();
}

void
Logger::start(void)
{
  _dw = thread(&Logger::write_to_disk, this);
}

void
Logger::write_to_disk(void)
{
  while (1) {
    if (!_pause_dw) {
      lock_guard<recursive_mutex> guard(_mutex);
      print();
    }

    if (unlikely(_stop_dw)) {
      lock_guard<recursive_mutex> guard(_mutex);
      while(!_records.empty()) print();
      break;
    }
  }
}

Logger&
Logger::operator<<(const int value) noexcept
{
  _pause_dw = true;
  _oss << value;
  {
    lock_guard<recursive_mutex> guard(_mutex);
    _records.push_back({
	this_thread::get_id(),
	  time(nullptr),
	  _oss.str()});
  }
  _oss.str("");
  _oss.clear();
  return *this;
}

Logger&
Logger::operator<<(char const *value) noexcept
{
  _pause_dw = true;
  {
    lock_guard<recursive_mutex> guard(_mutex);
    _records.push_back({
	this_thread::get_id(),
	  time(nullptr),
	  value});
  }
  if (0 == strcmp(value, LogEnd))
    _pause_dw = false;
  return *this;
}
