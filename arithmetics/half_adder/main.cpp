#include <systemc>
#include <fmt/format.h>
#include "half_adder.h"

using namespace sc_core;

int sc_main(int argc, char* argv[]) {

  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };

  sc_signal<bool> a{"a"};
  sc_signal<bool> b{"b"};
  sc_signal<bool> l{"l"};
  sc_signal<bool> h{"h"};

  HalfAdder ha{"half_adder"};
  ha.a(a);
  ha.b(b);
  ha.l(l);
  ha.h(h);

  sc_trace_file *vcd = sc_create_vcd_trace_file("half_adder");
  vcd->set_time_unit(1.0, sc_time_unit::SC_NS);
  sc_trace(vcd,a,"A");
  sc_trace(vcd,b,"B");
  sc_trace(vcd,h,"H");
  sc_trace(vcd,l,"L");

  auto print_ha = [&](){
    std::cout << fmt::format("@ {:>5} :: {} (A: {}, B: {}, H: {}, L: {})\n",
                sc_time_stamp().to_string(),
                ha.name(),
                static_cast<int>(a.read()),
                static_cast<int>(b.read()),
                static_cast<int>(h.read()),
                static_cast<int>(l.read()));
  };

  a.write(false); b.write(false);  run_ns(1);  print_ha();
  a.write(false); b.write( true);  run_ns(1);  print_ha();
  a.write( true); b.write(false);  run_ns(1);  print_ha();
  a.write( true); b.write( true);  run_ns(1);  print_ha();

  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
