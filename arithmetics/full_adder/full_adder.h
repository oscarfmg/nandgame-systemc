#ifndef FULLADDER_HALFADDER_H_
#define FULLADDER_HALFADDER_H_

#include <systemc>
#include "logic_gates/nand/nand.h"
using namespace sc_core;

class FullAdder : public sc_module {
public:
  sc_in<bool> a;
  sc_in<bool> b;
  sc_in<bool> c;
  sc_out<bool> l,h;

  FullAdder(sc_module_name);
private:
  sc_vector<Nand<2>> nands;
/*  sc_signal<bool> m_sig_nand0_nand1_nand2;
  sc_signal<bool> m_sig_nand1_nand3;
  sc_signal<bool> m_sig_nand2_nand3;*/
  sc_signal<bool> m_nand0_out, m_nand1_out, m_nand2_out, m_nand3_out;
  sc_signal<bool> m_nand4_out, m_nand5_out, m_nand6_out;
};

FullAdder::FullAdder(sc_module_name name)
: sc_module(name)
, a{"a"}
, b{"b"}
, c{"c"}
, l{"l"}
, h{"h"}
, nands{"nand",9}
, m_nand0_out{"nand0_out_sig"}
, m_nand1_out{"nand1_out_sig"}
, m_nand2_out{"nand2_out_sig"}
, m_nand3_out{"nand3_out_sig"}
, m_nand4_out{"nand4_out_sig"}
, m_nand5_out{"nand5_out_sig"}
, m_nand6_out{"nand6_out_sig"}
{
  nands[0].inputs[0](a);
  nands[0].inputs[1](b);
  nands[0].output(m_nand0_out);

  nands[1].inputs[0](a);
  nands[1].inputs[1](m_nand0_out);
  nands[1].output(m_nand1_out);

  nands[2].inputs[0](m_nand0_out);
  nands[2].inputs[1](b);
  nands[2].output(m_nand2_out);
  
  nands[3].inputs[0](m_nand1_out);
  nands[3].inputs[1](m_nand2_out);
  nands[3].output(m_nand3_out);

  nands[4].inputs[0](m_nand3_out);
  nands[4].inputs[1](c);
  nands[4].output(m_nand4_out);

  nands[5].inputs[0](m_nand3_out);
  nands[5].inputs[1](m_nand4_out);
  nands[5].output(m_nand5_out);

  nands[6].inputs[0](m_nand4_out);
  nands[6].inputs[1](c);
  nands[6].output(m_nand6_out);

  nands[7].inputs[0](m_nand5_out);
  nands[7].inputs[1](m_nand6_out);
  nands[7].output(l);

  nands[8].inputs[0](m_nand0_out);
  nands[8].inputs[1](m_nand4_out);
  nands[8].output(h);
}

#endif //FULL_ADDER_HALFADDER_H_
