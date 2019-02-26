#include <systemc>
#include "inverter.h"

int sc_main(int argc, char* argv[]) {

  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  
  sc_signal<bool> input{"input"};
  sc_signal<bool> output{"output"};

  Inverter inverter{"Inverter"};
  inverter.input(input);
  inverter.output(output);

  sc_trace_file *vcd = sc_create_vcd_trace_file("inverter");
  vcd->set_time_unit(1.0, sc_time_unit::SC_NS);
  sc_trace(vcd,input,"input");
  sc_trace(vcd,output,"output");

  input.write(false);
  run_ns(1);
  assert(output.read());

  input.write(true);
  run_ns(1);
  assert(!output.read());

  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
