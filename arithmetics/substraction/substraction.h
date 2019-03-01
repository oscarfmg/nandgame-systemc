#ifndef _SUBSTRACTION_SUBSTRACTION_H_
#define _SUBSTRACTION_SUBSTRACTION_H_

#include <systemc>
#include "arithmetics/adder/adder.h"
#include "logic_gates/inverter/inverter.h"

template <int W>
class Substract : public sc_module {
public:
  sc_in<sc_dt::sc_uint<W>> a, b;
  sc_out<sc_dt::sc_uint<W>> output;
  Substract(sc_module_name);
private:
  Adder<W> m_adder;
  sc_vector<Inverter> m_invs;

  sc_vector<sc_signal<bool>> m_inv_out_sigs, m_inv_in_sigs;
  sc_vector<sc_signal<bool>> m_addr_in_sigs, m_addr_out_sigs;;
  sc_signal<bool> m_ci_fix1;
  sc_signal<bool> m_co_terminator;

  void split_a();
  void split_b();
  void arrange_o();
};

template <int W>
Substract<W>::Substract(sc_module_name name)
: sc_module(name)
, a{"a"}
, b{"b"}
, output{"output"}
, m_adder{"adder"}
, m_invs{"inverter",W}
, m_inv_in_sigs{"signal_b2inv",W}
, m_inv_out_sigs{"signal_inv2adder",W}
, m_addr_in_sigs{"signal_a2adder",W}
, m_addr_out_sigs{"signal_adder2out",W}
, m_ci_fix1{"signal_ci"}
, m_co_terminator{"signal_co"}
{
  sc_assemble_vector(m_invs, &Inverter::input).bind(m_inv_in_sigs);
  sc_assemble_vector(m_invs, &Inverter::output).bind(m_inv_out_sigs);

  m_adder.c(m_ci_fix1);
  m_adder.b.bind(m_inv_out_sigs);
  m_adder.a.bind(m_addr_in_sigs);
  m_adder.co(m_co_terminator);
  m_adder.sum.bind(m_addr_out_sigs);
  
  SC_HAS_PROCESS(Substract);
  SC_METHOD(split_b);
  sensitive << b;

  SC_METHOD(split_a);
  sensitive << a;

  SC_METHOD(arrange_o);
  for (auto& o : m_addr_out_sigs) {
    sensitive << o;
  }
  
  m_ci_fix1.write(1);
}

template<int W>
void Substract<W>::split_b() {
  sc_dt::sc_uint<W> val = b.read();
  for (int i=0; i < W; ++i) {
    m_inv_in_sigs[i].write(val[i]);
  }
}

template<int W>
void Substract<W>::split_a() {
  sc_dt::sc_uint<W> val = a.read();
  for (int i=0; i < W; ++i) {
    m_addr_in_sigs[i].write(val[i]);
  }
}

template<int W>
void Substract<W>::arrange_o() {
  sc_dt::sc_uint<W> val = 0;
  for (int i=0; i < W; ++i) {
    val[i] = m_addr_out_sigs[i].read();
  }
  output.write(val);
}

#endif //_SUBSTRACTION_SUBSTRACTION_H_
