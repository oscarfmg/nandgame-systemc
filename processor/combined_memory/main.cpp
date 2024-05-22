#include <systemc>
#include <fmt/format.h>
#include "combined_memory.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 16;

  sc_signal<bool> st_a{"a"}, st_d{"d"}, st_ram{"a*"};
  sc_signal<sc_uint<W>> A{"A_reg"}, D{"D_reg"}, RAM{"A*_reg"};
  sc_clock clk{"clk",sc_time(1.0,sc_time_unit::SC_NS)};
  sc_signal<sc_uint<W>> X{"X"};

  CombinedMemory<W> cmem{"combined_memory"};
  cmem.st_a(st_a);  cmem.st_d(st_d);  cmem.st_ram(st_ram);
  cmem.A(A);  cmem.D(D);  cmem.RAM(RAM);
  cmem.clk(clk);
  cmem.X(X);

  sc_trace_file *vcd = sc_create_vcd_trace_file("combined_memory");
  vcd->set_time_unit(100.0, sc_time_unit::SC_PS);
  sc_trace(vcd,st_a,"a");
  sc_trace(vcd,st_d,"d");
  sc_trace(vcd,st_ram,"a*");
  sc_trace(vcd,X,"X");
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,A,"A");
  sc_trace(vcd,D,"D");
  sc_trace(vcd,RAM,"A*");
  
  auto print_cmem = [&]() {
    std::cout << fmt::format("@ {:>5} :: (a: {:1b}, d: {:1b}, a*: {:1b}), X: 0x{:04x}, "
                             "(A: 0x{:04x}, D: 0x{:04x}, A*: 0x{:04x})\n",
                             sc_time_stamp().to_string(),
                             st_a.read(), st_d.read(), st_ram.read(), X.read(),
                             A.read(), D.read(), RAM.read());
  };

  auto write_ram = [&](sc_uint<W> addr, sc_uint<W> x) {
    st_a = 1; st_d = 0; st_ram = 0;
    X = addr;
    run_ns(1);

    st_a = 0; st_ram = 1;
    X = x;
    run_ns(1);
  };

  print_cmem();
  
  write_ram(0xdead,0xbeef);
  print_cmem();

  st_a = 1; st_d = 1; st_ram = 0;
  X = 0xa0a0;
  run_ns(1); print_cmem();

  st_a = 0; st_d = 0; st_ram = 1;
  X = 0x5a5a;
  run_ns(1); print_cmem();

  st_d = 1; st_ram = 0;
  X = 0xbaad;
  run_ns(1); print_cmem();

  st_a = 1; st_d = 0;
  X = 0xdead;
  run_ns(1); print_cmem(); //Write to A

  run_ns(1); print_cmem(); //Clk to take A into RAM

  assert(A.read() == 0xdead);
  assert(D.read() == 0xbaad);
  assert(RAM.read() == 0xbeef);

  run_ns(1);

  sc_close_vcd_trace_file(vcd);
  return 0;
}
