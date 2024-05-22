#include <systemc>
#include <fmt/format.h>
#include "switch.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 16;

  sc_signal<bool> s1{"select1"};
  sc_signal<sc_uint<1>> b1{"b1"}, bo1{"bo1"}, bo2{"bo2"};
  Switch<1> switch1{"switch1"};
  switch1.s(s1);
  switch1.d(b1);
  switch1.c0(bo1);
  switch1.c1(bo2);

  sc_signal<bool> s2{"select2"};
  sc_signal<sc_uint<W>> w1{"w1"}, wo1{"wo1"}, wo2{"wo2"};
  Switch<W> switchW{"switchW"};
  switchW.s(s2);
  switchW.d(w1);
  switchW.c0(wo1);
  switchW.c1(wo2);
  auto print_s1 = [&]() {
    std::cout << fmt::format("@ {:>5} :: {} - (d: {}, s: {}, Ch0: {}, Ch1: {})\n",
                             sc_time_stamp().to_string(), switch1.name(),
                             b1.read(), static_cast<int>(s1.read()), bo1.read(), bo2.read());
  };
  auto print_sw = [&]() {
    std::cout << fmt::format("@ {:>5} :: {} - (d: 0x{:X}, s: {}, Ch0: 0x{:04X} Ch1: 0x{:04X})\n",
                             sc_time_stamp().to_string(), switchW.name(),
                             w1.read(), static_cast<int>(s2.read()), wo1.read(), wo2.read());
  };

  b1.write(1); s1.write(0); run_ns(1); print_s1();
  b1.write(1); s1.write(1); run_ns(1); print_s1();
  b1.write(0); s1.write(0); run_ns(1); print_s1();
  b1.write(0); s1.write(1); run_ns(1); print_s1();

  w1.write(0xDEAD);
  s2.write(0); run_ns(1); print_sw();
  s2.write(1); run_ns(1); print_sw();

  w1.write(0xCAFE);
  s2.write(0); run_ns(1); print_sw();
  s2.write(1); run_ns(1); print_sw();

  return 0;
}
