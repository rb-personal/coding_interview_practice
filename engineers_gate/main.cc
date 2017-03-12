#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "util.hh"
#include "message.hh"
#include "line_handler.hh"
#include "book.hh"

#define _PROFILE

using namespace std;

std::atomic<bool> g_line_handler_finished(false);
pc_list_t<message_t> g_shared_queue;

void lh(const char *fpath)
{
  line_handler_t line_handler(fpath);
  custom_timer_t timer("LH");
  message_t msg;

  while (1) {
#ifdef _PROFILE
    timer.click();
#endif
    bool val = line_handler.get_md_message(msg);
#ifdef _PROFILE
    timer.click();
#endif
    if (!val) break;
    g_shared_queue.produce(msg);
  }

  g_line_handler_finished.store(true);
#ifdef _PROFILE
  timer.report();
#endif
}

void bk(void)
{
  unordered_map<string, instr_book_t> book;
  custom_timer_t timer("BK");
  message_t msg;
  uint32_t times_to_check = 10;

  while (1) {
    bool new_work = g_shared_queue.consume(msg);

    if (new_work) {
      if (book.find(msg._ticker) == book.end()) {
	book.insert({msg._ticker, {msg._ticker}});
      }
#ifdef _PROFILE
      timer.click();
#endif
      book[msg._ticker].update(msg);
#ifdef _PROFILE
      timer.click();
#endif
    }

    if (g_line_handler_finished.load() && times_to_check--)
      break;
  }

#ifdef _PROFILE
  timer.report();
#endif
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    TERROR("Usage: % <CSV file path>", argv[0]);
  }

#ifdef _MULTITHREADED
  thread line_handler(lh, argv[1]);
  thread book_builder(bk);

  line_handler.join();
  book_builder.join();
#else
  unordered_map<string, instr_book_t> book;
  line_handler_t line_handler(argv[1]);
  custom_timer_t lh_timer("LH"), bk_timer("BK");
  message_t msg;

  while (1) {
#ifdef _PROFILE
    lh_timer.click();
#endif
    bool val = line_handler.get_md_message(msg);
#ifdef _PROFILE
    lh_timer.click();
#endif
    if (!val) break;

    if (book.find(msg._ticker) == book.end()) {
      book.insert({msg._ticker, {msg._ticker}});
    }
#ifdef _PROFILE
    bk_timer.click();
#endif
    book[msg._ticker].update(msg);
#ifdef _PROFILE
    bk_timer.click();
#endif
  }

#ifdef _PROFILE
  lh_timer.report();
  bk_timer.report();
#endif

#endif

  return EXIT_SUCCESS;
}
