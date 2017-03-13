#include "Logger.h"
Logger::Logger(char *dsc)
  : _desc(dsc),
    _print_intro(true),
    _stop_dw(false),
    _pause_dw(false),
    _dw()
{
  (*this) << _desc;
}

Logger::Logger(void)
  : _desc(""),
    _print_intro(true),
    _stop_dw(false),
    _pause_dw(false),
    _dw()
{}

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
      lock_guard<mutex> guard(_mutex);
      if(!_records.empty()) {
	auto &r = _records.front();
	cout << r;
	if (r.compare(LogEnd) == 0) cout << _desc;
	_records.pop_front();
      }
    }

    if (unlikely(_stop_dw)) {
      lock_guard<mutex> guard(_mutex);
      while(!_records.empty()) {
	auto &r = _records.front();
	cout << r;
	if (r.compare(LogEnd) == 0) cout << _desc;
	_records.pop_front();
      }
      break;
    }
  }
}

Logger&
Logger::operator<<(int value) {
  _pause_dw = true;
  lock_guard<mutex> guard(_mutex);

  ostringstream oss;
  if (_print_intro) {
    auto result = time(nullptr);
    oss << "_desc "
	<< this_thread::get_id()
	<< " "
	<< asctime(localtime(&result))
	<< ": ";
    _print_intro = false;
  }
  oss << value << " ";
  _records.push_back(oss.str());

  _pause_dw = false;
  return *this;
}

Logger&
Logger::operator<<(string &value) {
  _pause_dw = true;
  lock_guard<mutex> guard(_mutex);

  ostringstream oss;
  if (_print_intro) {
    auto result = time(nullptr);
    oss << "_desc "
	<< this_thread::get_id()
	<< " "
	<< asctime(localtime(&result))
	<< ": ";
    _print_intro = false;
  }
  oss << value << " ";
  _records.push_back(oss.str());

  if (0 == value.compare(LogEnd)) _print_intro = true;

  _pause_dw = false;
  return *this;
}
