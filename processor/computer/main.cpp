#include <systemc>
#include <fmt/format.h>
#include "computer.h"

using namespace sc_core;
using namespace sc_dt;

#define ADD_TRACE(X) sc_trace(vcd,X,X.name())

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 16;

  sc_clock clk{"clk",sc_time(1.0,sc_time_unit::SC_NS)};
  Computer<W> computer{"computer","program.txt"};
  computer.clk(clk);
  
  sc_trace_file *vcd = sc_create_vcd_trace_file("computer");
  vcd->set_time_unit(100, sc_time_unit::SC_PS);
  ADD_TRACE(computer.clk);
  ADD_TRACE(computer.m_cu.A);
  ADD_TRACE(computer.m_cu.j);
  ADD_TRACE(computer.m_pe.output);
  ADD_TRACE(computer.m_cu.m_memory.D);
  ADD_TRACE(computer.m_cu.m_memory.RAM);

  for(int i=0; i<200; ++i) {
    run_ns(1);
  }
  run_ns(1);

  sc_close_vcd_trace_file(vcd);

  auto print_mem = [&](int max) {
    for(int i=0; i < max; ++i) {
      fmt::print(" RAM[{}]: {}\n",i,computer.m_cu.m_memory.m_ram.m_data[i]);
    }
  };

  print_mem(5);
  return 0;
}
