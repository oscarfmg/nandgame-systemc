#include <systemc>
#include <fmt/format.h>
#include "alu.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  constexpr int W = 8;
  
  sc_signal<bool> zx{"zx"}, nx{"nx"};
  sc_signal<bool> zy{"zy"}, ny{"ny"};
  sc_signal<bool> f{"f"}, no{"no"};
  sc_signal<sc_uint<W>> X{"X"}, Y{"Y"};
  sc_signal<sc_uint<W>> out{"out"};
  
  Alu<W> alu{"alu"};
  alu.zx(zx); alu.nx(nx);
  alu.zy(zy); alu.ny(ny);
  alu.f(f); alu.no(no);
  alu.X(X); alu.Y(Y);
  alu.output(out);

  sc_trace_file *vcd = sc_create_vcd_trace_file("alu");
  vcd->set_time_unit(100.0, sc_time_unit::SC_PS);
  sc_trace(vcd,zx,"zx");
  sc_trace(vcd,nx,"nx");
  sc_trace(vcd,zy,"zy");
  sc_trace(vcd,ny,"ny");
  sc_trace(vcd,f,"f");
  sc_trace(vcd,no,"no");
  sc_trace(vcd,X,"X");
  sc_trace(vcd,Y,"Y");
  sc_trace(vcd,out,"output");

  
  sc_uint<W> x,y,o;
  x = 0xDE;
  y = 0xAD;
  X.write(x);
  Y.write(y);

  auto print_alu = [&]() {
    std::cout << fmt::format("@ {:>5} :: (zx: {:1b}, nx: {:1b}), "
                             "(zy: {:1b}, ny: {:1b}), "
                             "(X: {:4}, Y: {:4}), "
                             "(f: {:1b}, no: {:1b}), "
                             "O: {:4} (0x{:02x})\n",
                             sc_time_stamp().to_string(),
                             zx.read(), nx.read(),
                             zy.read(), ny.read(),
                             static_cast<sc_int<W>>(X.read()),
                             static_cast<sc_int<W>>(Y.read()),
                             f.read(), no.read(),
                             static_cast<sc_int<W>>(out.read()),
                             out.read() );
  };

  /* X + Y */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x+y));

  /* X + ~Y */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x+~y));

  /* ~X + Y */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x+y));

  /* ~X + ~Y */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x+~y));


  /** ~OUT **/
  /* ~ (X + Y) */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x+y)));

  /* ~(X + ~Y) */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x+~y)));

  /* ~(~X + Y) */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x+y)));

  /* ~(~X + ~Y) */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x+~y)));

  /** +ZEROS **/
  /* (X + 0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x+0));

  /* (X + ~0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x+~0));

  /* (0 + Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0+y));

  /* (~0 + Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0+y));

  /* (~X + 0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x+0));

  /* (~X + ~0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x+~0));

  /* (0 + ~Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0+~y));

  /* (~0 + ~Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0+~y));

  /* (0 + 0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0+0));

  /* (~0 + 0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0+0));

  /* (0 + ~0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0+~0));

  /* (~0 + ~0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(1); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0+~0));



  /** ~OUT   ZEROS **/
  /* ~(X + 0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x+0)));

  /* ~(X + ~0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x+~0)));

  /* ~(0 + Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0+y)));

  /* ~(~0 + Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0+y)));

  /* ~(~X + 0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x+0)));

  /* ~(~X + ~0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x+~0)));

  /* ~(0 + ~Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0+~y)));

  /* ~(~0 + ~Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0+~y)));

  /* ~(0 + 0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0+0)));

  /* ~(~0 + 0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0+0)));

  /* ~(0 + ~0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0+~0)));

  /* ~(~0 + ~0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(1); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0+~0)));


  /** AND FUNCTION **/
  /* X & Y */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x&y));

  /* X & ~Y */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x&~y));

  /* ~X & Y */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x&y));

  /* ~X & ~Y */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x&~y));


  /** ~OUT **/
  /* ~ (X & Y) */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x&y)));

  /* ~(X & ~Y) */
  zx.write(0); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x&~y)));

  /* ~(~X & Y) */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x&y)));

  /* ~(~X & ~Y) */
  zx.write(0); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x&~y)));

  /** +ZEROS **/
  /* (X & 0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x&0));

  /* (X & ~0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(x&~0));

  /* (0 & Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0&y));

  /* (~0 & Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0&y));

  /* (~X & 0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x&0));

  /* (~X & ~0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~x&~0));

  /* (0 & ~Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0&~y));

  /* (~0 & ~Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0&~y));

  /* (0 & 0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0&0));

  /* (~0 & 0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0&0));

  /* (0 & ~0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(0&~0));

  /* (~0 & ~0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(0); no.write(0);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~0&~0));



  /** ~OUT   ZEROS **/
  /* ~(X & 0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x&0)));

  /* ~(X & ~0) */
  zx.write(0); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(x&~0)));

  /* ~(0 & Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0&y)));

  /* ~(~0 & Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0&y)));

  /* ~(~X & 0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x&0)));

  /* ~(~X & ~0) */
  zx.write(0); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~x&~0)));

  /* ~(0 & ~Y) */
  zx.write(1); nx.write(0);
  zy.write(0); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0&~y)));

  /* ~(~0 & ~Y) */
  zx.write(1); nx.write(1);
  zy.write(0); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0&~y)));

  /* ~(0 & 0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0&0)));

  /* ~(~0 & 0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(0);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0&0)));

  /* ~(0 & ~0) */
  zx.write(1); nx.write(0);
  zy.write(1); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(0&~0)));

  /* ~(~0 & ~0) */
  zx.write(1); nx.write(1);
  zy.write(1); ny.write(1);
  f.write(0); no.write(1);
  run_ns(1);
  o = out.read();
  print_alu();
  assert(o == static_cast<sc_uint<W>>(~(~0&~0)));


  run_ns(1);
  sc_close_vcd_trace_file(vcd);

  return 0;
}
