#include "Logger.hh"
#include <vector>

using namespace std;

void thread_func(void)
{
  for (int i = 0; i < 5; ++i)
    LogVERBOSE << "BLAH" << rand() << "FOO" << LogEnd;
}

int main(int argc, char **argv)
{
  // const unsigned NUM_THREADS = 1000;
  // vector<thread> threads;
  // for (auto i = 0u; i < NUM_THREADS; ++i) {
  //   threads.push_back(thread(thread_func));
  // }

  // for(auto &t : threads)
  //   t.join();

  const size_t NUM = 1000;
  for (int i = 0; i < NUM; ++i)
    LogDEBUG << "Asfasdf" << LogEnd;
  return 0;
}
