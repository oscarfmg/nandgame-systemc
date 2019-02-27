#include <systemc>
#include <fmt/format.h>
#include "xor.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x) { sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x) { return (1<<x)-1; };

  /* Initialization */
  /* XorBase2 */
  XorBase2 xorbase{"xor"};
  sc_signal<bool> a;
  sc_signal<bool> b;
  sc_signal<bool> o;

  xorbase.a(a);
  xorbase.b(b);
  xorbase.o(o);
  
//  Xor<2> xor2{"Xor2"};
//  Xor<3> xor3{"Xor3"};
 
  auto print_xor = [&](){
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, Out: {:>5})\n",
                                sc_time_stamp().to_string(),
                                xorbase.name(),
                                static_cast<int>(a.read()),
                                static_cast<int>(b.read()),
                                o.read());
    std::cout << s;
  };

  a.write(false); b.write(false); run_ns(1);  print_xor();
  a.write(false); b.write( true); run_ns(1);  print_xor();
  a.write( true); b.write(false); run_ns(1);  print_xor();
  a.write( true); b.write( true); run_ns(1);  print_xor();
 
  return 0;
}
