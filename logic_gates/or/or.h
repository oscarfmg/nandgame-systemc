#ifndef OR_OR_H_
#define OR_OR_H_

#include <systemc>
#include "logic_gates/inverter/inverter.h"
#include "logic_gates/nand/nand.h"

using namespace sc_core;

template<int W>
class Or : public sc_module {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;

  Or(sc_module_name);
private:
  Nand<W> m_nand;
  sc_vector<Inverter> m_invs;
  sc_vector<sc_signal<bool>> m_inv_nand_sigs;
};

template<int W>
Or<W>::Or(sc_module_name name)
: sc_module(name)
, m_nand{"NAnd"}
, m_invs{"InvInput",W}
, inputs{"Input",W}
, m_inv_nand_sigs{"Signal_Inv_NAnd",W}
{
  sc_assemble_vector(m_invs, &Inverter::input).bind(inputs);

  sc_assemble_vector(m_invs, &Inverter::output).bind(m_inv_nand_sigs);

  m_nand.inputs(m_inv_nand_sigs);
  m_nand.output(output);
}

#endif
