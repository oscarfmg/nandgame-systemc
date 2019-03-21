#include <systemc>
#include <fmt/format.h>
#include "unary_alu.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 8;

  sc_signal<sc_uint<W>> X{"X"};
  sc_signal<sc_uint<W>> out{"out"};
  sc_signal<bool> z{"z"};
  sc_signal<bool> n{"n"};

  UnaryAlu<W> ualu{"unary_alu"};
  ualu.n(n);
  ualu.z(z);
  ualu.X(X);
  ualu.output(out);

  X.write(0xA5);

  auto print_ualu = [&](){
    std::cout << fmt::format("@ {:>5} :: {} (z:{:1b}) (n:{:1b}) (O:0x{:02x})\n",
                             sc_time_stamp().to_string(), ualu.name(),
                             z.read(), n.read(), out.read());
  };
  
  n.write(0); z.write(0); run_ns(1); print_ualu();
  n.write(0); z.write(1); run_ns(1); print_ualu();
  n.write(1); z.write(0); run_ns(1); print_ualu();
  n.write(1); z.write(1); run_ns(1); print_ualu();

  run_ns(1);
  return 0;
}
