#ifndef HALFADDER_HALFADDER_H_
#define HALFADDER_HALFADDER_H_

#include <systemc>
#include "logic_gates/nand/nand.h"
#include "logic_gates/inverter/inverter.h"
using namespace sc_core;

class HalfAdder : public sc_module {
public:
  sc_in<bool> a;
  sc_in<bool> b;
  sc_out<bool> l,h;

  HalfAdder(sc_module_name);
private:
  sc_vector<Nand<2>> nands;
  Inverter inverter;
  sc_signal<bool> m_sig_nand0_nand1_nand2;
  sc_signal<bool> m_sig_nand1_nand3;
  sc_signal<bool> m_sig_nand2_nand3;
};

HalfAdder::HalfAdder(sc_module_name name)
: sc_module(name)
, a{"a"}
, b{"b"}
, l{"l"}
, h{"h"}
, nands{"nands",4}
, inverter{"not_nand0"}
, m_sig_nand0_nand1_nand2{"NAnd0_NAnd1_NAnd2_signal"}
, m_sig_nand1_nand3{"NAnd1_NAnd3_signal"}
, m_sig_nand2_nand3{"NAnd2_NAnd3_signal"}
{
  nands[0].inputs[0](a);
  nands[0].inputs[1](b);
  nands[0].output(m_sig_nand0_nand1_nand2);

  nands[1].inputs[0](a);
  nands[1].inputs[1](m_sig_nand0_nand1_nand2);
  nands[1].output(m_sig_nand1_nand3);

  nands[2].inputs[0](m_sig_nand0_nand1_nand2);
  nands[2].inputs[1](b);
  nands[2].output(m_sig_nand2_nand3);
  
  nands[3].inputs[0](m_sig_nand1_nand3);
  nands[3].inputs[1](m_sig_nand2_nand3);
  nands[3].output(l);

  inverter.input(m_sig_nand0_nand1_nand2);
  inverter.output(h);
}

#endif //HALF_ADDER_HALFADDER_H_
