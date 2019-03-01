#ifndef ARITHMETICS__IS_ZERO_H_
#define ARITHMETICS__IS_ZERO_H_

#include <systemc>
#include "logic_gates/or/or.h"
#include "logic_gates/inverter/inverter.h"

using namespace sc_core;

template <int W>
class IsZero : public sc_module {
public:
  sc_in<sc_dt::sc_uint<W>> input;
  sc_out<bool> output;
  IsZero(sc_module_name);
private:
  Or<W> m_or;
  Inverter m_inv;
  sc_signal<bool> m_or2inv_sig;
  sc_vector<sc_signal<bool>> m_or_input_sigs;
  void split_input();
};

template <int W>
IsZero<W>::IsZero(sc_module_name name)
  : sc_module(name)
  , input{"input"}
  , output{"output"}
  , m_or{"or"}
  , m_inv{"not"}
  , m_or2inv_sig{"sig_or2inv"}
  , m_or_input_sigs{"sig_or_in",W}
{
  m_or.inputs.bind(m_or_input_sigs);
  m_or.output(m_or2inv_sig);

  m_inv.input(m_or2inv_sig);
  m_inv.output(output);

  SC_HAS_PROCESS(IsZero);
  SC_METHOD(split_input);
  sensitive << input;
}

template <int W>
void IsZero<W>::split_input() {
  sc_dt::sc_uint<W> val = input.read();
  for (int i=0; i < W; ++i) {
    m_or_input_sigs[i].write(val[i]);
  }
}

#endif //ARITHMETICS__IS_ZERO_H_
