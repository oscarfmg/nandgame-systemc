#include <systemc>
#include <fmt/format.h>
#include "dff.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto run_ps = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_PS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 1;
  
  sc_signal<bool> store{"store"};
  sc_signal<sc_uint<W>> data{"data"};
  sc_signal<sc_uint<W>> output{"output"};
  sc_clock clk{"clk", sc_time(1.0,sc_time_unit::SC_NS)};

  Dff<W> dff{"dff"};
  dff.store(store);
  dff.data(data);
  dff.output(output);
  dff.clk(clk);

  sc_trace_file *vcd = sc_create_vcd_trace_file("dff");
  vcd->set_time_unit(1.0, sc_time_unit::SC_PS);
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,store,"store");
  sc_trace(vcd,data,"data");
  sc_trace(vcd,output,"output");

  run_ns(1);

  run_ns(1);
  data.write(1);
  store.write(0);
  run_ns(1);

  store.write(1);
  run_ns(1);

  run_ps(10);
  data.write(0);
  run_ns(1);

  store.write(0);
  run_ps(1);
  data.write(1);
  run_ns(1);

  data.write(0);
  run_ps(10);
  data.write(1);
  run_ps(10);
  data.write(0);
  store.write(1);
  run_ps(10);
  data.write(1);
  run_ps(100);
  data.write(0);
  run_ps(200);
  data.write(1);
  run_ps(300);
  data.write(0);
  run_ps(400);
  data.write(1);
  run_ps(500);
  data.write(0);
  run_ps(600);
  data.write(1);
  run_ps(700);
  data.write(0);
  store.write(0);
  run_ps(800);
  store.write(1);
  run_ps(100);
  data.write(1);
  run_ps(900);
  data.write(0);
  
  run_ns(1);
  
  sc_close_vcd_trace_file(vcd);
  return 0;
}
