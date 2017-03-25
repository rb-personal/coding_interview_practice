#include <cstdio>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "lockless_queue.hh"

using namespace std;

lockless_queue_t<int> lq;

int main(int argc, char **argv)
{

  std::atomic<bool> producing;
  producing = true;

  thread producer([&](){
      for (int i = 0; i < 1000; ++i) {
	lq.produce(i);
	cout << "Producing " << i << endl;
      }
    });

  thread consumer([&](){
      int consumed_value;
      bool valid_consume = false;
      do {
	usleep(100);
	valid_consume = (lq.consume(consumed_value));
	if (valid_consume) cout << "Consumed " << consumed_value << endl;
      } while(valid_consume);
    });

  producer.join();
  producing = false;
  consumer.join();

  return 0;
}
