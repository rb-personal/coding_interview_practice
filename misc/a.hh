#ifndef __A_HH__
#define __A_HH__

template<typename t>
t operator+(const t &lhs, const t&rhs)
{
  t ret;
  ret = lhs + rhs;
}

class rc {
  int count;
public:
  rc(void)
    : count(0) {}

  rc(const rc& other)
    : count(other.count) {}

  rc& operator=(const rc& other)
  {
    if (this != &other) {
      count = other.count;
    }
    return *this;
  }

  inline void add(void) { ++count; }
  inline int dec(void)
  {
    if(count >= 1) {
      return --count;
    }
    return 0;
  }
};

template <typename t>
class sp
{
private:
  t *_data;
  rc _rc;

public:
  sp(void)
    : _data(nullptr)
  {
    _rc.add();
  }

  sp(t *data)
    : _data(data)
  {
    _rc.add();
  }

  sp(const sp<t> &other)
    : _data(other._data), _rc(other._rc)
  {
    _rc.add();
  }

  sp& operator=(const sp<t> &other)
  {
    if (this != &other) {
      if (_rc.dec() == 0) {
	delete _data;
      }
      _data = other._data;
      _rc = other._rc;
      _rc.add();
    }
    return *this;
  }

  ~sp(void)
  {
    if (_rc.dec() == 0) {
      delete _data;
    }
  }

  inline t* get() const { return _data; }
  t& operator* (void) { return *_data; }
  t* operator->(void) { return get(); }
};

#endif
