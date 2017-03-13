#include "Logger.hh"
#include <vector>

void thread_func(void)
{
  for (int i = 0; i < 10; ++i)
    LogVERBOSE << "BLAH" << rand() << "FOO" << LogEnd;
}

int main(int argc, char **argv)
{
  const unsigned NUM_THREADS = 10;
  vector<thread> threads;
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    threads.push_back(thread(thread_func));
  }

  for(auto &t : threads)
    t.join();

  //Log << "Hello" << 5 << LogEnd;
  //LogINFO << "BLAH" << rand() << "FOO" << LogEnd;
  // LogDEBUG << 10 << LogEnd;
  return 0;
}
