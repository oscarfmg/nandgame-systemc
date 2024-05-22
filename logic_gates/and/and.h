#ifndef AND_AND_H_
#define AND_AND_H_

#include <systemc>
#include "logic_gates/nand/nand.h"
#include "logic_gates/inverter/inverter.h"

using namespace sc_core;

template<int W>
class And : public sc_module {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;

  And(sc_module_name);
private:
  sc_signal<bool> m_nand_inv_sig;
  Nand<W>  m_nand;
  Inverter m_inv;
};

template<int W>
And<W>::And(sc_module_name name)
: sc_module(name)
, m_nand{"NAnd"}
, m_inv{"Not"}
, inputs{"input",W}
, output{"output"}
, m_nand_inv_sig{"signal_nand_inv"}
{
  m_nand.inputs(inputs);
  
  m_nand.output(m_nand_inv_sig);
  m_inv.input(m_nand_inv_sig);
  
  m_inv.output.bind(output);
}

#endif //AND_AND_H_
