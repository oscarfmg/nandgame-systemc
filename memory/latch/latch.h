#ifndef MEMORY_LATCH_H_
#define MEMORY_LATCH_H_

#include <systemc>
#include "plumbing/selector/selector.h"

using namespace sc_core;

template<int W>
class Latch : public sc_module {
public:
  sc_in<bool> store;
  sc_in<sc_dt::sc_uint<W>> data;
  sc_out<sc_dt::sc_uint<W>> output;
  Latch(sc_module_name);
private:
  Selector<W> m_select;
  sc_signal<sc_dt::sc_uint<W>, SC_MANY_WRITERS> m_out2data0_sig;
  void write_out();
};

template<int W>
Latch<W>::Latch(sc_module_name name)
  : sc_module(name)
  , store{"store"}
  , data{"data"}
  , output{"output"}
  , m_select{"select"}
  , m_out2data0_sig{"signal_out2data0"}
{
  m_select.s(store);
  m_select.d1(data);

  m_select.output(m_out2data0_sig);
  m_select.d0(m_out2data0_sig);

  SC_HAS_PROCESS(Latch);
  SC_METHOD(write_out);
  sensitive << m_select.output;
}

template <int W>
void Latch<W>::write_out() {
  output.write(m_select.output.read());
}
#endif // MEMORY_LATCH_H_
