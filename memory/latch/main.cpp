#include <systemc>
#include <fmt/format.h>
#include "latch.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };

  const int W = 4;
  
  sc_signal<bool> store{"store"};
  sc_signal<sc_uint<W>> data{"data"};
  sc_signal<sc_uint<W>> out{"out"};

  Latch<W> latch{"latch"};
  latch.store(store);
  latch.data(data);
  latch.output(out);

  sc_trace_file *vcd = sc_create_vcd_trace_file("latch");
  vcd->set_time_unit(1.0, sc_time_unit::SC_NS);
  sc_trace(vcd,data,"data");
  sc_trace(vcd,store,"store");
  sc_trace(vcd,out,"output");
 

  auto print_l = [&]()
  {
    std::cout << fmt::format("@ {:>5} :: {} St: {:1b}, D: {:04b}, O: {:04b}\n",
                             sc_time_stamp().to_string(), latch.name(),
                             store.read(), data.read(), out.read());
  };
  
                   data.write(9);  run_ns(1);  print_l();
  store.write(1);                  run_ns(1);  print_l();
  store.write(0);  data.write(6);  run_ns(1);  print_l();
  store.write(1);                  run_ns(1);  print_l();
  
  run_ns(1);

  sc_close_vcd_trace_file(vcd);
  
  return 0;
}
