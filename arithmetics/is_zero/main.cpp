#include <systemc>
#include <fmt/format.h>
#include "is_zero.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 4;
  
  sc_signal<sc_uint<W>> in{"in"};
  sc_signal<bool> out{"out"};

  IsZero<W> is_zero{"is_zero"};
  is_zero.input(in);
  is_zero.output(out);

  for (sc_uint<W+1> tmp = 0; tmp <= max2pow(W)+1; ++tmp) {
    in.write(tmp);
    run_ns(1);

    std::cout << fmt::format("@ {:>5} :: {} - In: {:04b}, Out: {}\n",
                             sc_time_stamp().to_string(),
                             is_zero.name(),
                             in.read(),
                             out.read());
  }
  
  return 0;
}
