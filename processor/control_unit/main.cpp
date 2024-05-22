#include <systemc>
#include <fmt/format.h>

#include "control_unit.h"

using namespace sc_core;
using namespace sc_dt;

#define ADD_TRACE(X) sc_trace(vcd,X,X.name())

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto run_ps = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_PS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 16;

  sc_clock clk{"clk", sc_time(1.0, sc_time_unit::SC_NS)};
  sc_signal<bool> j{"j"};
  sc_signal<sc_uint<W>> I{"I"};
  sc_signal<sc_uint<W>> A{"A"};

  ControlUnit<W> cu{"cu"};
  cu.clk(clk);
  cu.I(I);
  cu.j(j);
  cu.A(A);

  auto print_cu = [&]() {
    fmt::print("@ {:>6} :: I: 0x{:04x}, j: {:1b}, A: 0x{:04x}\n"
               ,sc_time_stamp().to_string()
               , I.read(), j.read(), A.read());
  };
  
  sc_trace_file *vcd = sc_create_vcd_trace_file("control_unit");
  vcd->set_time_unit(100.0, sc_time_unit::SC_PS);
  sc_trace(vcd,cu.I,cu.I.name());
  sc_trace(vcd,cu.A,cu.A.name());
  sc_trace(vcd,cu.clk,cu.clk.name());
  sc_trace(vcd,cu.j,cu.j.name());
  sc_trace(vcd,cu.m_memory.X,cu.m_memory.X.name());
  ADD_TRACE(cu.m_memory.A);
  ADD_TRACE(cu.m_memory.D);
  ADD_TRACE(cu.m_memory.RAM);

  print_cu();
  
  I = 61415; // 1->A; JMP
  run_ns(1);
  print_cu();

  I = 60066; // 0->A; JEQ
  run_ns(1);
  print_cu();

  I = 61410; // 1->A; JEQ
  run_ns(1);
  print_cu();

  I = 0xec12; // A->D; JEQ
  run_ns(1);
  print_cu();

  I = 0xeca2; // A-1->A;JEQ
  run_ns(1);
  print_cu();
  
  run_ns(1);
  
  sc_close_vcd_trace_file(vcd);
  return 0;
}
