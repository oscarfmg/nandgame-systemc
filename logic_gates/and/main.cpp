#include <systemc>
#include <fmt/format.h>
#include "and.h"

int sc_main(int argc, char* argv[]) {  
  const int W = 3;
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  
  sc_vector<sc_signal<bool>> _inputs{"Ins",W};
  sc_signal<bool> _out{"Out"};
  
  And<W> _and{"And"};
  _and.inputs(_inputs);
  _and.output(_out);

  for (sc_dt::sc_uint<W+1> i=0; i <= max2pow(W); ++i) {
    for (int x = 0; x < W; ++x) {
      _inputs[W-x-1].write(i[x]);
    }
    run_ns(1);
    std::string s = fmt::format("@ {:>4} :: {} (A: {}, B: {}, C: {}, Out: {:>5})\n",
                                sc_time_stamp().to_string(),
                                _and.name(),
                                i[2],
                                i[1],
                                i[0],
                                _out.read());
    std::cout << s;
  }

  run_ns(1);
  return 0;
}
