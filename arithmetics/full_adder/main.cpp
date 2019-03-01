#include <systemc>
#include <fmt/format.h>
#include "full_adder.h"

using namespace sc_core;

int sc_main(int argc, char* argv[]) {

  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };

  sc_signal<bool> a{"a"};
  sc_signal<bool> b{"b"};
  sc_signal<bool> c{"c"};
  sc_signal<bool> l{"l"};
  sc_signal<bool> h{"h"};

  FullAdder fa{"full_adder"};
  fa.a(a);
  fa.b(b);
  fa.c(c);
  fa.l(l);
  fa.h(h);

  sc_trace_file *vcd = sc_create_vcd_trace_file("full_adder");
  vcd->set_time_unit(1.0, sc_time_unit::SC_NS);
  sc_trace(vcd,a,"A");
  sc_trace(vcd,b,"B");
  sc_trace(vcd,c,"C");
  sc_trace(vcd,h,"H");
  sc_trace(vcd,l,"L");

  auto print_fa = [&](){
    std::cout << fmt::format("@ {:>5} :: {} (A: {}, B: {}, C: {}, H: {}, L: {})\n",
                sc_time_stamp().to_string(),
                fa.name(),
                static_cast<int>(a.read()),
                static_cast<int>(b.read()),
                static_cast<int>(c.read()),
                static_cast<int>(h.read()),
                static_cast<int>(l.read()));
  };

  a.write(0); b.write(0); c.write(0);  run_ns(1);  print_fa();
  a.write(0); b.write(0); c.write(1);  run_ns(1);  print_fa();
  a.write(0); b.write(1); c.write(0);  run_ns(1);  print_fa();
  a.write(0); b.write(1); c.write(1);  run_ns(1);  print_fa();
  a.write(1); b.write(0); c.write(0);  run_ns(1);  print_fa();
  a.write(1); b.write(0); c.write(1);  run_ns(1);  print_fa();
  a.write(1); b.write(1); c.write(0);  run_ns(1);  print_fa();
  a.write(1); b.write(1); c.write(1);  run_ns(1);  print_fa();

  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
