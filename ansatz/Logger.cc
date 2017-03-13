#include "Logger.hh"
#include <cstring>

Logger::Logger(const char *dsc)
  : _desc(dsc),
    _print_intro(true),
    _stop_dw(false),
    _pause_dw(false),
    _dw()
{
  start();
}

Logger::Logger(void)
  : _desc(""),
    _print_intro(true),
    _stop_dw(false),
    _pause_dw(false),
    _dw()
{
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
      lock_guard<mutex> guard(_mutex);
      if(!_records.empty()) {
	auto &r = _records.front();
	cout << r;
	_records.pop_front();
      }
    }

    if (unlikely(_stop_dw)) {
      lock_guard<mutex> guard(_mutex);
      while(!_records.empty()) {
	auto &r = _records.front();
	cout << r;
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
    oss << _desc;
    if (!_desc.empty()) oss << " ";
    oss << "Thread ID: 0x" << hex << this_thread::get_id() << " ";
    //oss << asctime(localtime(&result)) << ": ";
    oss << " Time: " << result << ": ";
    _print_intro = false;
  }
  oss << value << " ";
  _records.push_back(oss.str());

  _pause_dw = false;
  return *this;
}

Logger&
Logger::operator<<(const char *value) {
  _pause_dw = true;
  lock_guard<mutex> guard(_mutex);

  ostringstream oss;
  if (_print_intro) {
    auto result = time(nullptr);
    oss << _desc;
    if (!_desc.empty()) oss << " ";
    oss << "Thread ID: 0x" << hex <<  this_thread::get_id() << " ";
    //oss << asctime(localtime(&result)) << ": ";
    oss << " Time: " << result << ": ";
    _print_intro = false;
  }
  oss << value << " ";
  _records.push_back(oss.str());

  if (0 == strcmp(value, LogEnd)) {
    _print_intro = true;
  }

  _pause_dw = false;
  return *this;
}
