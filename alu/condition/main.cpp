#include <systemc>
#include <fmt/format.h>
#include "condition.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 4;

  sc_signal<bool> lt{"lt"};
  sc_signal<bool> eq{"eq"};
  sc_signal<bool> gt{"gt"};
  sc_signal<sc_uint<W>> X{"X"};
  sc_signal<bool> out{"output"};

  Condition<W> cond{"condition"};
  cond.lt(lt);
  cond.eq(eq);
  cond.gt(gt);
  cond.X(X);
  cond.output(out);

  sc_int<W> x;
  sc_uint<W> ux;

  auto print_cond = [&]() {
    std::cout << fmt::format("@ {:>5} :: lt: {:1b}, eq: {:1b}, gt: {:1b}, O:{:1b}\n",
                             sc_time_stamp().to_string(),
                             lt.read(),
                             eq.read(),
                             gt.read(),
                             out.read());
  };

  auto must_be = [&](bool x) {
    assert(out.read() == x);
  };

  /** x = -6 **/
  ux = x = -6;
  X.write(ux);
  std::cout << fmt::format("x: {:04b} ({})\n",ux,x);
  lt=0; eq=0; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=0; gt=1; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=1; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=1; gt=1; run_ns(1); print_cond(); must_be(0);
  lt=1; eq=0; gt=0; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=0; gt=1; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=1; gt=0; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=1; gt=1; run_ns(1); print_cond(); must_be(1);
  
  /** x = 0 **/
  ux = x = 0;
  X.write(ux);
  std::cout << fmt::format("x: {:04b} ({})\n",ux,x);
  lt=0; eq=0; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=0; gt=1; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=1; gt=0; run_ns(1); print_cond(); must_be(1);
  lt=0; eq=1; gt=1; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=0; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=1; eq=0; gt=1; run_ns(1); print_cond(); must_be(0);
  lt=1; eq=1; gt=0; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=1; gt=1; run_ns(1); print_cond(); must_be(1);

  /** x = 5 **/
  ux = x = 5;
  X.write(ux);
  std::cout << fmt::format("x: {:04b} ({})\n",ux,x);
  lt=0; eq=0; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=0; gt=1; run_ns(1); print_cond(); must_be(1);
  lt=0; eq=1; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=0; eq=1; gt=1; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=0; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=1; eq=0; gt=1; run_ns(1); print_cond(); must_be(1);
  lt=1; eq=1; gt=0; run_ns(1); print_cond(); must_be(0);
  lt=1; eq=1; gt=1; run_ns(1); print_cond(); must_be(1);

  run_ns(1);
  return 0;
}
