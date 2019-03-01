#include <systemc>
#include <fmt/format.h>
#include "substraction.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  const int W = 5;

  sc_signal<sc_uint<W>> a{"a"},b{"b"}, o{"o"};
  
  Substract<W> sub{"Substraction"};
  sub.a(a);
  sub.b(b);
  sub.output(o);

  auto print_sub = [&]() {
    sc_int<W> val = o.read().to_int();
    std::cout << fmt::format("@ {:>5} :: {} - A: {:2}, B: {:2}, Sub: {:2}\n",
                             sc_time_stamp().to_string(),
                             sub.name(),
                             a.read(),
                             b.read(),
                             val);
  };                                              
  
  a.write(10);  b.write( 7);  run_ns(1); print_sub(); assert(o.read() ==  3);
  a.write( 0);  b.write( 0);  run_ns(1); print_sub(); assert(o.read() ==  0);
  a.write( 7);  b.write(10);  run_ns(1); print_sub(); assert(o.read() == sc_uint<W>(-3));
  a.write(10);  b.write(-1);  run_ns(1); print_sub(); assert(o.read() == 11);
  a.write(31);  b.write(31);  run_ns(1); print_sub(); assert(o.read() ==  0);
  return 0;
}
