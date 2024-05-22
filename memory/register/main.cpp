#include <systemc>
#include <fmt/format.h>
#include "register.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto run_ps = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_PS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 4;
  
  sc_signal<bool> store{"store"};
  sc_signal<sc_uint<W>> data{"data"};
  sc_signal<sc_uint<W>> output{"output"};
  sc_clock clk{"clk", sc_time(1.0,sc_time_unit::SC_NS)};

  Register<W> reg{"reg"};
  reg.store(store);
  reg.data(data);
  reg.output(output);
  reg.clk(clk);

  sc_trace_file *vcd = sc_create_vcd_trace_file("register");
  vcd->set_time_unit(1.0, sc_time_unit::SC_PS);
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,store,"store");
  sc_trace(vcd,data,"data");
  sc_trace(vcd,output,"output");

  run_ns(1);

  run_ns(1);
  data.write(0xE);
  store.write(0);
  run_ns(1);

  store.write(1);
  run_ns(1);

  run_ps(10);
  data.write(0xD);
  run_ns(1);

  store.write(0);
  run_ps(1);
  data.write(0xC);
  run_ns(1);

  data.write(0xB);
  run_ps(10);
  data.write(0xA);
  run_ps(10);
  data.write(0x9);
  store.write(1);
  run_ps(10);
  data.write(0x8);
  run_ps(100);
  data.write(0x7);
  run_ps(200);
  data.write(0x6);
  run_ps(300);
  data.write(0x5);
  run_ps(400);
  data.write(0x4);
  run_ps(500);
  data.write(0x3);
  run_ps(600);
  data.write(0x2);
  run_ps(700);
  data.write(0x1);
  store.write(0);
  run_ps(800);
  store.write(1);
  run_ps(100);
  data.write(0xF);
  run_ps(900);
  data.write(0);
  
  run_ns(1);
  
  sc_close_vcd_trace_file(vcd);
  return 0;
}
