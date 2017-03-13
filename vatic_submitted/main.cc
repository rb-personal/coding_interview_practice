#include "main.hh"
#include <vector>
#include <algorithm>

using namespace std;

int main
(int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s [quotes_file] [trades_file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("OPEN_TIME,CLOSE_TIME,SYMBOL,QUANTITY,PNL,OPEN_SIDE,CLOSE_SIDE,OPEN_PRICE,CLOSE_PRICE,OPEN_BID,CLOSE_BID,OPEN_ASK,CLOSE_ASK,OPEN_LIQUIDITY,CLOSE_LIQUIDITY\n");

  // file management
  ifstream quotes_file(argv[1]);
  if (!quotes_file)
    exit(EXIT_FAILURE);
  string q_line;
  getline(quotes_file, q_line);

  ifstream trades_file(argv[2]);
  if (!trades_file)
    exit(EXIT_FAILURE);
  string t_line;
  getline(trades_file, t_line);

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

  deque<string> q_tokens, t_tokens;

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

	  // Design Note: Custom tokenizer is faster
	  size_t start = 0;
	  size_t end = q_line.find_first_of(',');
	  q_tokens.clear();
	  while (end <= string::npos) {
	    q_tokens.emplace_back(q_line.substr(start, end-start));
	    if (end == string::npos) break;
	    start = end+1;
	    end = q_line.find_first_of(',', start);
	  }
	  quote.set_time(stoi(q_tokens[0]));
	  quote.set_bid(stof(q_tokens[2]));
	  quote.set_ask(stof(q_tokens[3]));
	  quote_instrument = q_tokens[1];

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

	  size_t start = 0;
	  size_t end = t_line.find_first_of(',');
	  t_tokens.clear();
	  while (end <= string::npos) {
	    t_tokens.emplace_back(t_line.substr(start, end-start));
	    if (end == string::npos) break;
	    start = end+1;
	    end = t_line.find_first_of(',', start);
	  }
	  trade.set_time(stoi(t_tokens[0]));
	  trade.set_side(t_tokens[2][0]);
	  trade.set_price(stof(t_tokens[3]));
	  trade.set_qty(stoi(t_tokens[4]));

	  trade_instrument = t_tokens[1];

	  fresh_trade = true;
	} while (!fresh_trade);
      }
    }

    //
    if (fresh_quote && fresh_trade) {
      if (quote.get_time() <= trade.get_time()) {
	process_quote
	  (quote_instrument, quote,
	   read_quote, read_trade, fresh_quote, fresh_trade);
      }
      else {
	process_trade
	  (trade_instrument, trade,
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
      process_trade
	(trade_instrument, trade,
	 read_quote, read_trade,
	 fresh_quote, fresh_trade);
    }
    else {
      // quote and trade are both stale
      break;
    }

    if (quotes_finished && trades_finished) break;
  }

  // cleanup
  quotes_file.close();
  trades_file.close();
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

  if (open_quotes.find(instrument) == open_quotes.end()) {
    open_quotes.insert({instrument, deque<quote_t>()});
  }
  if (open_trades.find(instrument) == open_trades.end()) {
    open_trades.insert({instrument, deque<trade_t>()});
  }


  auto &i_oqs = open_quotes[instrument];
  auto &i_ots = open_trades[instrument];
  auto &q = book[instrument];

  if (i_oqs.empty()) {
    // creating a new position
    i_oqs.push_back(q);
    i_ots.push_back(t);
    return;
  }

  auto &i_ots_f = i_ots.front();
  if (t.get_side() == i_ots_f.get_side()) {
    // increasing position
    i_oqs.push_back(q);
    i_ots.push_back(t);
    return;
  }

  // reducing/closing positions
  int32_t qtc = t.get_sqty();
  while (qtc != 0) {
    if (i_ots.empty()) {
      // reversed the overall position
      i_oqs.push_back(q);
      i_ots.push_back({t.get_time(), t.get_side(), t.get_price(), abs(qtc)});
      break;
    }

    auto &oq = i_oqs.front();
    auto &ot = i_ots.front();

    if (abs(qtc) >= ot.get_qty()) {
      // Will exhaust this open trade
      generate_report
	(instrument, oq, ot, q, t, -ot.get_sqty());

      qtc += ot.get_sqty();

      i_oqs.pop_front();
      i_ots.pop_front();

      if (qtc == 0) break;
    }
    else if (abs(qtc) < ot.get_qty()) {
      // Won't exhaust
      generate_report
	(instrument, oq, ot, q, t, qtc);
      ot.reduce(abs(qtc));
      qtc = 0;
      break;
    }
  }
}
