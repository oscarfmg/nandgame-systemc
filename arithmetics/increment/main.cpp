#include <systemc>
#include <fmt/format.h>
#include "increment.h"
#include <sstream>

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {

  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
//  const int W = 16;
  const int W = 5;

  
  sc_signal<sc_uint<W>> input{"input"};
  sc_signal<sc_uint<W>> output{"output"};
  Increment<W> increment{"Increment"};
  increment.input(input);
  increment.output(output);

  std::stringstream ss;
  
  for ( sc_uint<W+1> i=0; i <= max2pow(W); ++i) {
    input.write(i);
    run_ns(1);
    ss << fmt::format("@ {:>5} :: {} - In: {}, Out: {}\n",
                             sc_time_stamp().to_string(),
                             increment.name(),
                             input.read(),
                             output.read());
  }

  std::cout << ss.str();
  
  return 0;
}
