#include <systemc>
#include <fmt/format.h>
#include "is_negative.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W=4;

  sc_signal<sc_uint<W>> in{"input"};
  sc_signal<bool> out{"output"};

 
  IsNegative<W> is_negative{"is_negative"};
  is_negative.input(in);
  is_negative.output(out);

  sc_int<W> max = 0;
  max[W-1] = 1;
  for (sc_int<W+1> tmp = max; tmp <= max2pow(W) / 2; ++tmp) {
    in.write(static_cast<sc_uint<W>>(tmp));
    run_ns(1);
    std::cout << fmt::format("@ {:>5} :: {} - (In: {:04b} - {: 2}) (Out: {})\n",
                             sc_time_stamp().to_string(),
                             is_negative.name(),
                             static_cast<sc_uint<W>>(tmp),
                             tmp,
                             is_negative.output.read());
  }

  run_ns(1);
  
  return 0;
}
