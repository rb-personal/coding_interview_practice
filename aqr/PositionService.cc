#include "PositionService.hh"

PositionService::PositionService
(const char *fills, const char *prices)
{
  _ff = fopen(fills, "r");
  if (!_ff) {
    fprintf(stderr, "Error opening fills file\n");
    exit(EXIT_FAILURE);
  }

  _pf = fopen(prices, "r");
  if (!_pf) {
    fprintf(stderr, "Error opening prices file\n");
    fclose(_ff);
    exit(EXIT_FAILURE);
  }

  read_fill = read_price = true;
  fills_finished = prices_finished = false;
}

PositionService::~PositionService
(void)
{
  fclose(_ff);
  fclose(_pf);
}

void
PositionService::process
(void)
{
  while (!fills_finished || !prices_finished) {
    if (read_fill)  read_fill_record();
    if (read_price) read_price_record();

    if (fresh_fill && fresh_price) {
      if (fill_read_data.when <= price_read_data.when)
	process_fill();
      else
	process_price();
    }
    else if (fresh_fill && !fresh_price) {
      process_fill();
    }
    else if (!fresh_fill && fresh_price) {
      process_price();
    }
    else {
      break;
    }

    if (fills_finished && prices_finished) break;
  }
}

bool
PositionService::read_fill_record
(void)
{
  fresh_fill = false;

  if (!_ff) {
    fills_finished = true;
    return false;
  }

  // TODO: magic number
  fill_read_data.qty = 0;
  while (fill_read_data.qty == 0) {
    if ( 6 != fscanf(_ff, "%c %ld %s %lf %d %c\n",
		     &fill_read_data.type,
		     &fill_read_data.when,
		     fill_read_data.which,
		     &fill_read_data.price,
		     &fill_read_data.qty,
		     &fill_read_data.dir_c) ) {
      fills_finished = true;
      return false;
    }
  }

#ifdef __DEBUG
  printf("%c %ld %s %lf %d %c\n",
	 fill_read_data.type,
	 fill_read_data.when,
	 fill_read_data.which,
	 fill_read_data.price,
	 fill_read_data.qty,
	 fill_read_data.dir_c);
#endif

  read_fill = false;
  return (fresh_fill = true);
}

bool
PositionService::read_price_record
(void)
{
  fresh_price = false;

  if (!_pf) {
    prices_finished = true;
    return false;
  }

  // TODO: magic number
  if( 4 != fscanf(_pf, "%c %ld %s %lf\n",
		  &price_read_data.type,
		  &price_read_data.when,
		  price_read_data.which,
		  &price_read_data.price) ) {

    prices_finished = true;
    return false;
  }

#ifdef __DEBUG
  printf("%c %ld %s %lf\n",
	 price_read_data.type,
	 price_read_data.when,
	 price_read_data.which,
	 price_read_data.price);
#endif

  read_price = false;
  return (fresh_price = true);
}

void
PositionService::process_fill
(void)
{
  // Update read logic
  fresh_fill = false;
  read_fill  = true;

  std::string w(fill_read_data.which);
  int sqty = fill_read_data.qty;
  if (fill_read_data.dir_c == 'S') sqty = -sqty;

  if (curr_positions.find(w) == curr_positions.end()) {
    curr_positions.insert({
	w, {0., {{fill_read_data.price, sqty}}}});
#ifdef __DEBUG
    print_current_positions();
#endif
    return;
  }

  auto &pnl = curr_positions[w].first;
  auto &q   = curr_positions[w].second;

  if( (q.front().sqty < 0 && sqty < 0) ||
      (q.front().sqty > 0 && sqty > 0) ) {
    q.push_back({fill_read_data.price, sqty});
#ifdef __DEBUG
    print_current_positions();
#endif
    return;
  }

  while (fill_read_data.qty > 0 && !q.empty()) {
    auto &qf = q.front();
    if (fill_read_data.qty <= abs(qf.sqty)) {
      pnl -= (static_cast<double>(sqty)*fill_read_data.price + static_cast<double>(-sqty)*qf.price);
      qf.sqty += sqty;
      sqty = 0;
      fill_read_data.qty = 0;
      if (qf.sqty == 0) q.pop_front();
    }
    else {
      pnl -= (static_cast<double>(qf.sqty)*qf.price + static_cast<double>(-qf.sqty)*fill_read_data.price);
      sqty += qf.sqty;
      fill_read_data.qty = abs(sqty);
      q.pop_front();
    }
  }

  if (fill_read_data.qty > 0) {
    q.push_back({fill_read_data.price, sqty});
  }

#ifdef __DEBUG
  print_current_positions();
#endif
}

void
PositionService::process_price
(void)
{
  // Update read logic
  fresh_price = false;
  read_price  = true;

  std::string pw(price_read_data.which);

  if (curr_positions.find(pw) == curr_positions.end()) {
    printf("PNL %ld %s 0 0.00\n",
	   price_read_data.when, pw.c_str());
    return;
  }

#ifdef __DEBUG
  print_current_positions();
#endif

  auto pnl = curr_positions[pw].first;
  auto sp = 0;
  for_each(curr_positions[pw].second.begin(),
	   curr_positions[pw].second.end(),
	   [&](const position_t &pos){
	     sp -= pos.sqty;
	     pnl += (price_read_data.price - pos.price)*static_cast<double>(pos.sqty);
	   });
  printf("PNL %ld %s %d %.2lf\n",
	 price_read_data.when, price_read_data.which, sp, pnl);
}

void
PositionService::print_current_positions
(void)
{
  printf("---\n");
  for (const auto &cp : curr_positions) {
    printf("%s: [PNL: %.2f]\n", cp.first.c_str(), cp.second.first);
    if (cp.second.second.empty()) printf("No open positions\n");
    for (const auto &p : cp.second.second) {
      printf("%d @ %.2f\n", p.sqty, p.price);
    }
  }
  printf("---\n");
}
