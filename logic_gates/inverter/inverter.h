#ifndef INVERTER_INVERTER_H_
#define INVERTER_INVERTER_H_

#include <systemc>
#include "logic_gates/nand/nand.h"

class Inverter : public sc_module {
public:
  sc_in<bool> input;
  sc_out<bool> output;

  Inverter(sc_module_name);
private:
  Nand<2> m_nand;
};

Inverter::Inverter(sc_module_name name)
: sc_module(name)
, input{"Input"}
, output{"Output"}
, m_nand{"Nand"} {
  SC_HAS_PROCESS(Inverter);

  m_nand.inputs[0].bind(input);
  m_nand.inputs[1].bind(input);

  m_nand.output.bind(output); 
}

#endif //INVERTER_INVERTER_H_
