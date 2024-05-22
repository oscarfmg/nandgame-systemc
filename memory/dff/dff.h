#ifndef MEMORY_DFF_H_
#define MEMORY_DFF_H_

#include <systemc>
#include "memory/latch/latch.h"
#include "logic_gates/inverter/inverter.h"
#include "logic_gates/and/and.h"
using namespace sc_core;

template <int W>
class Dff : public sc_module {
public:
  sc_in<bool> store;
  sc_in<sc_dt::sc_uint<W>> data;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W>> output;
  Dff(sc_module_name);
private:
  And<2> m_and;
  Inverter m_inv;
  Latch<W> m_latch0, m_latch1;
  sc_signal<sc_dt::sc_uint<W>> m_outLatch0_sig;
  sc_signal<bool> m_outInv_sig;
  sc_signal<bool> m_outAnd_sig;
};

template <int W>
Dff<W>::Dff(sc_module_name name)
  : sc_module(name)
  , store{"store"}
  , data{"data"}
  , clk{"clk"}
  , output{"out"}
  , m_latch0{"latch0"}
  , m_latch1{"latch1"}
  , m_and{"and"}
  , m_inv{"inv"}
  , m_outLatch0_sig{"signal_outLatch0"}
  , m_outInv_sig{"signal_outInv"}
  , m_outAnd_sig{"signal_outAnd"}
{
  m_and.inputs[0](store);
  m_and.inputs[1](m_outInv_sig);
  m_and.output(m_outAnd_sig);

  m_inv.input(clk);
  m_inv.output(m_outInv_sig);
  
  m_latch0.store(m_outAnd_sig);
  m_latch0.output(m_outLatch0_sig);
  m_latch0.data(data);

  m_latch1.store(clk);
  m_latch1.data(m_outLatch0_sig);
  m_latch1.output(output); 
}

#endif //MEMORY_DFF_H_
