#include <systemc>
#include <fmt/format.h>
#include "selector.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 16;

  sc_signal<bool> s1{"select1"};
  sc_signal<sc_uint<1>> b1{"b1"}, b2{"b2"}, bo{"bo"};
  Selector<1> selector1{"selector1"};
  selector1.s(s1);
  selector1.d0(b1);
  selector1.d1(b2);
  selector1.output(bo);

  sc_signal<bool> s2{"select2"};
  sc_signal<sc_uint<W>> w1{"w1"}, w2{"w2"}, wo{"wo"};
  Selector<W> selectorW{"selectorW"};
  selectorW.s(s2);
  selectorW.d0(w1);
  selectorW.d1(w2);
  selectorW.output(wo);
  auto print_s1 = [&]() {
    std::cout << fmt::format("@ {:>5} :: {} - (d0: {}, d1: {}, s: {}, O: {})\n",
                             sc_time_stamp().to_string(), selector1.name(),
                             b1.read(), b2.read(), static_cast<int>(s1.read()), bo.read());
  };
  auto print_sw = [&]() {
    std::cout << fmt::format("@ {:>5} :: {} - (d0: 0x{:X}, d1: 0x{:X}, s: {}, O: 0x{:X})\n",
                             sc_time_stamp().to_string(), selectorW.name(),
                             w1.read(), w2.read(), static_cast<int>(s2.read()), wo.read());
  };

  b1.write(1); b2.write(0);  s1.write(0);  run_ns(1); print_s1();
  b1.write(1); b2.write(0);  s1.write(1);  run_ns(1); print_s1();
  b1.write(0); b2.write(1);  s1.write(0);  run_ns(1); print_s1();
  b1.write(0); b2.write(1);  s1.write(1);  run_ns(1); print_s1();

  w1.write(0xDEAD); w2.write(0xBAAD);
  s2.write(0); run_ns(1); print_sw();
  s2.write(1); run_ns(1); print_sw();

  w1.write(0xBEEF); w2.write(0xCAFE);
  s2.write(0); run_ns(1); print_sw();
  s2.write(1); run_ns(1); print_sw();
  return 0;
}
