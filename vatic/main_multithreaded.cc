// **NOTE** Wrote this version before I saw that
// the submission tool wouldn't be compiling with -pthread
// My thinking here was that you could shard work based on instruments,
// keep a separate logger thread that would handle disk writes asynchronously
// I didn't end up finishing this code -- was planning on adding some condition
// variables which would allow the workers to flag the logger that they were done

#if 0
#include "main.hh"
#include <vector>

using namespace std;

void logger(void)
{
  cout << "OPEN_TIME,CLOSE_TIME,SYMBOL,QUANTITY,PNL,OPEN_SIDE,CLOSE_SIDE,OPEN_PRICE,CLOSE_PRICE,OPEN_BID,CLOSE_BID,OPEN_ASK,CLOSE_ASK,OPEN_LIQUDITY,CLOSE_LIQUIDITY" << endl;

  chrono::milliseconds interval(100);

  while (1) {
    if (shared_records_mutex.try_lock()) {
      auto iter = shared_records.begin();
      shared_records_mutex.unlock();
      cout << (*iter) << endl;
    }
    else {
      this_thread::sleep_for(interval);
    }
  }
}

static inline
bool process_instr
(
 const string &instrument,
 const uint8_t tid
)
{
  if (instrument.empty()) return false;
  if (instrument[0]%NUM_THREADS == tid) return true;
  return false;
}

void worker(const char *qpath, const char *tpath, const uint8_t tid)
{
  // file management
  ifstream quotes_file(qpath);
  if (!quotes_file)
    exit(EXIT_FAILURE);
  string q_line;
  getline(quotes_file, q_line);

  ifstream trades_file(tpath);
  if (!trades_file)
    exit(EXIT_FAILURE);
  string t_line;
  getline(quotes_file, t_line);

  // memory
  quote_t quote;
  trade_t trade;

  string quote_instrument;
  string trade_instrument;

  string q_cell;
  string t_cell;

  // sequence maintenance
  bool fresh_quote, fresh_trade;
  bool read_quote = true, quotes_finished = false;
  bool read_trade = true, trades_finished = false;

  while (1) {
    // Should we read a quote
    if (read_quote && !quotes_finished) {
      if (!quotes_file) quotes_finished = true;
      if (!quotes_finished) {
	do {
	  fresh_quote = false;

	  if (!getline(quotes_file, q_line)) {
	    quotes_finished = true;
	    break;
	  }
	  if (q_line.empty()) {
	    quotes_finished = true;
	    break;
	  }

	  stringstream q_linestream(q_line);
	  getline(q_linestream, q_cell, ',');
	  quote.set_time(stoi(q_cell));

	  getline(q_linestream, quote_instrument, ',');
	  if (!process_instr(quote_instrument, tid)) continue;

	  getline(q_linestream, q_cell, ',');
	  quote.set_bid(stof(q_cell));

	  getline(q_linestream, q_cell, ',');
	  quote.set_ask(stof(q_cell));

	  fresh_quote = true;
	} while (!fresh_quote);
      }
    }

    // Should we read a trade
    if (read_trade && !trades_finished) {
      if (!trades_file) trades_finished = true;
      if (!trades_finished) {
	do {
	  fresh_trade = false;

	  if (!getline(trades_file, t_line)) {
	    trades_finished = true;
	    break;
	  }
	  if (t_line.empty()) {
	    trades_finished = true;
	    break;
	  }

	  stringstream t_linestream(t_line);
	  getline(t_linestream, t_cell, ',');
	  trade.set_time(stoi(t_cell));

	  getline(t_linestream, trade_instrument, ',');
	  if (!process_instr(trade_instrument, tid)) continue;

	  getline(t_linestream, t_cell, ',');
	  trade.set_side(t_cell[0]);

	  getline(t_linestream, t_cell, ',');
	  trade.set_price(stof(t_cell));

	  getline(t_linestream, t_cell, ',');
	  trade.set_qty(stoi(t_cell));

	  fresh_trade = true;

	} while (!fresh_trade);
      }
    }

    // Which one should we process?
    if (fresh_quote && fresh_trade) {

      if (quote.get_time() <= trade.get_time()) {
	// quote is at least as old as trade -- process quote
	process_quote
	  (quote_instrument, quote,
	   read_quote, read_trade, fresh_quote, fresh_trade);
      }
      else {
	// trade is older -- process trade
	process_trade(trade_instrument, trade,
		      read_quote, read_trade,
		      fresh_quote, fresh_trade);
      }
    }
    else if (fresh_quote && !fresh_trade) {
      process_quote
	(quote_instrument, quote,
	 read_quote, read_trade, fresh_quote, fresh_trade);
    }
    else if (!fresh_quote && fresh_trade) {
      process_trade(trade_instrument, trade,
		    read_quote, read_trade,
		    fresh_quote, fresh_trade);
    }
    else {
      // quote and trade are both stale
    }

    if (quotes_finished && trades_finished) break;
  }


  // cleanup
  quotes_file.close();
  trades_file.close();
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s [quotes_file] [trades_file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Create the logger thread
  thread logger_thread(logger);

  // Create the workers
  vector<thread> workers;
  for(uint8_t i = 0; i < NUM_THREADS; ++i) workers.push_back(thread(worker, argv[1], argv[2], i));

  // Join
  for (auto &thrd : workers) thrd.join();
  logger_thread.join();

  return 0;
}

void process_quote
(
 const string &instrument,
 const quote_t &q,
 bool &read_quote,
 bool &read_trade,
 bool &fresh_quote,
 bool &fresh_trade
 )
{
  read_quote = true;
  read_trade = false;
  fresh_quote = false;
  book[instrument] = q;
}

void process_trade
(
 const string &instrument,
 const trade_t &t,
 bool &read_quote,
 bool &read_trade,
 bool &fresh_quote,
 bool &fresh_trade
 )
{
  read_quote = false;
  read_trade = true;
  fresh_trade = false;

  if (open_positions.find(instrument) == open_positions.end()) {
    open_positions.insert({instrument, deque<state_t>()});
  }

  auto &i_pos = open_positions[instrument];
  if (i_pos.empty()) {
    // creating a new position
    i_pos.push_back({book[instrument], t});
    return;
  }

  auto &i_pos_f = i_pos.front();
  if ( (t.get_side() == 'S' && i_pos_f.second.get_sqty() < 0) ||
       (t.get_side() == 'B' && i_pos_f.second.get_sqty() > 0) ) {
    // increasing position
    i_pos.push_back({book[instrument], t});
    return;
  }

  // reducing/closing positions
  int32_t qtc = t.get_sqty();
  while (1) {
    if (i_pos.empty()) {
      // reversed the overall position
      if (qtc != 0) {
	i_pos.push_back
	  ({book[instrument],
	      {t.get_time(), t.get_side(), t.get_price(), abs(qtc)}});
      }
      break;
    }

    auto &f = i_pos.front();
    auto &oq = f.first;
    auto &ot = f.second;

    if (abs(qtc) >= ot.get_qty()) {
      // Will exhaust this open trade
      generate_report
	(instrument, f, book[instrument], t, qtc);

      qtc += ot.get_sqty();
      i_pos.pop_front();
      if (qtc == 0) break;
    }
    else if (abs(qtc) < ot.get_qty()) {
      // Won't exhaust
      generate_report
	(instrument, f, book[instrument], t, qtc);

      ot.reduce(abs(qtc));
      qtc = 0;
      break;
    }
  }
}
#endif
