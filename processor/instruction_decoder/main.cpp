#include <systemc>
#include <fmt/format.h>

#include "instruction_decoder.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int WIDTH = 16;

  sc_signal<sc_uint<WIDTH>> I{"instruction"};
  sc_signal<sc_uint<WIDTH>> W{"data_word"};
  sc_signal<bool> ci{"computation_instruction"};
  sc_signal<bool> sm{"source"};
  sc_signal<bool> zx{"zero_x"}, nx{"not_x"}, zy{"zero_y"}, ny{"not_y"};
  sc_signal<bool> f{"function"}, no{"not_out"};
  sc_signal<bool> a{"a"}, d{"d"}, ram{"a*"};
  sc_signal<bool> gt{"greather_than_zero"}, eq{"equal_zero"}, lt{"less_than_zero"};
  
  InstructionDecoder<WIDTH> id{"instruction_decoder"};
  id.I(I);
  id.W(W);
  id.ci(ci);
  id.sm(sm);
  id.zx(zx);  id.nx(nx);  id.zy(zy);  id.ny(ny);  id.f(f);  id.no(no);
  id.a(a);  id.d(d);  id.ram(ram);
  id.gt(gt);  id.eq(eq);  id.lt(lt);
  
  auto print_decoder = [&](){
    std::cout << fmt::format("@ {:>5} :: (ci: {:b}) (sm: {:b}) "
                             "(zx: {:b}, nx: {:b}, zy: {:b}, ny: {:b}) (f: {:b}, no: {:b})\n"
                             "           (a: {:b}, d: {:b}, a*: {:b}) (gt: {:b}, eq: {:b}, lt: {:b}) "
                             "(W: 0x{:04x})\n",
                             sc_time_stamp().to_string(),
                             ci.read(), sm.read(),
                             zx.read(), nx.read(), zy.read(), ny.read(), f.read(), no.read(),
                             a.read(), d.read(), ram.read(), gt.read(), eq.read(), lt.read(), W.read());
  };

  auto check_decoder = [&](bool _ci, bool _sm, bool _zx, bool _nx, bool _zy, bool _ny,
                           bool _f, bool _no, bool _a, bool _d, bool _ram,
                           bool _gt, bool _eq, bool _lt, sc_uint<WIDTH> _W) {
    assert(ci.read() == _ci);
    assert(sm.read() == _sm);
    assert(zx.read() == _zx);
    assert(nx.read() == _nx);
    assert(zy.read() == _zy);
    assert(ny.read() == _ny);
    assert(f.read() == _f);
    assert(no.read() == _no);
    assert(a.read() == _a);
    assert(d.read() == _d);
    assert(ram.read() == _ram);
    assert(gt.read() == _gt);
    assert(eq.read() == _eq);
    assert(lt.read() == _lt);
    assert(W.read() == _W);
    
  };
  
//  print_decoder();

  fmt::print("Sets the A register to the value 1234, 1234->A\n");
  I = 0x4d2; //Sets the A register to the value 1234, 1234->A
  run_ns(1);
  print_decoder();
  check_decoder(0,0,0,0,0,0,0,0,1,0,0,0,0,0,1234);

  fmt::print("Sums A and D and stores it into A, D+A->A\n");
  I = 0xe0a0; //Sums A and D and stores it into A, D+A->A
  run_ns(1);
  print_decoder();
  check_decoder(1,0,0,0,0,0,1,0,1,0,0,0,0,0,0);

  fmt::print("Increment D register, D+1->D\n");
  I = 0xe7d0; //Increment D register, D+1->D
  run_ns(1);
  print_decoder();
  check_decoder(1,0,0,1,1,1,1,1,0,1,0,0,0,0,0);

  fmt::print("Copies the value from A into D, A->D\n");
  I = 0xec10; //Copies the value from A into D, A->D
  run_ns(1);
  print_decoder();
  check_decoder(1,0,1,1,0,0,0,0,0,1,0,0,0,0,0);

  fmt::print("Sets the A register to the value 42, 42->A\n");
  I = 0x2a; //Sets the A register to the value 42, 42->A
  run_ns(1);
  print_decoder();
  check_decoder(0,0,0,0,0,0,0,0,1,0,0,0,0,0,42);

  fmt::print("Jumps if D is 0, D; JEQ\n");
  I = 0xe302; //Jumps if D is 0, D; JEQ
  run_ns(1);
  print_decoder();
  check_decoder(1,0,0,0,1,1,0,0,0,0,0,0,1,0,0);

  fmt::print("1->A*AD;JMP\n");
  I = 0x9fff;
  run_ns(1);
  print_decoder();
  check_decoder(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0);
  
  run_ns(1);
  return 0;
}
