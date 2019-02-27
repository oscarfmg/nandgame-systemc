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

  /* XorBaseW */
  sc_vector<sc_signal<bool>> basew_inputs{"basew_in",3};
  sc_signal<bool> basew_out{"basew_out"};
  XorBaseW<3> xor3{"xor3"};
  xor3.inputs.bind(basew_inputs);
  xor3.output.bind(basew_out);

  /* Xor<W> ; W=2 */
  sc_vector<sc_signal<bool>> xor2_inputs{"xor2_in",2};
  sc_signal<bool> xor2_out{"xor2_out"};
  Xor<2> xor2{"Xor2"};
  xor2.inputs.bind(xor2_inputs);
  xor2.output.bind(xor2_out);
  
  /* Xor<W> ; W=4 */
  sc_vector<sc_signal<bool>> xor4_inputs{"xor4_in",4};
  sc_signal<bool> xor4_out{"xor4_out"};
  Xor<4> xor4{"Xor4"};
  xor4.inputs.bind(xor4_inputs);
  xor4.output.bind(xor4_out);


  /* XorBase2 */
  auto print_xor = [&](){
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, Out: {:>5})\n",
                                sc_time_stamp().to_string(),
                                xorbase.name(),
                                static_cast<int>(a.read()),
                                static_cast<int>(b.read()),
                                o.read());
    std::cout << s;
  };

  std::cout << "\n*** Testing XorBase2 ***" << std::endl;
  a.write(false); b.write(false); run_ns(1);  print_xor();
  a.write(false); b.write( true); run_ns(1);  print_xor();
  a.write( true); b.write(false); run_ns(1);  print_xor();
  a.write( true); b.write( true); run_ns(1);  print_xor();

  /* XorBaseW -- W = 3*/
  const int W3 = 3;
  sc_uint<W3+1> tmp;
  auto print_xor_w3 = [&](){
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, C: {}, Out: {:>5})\n",
                                sc_time_stamp().to_string(),
                                xor3.name(),
                                tmp[2],
                                tmp[1],
                                tmp[0],
                                basew_out.read());
    std::cout << s;
  };
  
  std::cout << "\n*** Testing XorBaseW ***" << std::endl;
  for (tmp = 0; tmp <= max2pow(W3); ++tmp) {
    for (int i = 0; i < W3; ++i) {
      basew_inputs[W3-i-1].write(tmp[i]);
    }
    run_ns(1);
    print_xor_w3();
  }

  /* Xor<2> */
  const int W2 = 2;
  sc_uint<W2+1> tmp2;  
  auto print_xor2 = [&] () {
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, Out: {:>5})\n",
                                sc_time_stamp().to_string(),
                                xor2.name(),
                                tmp2[1],
                                tmp2[0],
                                xor2_out.read());
    std::cout << s;
  };
  
  std::cout << "\n*** Testing Xor<2> ***" << std::endl;
  for (tmp2 = 0; tmp2 <= max2pow(W2); ++tmp2) {
    for (int i = 0; i < W2; ++i) {
      xor2_inputs[W2-i-1].write(tmp2[i]);
    }
    run_ns(1);
    print_xor2();
  }
 
  /* Xor<W> ; W=4*/
  const int W4 = 4;
  sc_uint<W4+1> tmp4;
  auto print_xor_w4 = [&](){
    std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, C: {}, D: {}, Out: {})\n",
                                sc_time_stamp().to_string(),
                                xor4.name(),
                                tmp4[3],
                                tmp4[2],
                                tmp4[1],
                                tmp4[0],
                                static_cast<int>(xor4_out.read()));
    std::cout << s;
  };
  
  std::cout << "\n*** Testing Xor<4> ***" << std::endl;
  for (tmp4 = 0; tmp4 <= max2pow(W4); ++tmp4) {
    for (int i = 0; i < W4; ++i) {
      xor4_inputs[W4-i-1].write(tmp4[i]);
    }
    run_ns(1);
    print_xor_w4();
  }

return 0;
}
