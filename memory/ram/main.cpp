#include <systemc>
#include <fmt/format.h>
#include "ram.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };

  const int W = 16;
  const int W_ADDR=1;

  sc_signal<bool> st{"store"};
  sc_clock clk{"clk",sc_time(1.0,sc_time_unit::SC_NS)};
  sc_signal<sc_uint<W>> X;
  sc_signal<sc_uint<W_ADDR>> addr;
  sc_signal<sc_uint<W>> out;

  Ram<W,W_ADDR> ram{"ram"};
  ram.clk(clk);
  ram.st(st);
  ram.X(X);
  ram.addr(addr);
  ram.output(out);

  sc_trace_file *vcd = sc_create_vcd_trace_file("ram");
  vcd->set_time_unit(100.0, sc_time_unit::SC_PS);
  sc_trace(vcd,st,"store");
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,addr,"address");
  sc_trace(vcd,X,"data");
  sc_trace(vcd,out,"output");
  
  addr.write(0);
  X.write(1234);
  st.write(1);
  run_ns(1);

  addr.write(1);
  X.write(42);
  st.write(1);
  run_ns(1);

  st.write(0);
  addr.write(0);
  X.write(0x5555);
  run_ns(1);
  assert(out.read() == 1234);
  std::cout << fmt::format("@ {:>5} :: {} Addr: {:01b}, Out: 0x{:04x} ({})\n",
                           sc_time_stamp().to_string(), ram.name(),
                           addr.read(), out.read(),out.read());

  addr.write(1);
  X.write(0xAAAA);
  run_ns(1);
  assert(out.read() == 42);
  std::cout << fmt::format("@ {:>5} :: {} Addr: {:01b}, Out: 0x{:04x} ({})\n",
                           sc_time_stamp().to_string(), ram.name(),
                           addr.read(), out.read(),out.read());

  run_ns(1);

  sc_close_vcd_trace_file(vcd);
  return 0;
}
