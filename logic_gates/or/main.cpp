#include <systemc>
#include <fmt/format.h>
#include "or.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]){
  const int W = 3;
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };

  sc_vector<sc_signal<bool>> inputs{"In",W};
  sc_signal<bool> output("Out");
  
  Or<W> _or{"Or"};
  _or.inputs(inputs);
  _or.output(output);

  for ( sc_uint<W+1> tmp = 0; tmp <= max2pow(W); ++tmp) {
    for (int i = 0; i < W; ++i) {
      inputs[i].write(tmp[i]);
    }
    run_ns(1);
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, C: {}, Out: {})\n",
                                sc_time_stamp().to_string(),
                                _or.name(),
                                tmp[2],
                                tmp[1],
                                tmp[0],
                                output.read());
    std::cout << s;
  }
  
  run_ns(1);
  return 0;
}
