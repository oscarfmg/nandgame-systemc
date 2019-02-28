#ifndef __INCREMENT_H_
#define __INCREMENT_H_

#include <systemc>
#include "arithmetics/adder/adder.h"
using namespace sc_core;

template <int W>
class Increment : public sc_module {
public:
  sc_in<sc_dt::sc_uint<W>> input;
  sc_out<sc_dt::sc_uint<W>> output;
  Increment(sc_module_name);
private:
  Adder<W> m_adder;
  sc_vector<sc_signal<bool>> m_a_sigs, m_b_sigs, m_sum_sigs;
  sc_signal<bool> m_terminator0, m_terminator1;
  void split_value();
  void arrange_out();
};

template <int W>
Increment<W>::Increment(sc_module_name name)
: sc_module(name)
, input{"input"}
, output{"output"}
, m_adder{"adder"}
, m_a_sigs{"a_signal",W}
, m_b_sigs{"b_signal",W}
, m_sum_sigs{"sum_signal",W}
, m_terminator0{"terminator0"}
, m_terminator1{"terminator1"}
{
  m_adder.co(m_terminator1);
  m_adder.c(m_terminator0);

  m_adder.sum.bind(m_sum_sigs);
  m_adder.a.bind(m_a_sigs);
  m_adder.b.bind(m_b_sigs);

  SC_HAS_PROCESS(Increment);
  SC_METHOD(split_value);
  sensitive << input;

  SC_METHOD(arrange_out);
  for (auto& out : m_sum_sigs) {
    sensitive << out;
  }

  for (auto& b : m_b_sigs) {
    b.write(0);
  }
  m_b_sigs[0].write(1);
}

template <int W>
void Increment<W>::split_value() {
  for (int i=0; i < W; ++i) {
    m_a_sigs[i].write(input.read()[i]);
  }
}

template <int W>
void Increment<W>::arrange_out() {
  sc_dt::sc_uint<W> out=0;
  for (int i=0; i < W; ++i) {
    out[i] = m_sum_sigs[i].read();
  }
  output.write(out);
}

#endif //__INCREMENT_H_
