#include <systemc>
#include <fmt/format.h>
#include "adder.h"

using namespace sc_core;
using namespace sc_dt;

template<int W>
sc_uint<W> vecToInt(sc_vector<sc_signal<bool>>& v) {
  sc_uint<W> val=0;
  for (int i=0; i < v.size(); ++i) {
    val[i] = v[i].read();
  }
  return val;
}

template<int W>
void intToVec(sc_uint<W>& val, sc_vector<sc_signal<bool>>& v) {
  for (int i = 0; i < v.size(); ++i) {
    v[i].write(val[i]);
  }
}

int sc_main(int argc, char* argv[]) {
  auto run_ns = [](int x){ sc_start(sc_time(x,sc_time_unit::SC_NS)); };
  auto max2pow = [](int x){ return (1<<x)-1; };
  
  const int W = 2;
  
  sc_vector<sc_signal<bool>> a{"a",W};
  sc_vector<sc_signal<bool>> b{"b",W};
  sc_vector<sc_signal<bool>> s{"s",W};
  sc_signal<bool> ci{"ci"};
  sc_signal<bool> co{"co"};

  Adder<2> adder{"adder"};
  adder.c(ci);
  adder.co(co);
  adder.a.bind(a);
  adder.b.bind(b);
  adder.sum.bind(s);

  sc_uint<W+1> av,bv,sumv;
  sc_uint<W> vcd_a,vcd_b;
  sc_uint<1> vcd_ci;
  sc_trace_file *vcd = sc_create_vcd_trace_file("adder");
  vcd->set_time_unit(1.0, sc_time_unit::SC_PS);
  sc_trace(vcd,vcd_ci,"Ci");
  sc_trace(vcd,vcd_a,"A");
  sc_trace(vcd,vcd_b,"B");
  sc_trace(vcd,sumv,"Sum");
  
  for (int carry_in = 0; carry_in < 2; ++carry_in) {
    ci.write(carry_in);
    vcd_ci = carry_in;

    for (av=0; av <= max2pow(W); ++av) {
      intToVec<W+1>(av,a);
    
      for (bv=0; bv <= max2pow(W); ++bv) {
        vcd_a = vecToInt<W>(a);
        vcd_b = vecToInt<W>(b);

        intToVec<W+1>(bv,b);
        run_ns(1);
        sumv=0;
        sumv = vecToInt<W+1>(s);
        sumv[W]=co.read();
    
        std::cout << fmt::format("@ {:>5} :: {} - A: {}, B: {}, C: {}, Co: {}, Sum: {}, Sum<W+1>: {}\n",
                                 sc_time_stamp().to_string(),
                                 adder.name(),
                                 av,
                                 bv,
                                 static_cast<int>(ci.read()),
                                 static_cast<int>(co.read()),
                                 static_cast<sc_uint<W>>(sumv),
                                 sumv);
        assert(av+bv+ci.read()==sumv);
      }
    }
  }

  sc_close_vcd_trace_file(vcd);
  return 0;
}
