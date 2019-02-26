#include <systemc>
#include <fmt/format.h>
#include "nand.h"

int sc_main(int argc, char* argv[]) {

  auto max2pow = [](int x){return (1<<x)-1;};
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  const int W = 3;

  Nand<3> nand{"NAND[3]"};
  sc_signal<bool> signal_a;
  sc_signal<bool> signal_b;
  sc_signal<bool> signal_c;
  sc_signal<bool> out;

  nand.inputs[0].bind(signal_a);
  nand.inputs[1].bind(signal_b);
  nand.inputs[2].bind(signal_c);
  nand.output.bind(out);
  
  sc_trace_file *vcd = sc_create_vcd_trace_file("nand");
  vcd->set_time_unit(1.0,sc_time_unit::SC_NS);
  sc_trace(vcd,signal_a,"A");
  sc_trace(vcd,signal_b,"B");
  sc_trace(vcd,signal_c,"C");
  sc_trace(vcd,out,"Out");
  
  for (int a = 0; a <= 1; ++a) {
    signal_a.write(a);
    for (int b = 0; b <= 1; ++b) {
      signal_b.write(b);
      for (int c = 0; c <= 1; ++c) {
        signal_c.write(c);
        run_ns(1);
        std::string s = fmt::format("A: {}, B: {}, C: {}, Out: {}\n",a,b,c,out.read());
        std::cout << s;
      }
    }
  }

  run_ns(1);
  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
