#include <systemc>
#include <fmt/format.h>
#include "counter.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 4;

  sc_signal<bool> st{"st"};
  sc_signal<sc_uint<W>> X{"X"};
  sc_clock clk{"clk", sc_time(1.0,sc_time_unit::SC_NS)};
  sc_signal<sc_uint<W>> out{"out"};

  Counter<W> cnt{"counter"};
  cnt.st(st);
  cnt.X(X);
  cnt.clk(clk);
  cnt.output(out);

  
  sc_trace_file *vcd = sc_create_vcd_trace_file("counter");
  vcd->set_time_unit(1.0, sc_time_unit::SC_PS);
  sc_trace(vcd,st,"st");
  sc_trace(vcd,X,"X");
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,out,"output");

  
  st.write(1);
  X.write(0);
  run_ns(1);

  st.write(0);
  
  sc_uint<W+1> tmp=0;
  for (;tmp <= max2pow(W); ++tmp) {
    assert(out.read() == tmp);

    std::cout << fmt::format("@ {:>5} :: {} St: {:1b}, X: {:04b}, O: {:04b}\n",
                             sc_time_stamp().to_string(), cnt.name(),
                             st.read(), X.read(), out.read());
    run_ns(1);
  }

  st.write(1);
  X.write(0x5);
  run_ns(2);

  st.write(0);

  for(tmp=0;tmp <= max2pow(W); ++tmp) {
    assert(out.read() == (tmp + 5) % 16);
    std::cout << fmt::format("@ {:>5} :: {} St: {:1b}, X: {:04b}, O: {:04b}\n",
                             sc_time_stamp().to_string(), cnt.name(),
                             st.read(), X.read(), out.read());
    run_ns(1);
  }
  
  run_ns(1);
  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
