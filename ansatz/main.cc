#include "Logger.hh"
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
  // thread producer([&](){
  //     for (int i = 0; i < 100; ++i)
  //	LogDEBUG << "blah " << i << LogEnd;
  //   });
  // producer.join();

  const unsigned NUM_THREADS = 10;
  vector<thread> threads;
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    threads.push_back(thread([&](){
	  for (int i = 0; i < 5; ++i)
	    LogVERBOSE << "BLAH" << rand() << "FOO" << LogEnd;
	}));;
  }

  for(auto &t : threads)
    t.join();

  return 0;
}
