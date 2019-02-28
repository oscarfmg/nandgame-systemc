#ifndef MULTIBIT_ADDER_H_
#define MULTIBIT_ADDER_H_

#include <systemc>
#include "arithmetics/full_adder/full_adder.h"
using namespace sc_core;

template <int W>
class Adder : public sc_module {
public:
  sc_vector<sc_in<bool>> a;
  sc_vector<sc_in<bool>> b;
  sc_in<bool> c;
  sc_vector<sc_out<bool>> sum;
  sc_out<bool> co;

  Adder(sc_module_name);
private:
  sc_vector<FullAdder> m_adders;
  sc_vector<sc_signal<bool>> m_adder_h_sig;
};

template<int W>
Adder<W>::Adder(sc_module_name name)
: sc_module(name)
, a{"a",W}
, b{"b",W}
, c{"c"}
, sum{"sum",W}
, co{"co"}
, m_adders{"adder",W}
, m_adder_h_sig{"sig_adder_h",W-1}
{
//  static_assert(W>=2,"W must be greather than 1.");
  m_adders[0].c(c);
  for (int i = 0; i < W; ++i) {
    m_adders[i].a(a[i]);
    m_adders[i].b(b[i]);
    m_adders[i].l(sum[i]);
    if (i < W-1) {
      m_adders[i].h(m_adder_h_sig[i]);
      m_adders[i+1].c(m_adder_h_sig[i]);
    }
  }
  m_adders[W-1].h(co);
}

#endif //MULTIBIT_ADDER_H_
