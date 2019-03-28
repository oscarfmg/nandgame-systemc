#include <fstream>
#include <systemc>
#include <fmt/format.h>
#include "program_engine.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 16;

  sc_signal<bool> j{"j"};
  sc_signal<sc_uint<W>> addr;
  sc_signal<sc_uint<W>> output;
  sc_clock clk{"clk", sc_time(1.0, sc_time_unit::SC_NS)};

  ProgramEngine<W> pe{"program_engine","program.txt"};
  pe.j(j);
  pe.addr(addr);
  pe.clk(clk);
  pe.output(output);

  sc_trace_file *vcd = sc_create_vcd_trace_file("program_engine");
  vcd->set_time_unit(100.0, sc_time_unit::SC_PS);
  sc_trace(vcd,j,"j");
  sc_trace(vcd,addr,"addr");
  sc_trace(vcd,clk,"clk");
  sc_trace(vcd,output,"output");

  
  std::vector<sc_uint<W>> data;
  int _data;
  std::ifstream infile{"program.txt"};
  if ( !infile.is_open() ) {
    return 1;
  }
  while ( infile >> std::hex >> _data) {
    data.push_back(_data);
  }

  auto print_pe = [&](){
    fmt::print("@ {:>5} :: (j: {:1b}) (Addr: 0x{:04x}) (Out: 0x{:04x})\n",
               sc_time_stamp().to_string(),
               j.read(), addr.read(), output.read());
  };

  j=1; addr=0;
  run_ns(1);
  j=0;
  
  for (int i=0; i < data.size(); ++i) {
    run_ns(1);
    assert(output.read() == data[i]);
    print_pe();
  }

  j=1; addr=5;
  run_ns(1);
  assert(output.read() == 0);
  print_pe();

  j=0;
  run_ns(1);
  assert(output.read() == 0xe3a0);
  print_pe();
  
  run_ns(1);
  sc_close_vcd_trace_file(vcd);
  return 0;
}
