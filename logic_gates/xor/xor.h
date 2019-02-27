#ifndef XOR_XOR_H_
#define XOR_XOR_H_

#include <systemc>
#include <type_traits>

#include <logic_gates/nand/nand.h>
using namespace sc_core;

/*****************************   XorBase2   ***************************/
class XorBase2 : public sc_module {
public:
  sc_in<bool> a;
  sc_in<bool> b;
  sc_out<bool> o;

  XorBase2(sc_module_name);
private:
  sc_vector<Nand<2>> nands;
  sc_signal<bool> m_sig_nand0_nand1_nand2;
  sc_signal<bool> m_sig_nand1_nand3;
  sc_signal<bool> m_sig_nand2_nand3;
};

XorBase2::XorBase2(sc_module_name name)
: sc_module(name)
, a{"a"}
, b{"b"}
, o{"o"}
, nands{"NAnds",4}
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
  nands[3].output(o);
}
/*****************************   XorBase2   ***************************/


/*****************************   XorBaseW   ***************************/
template<int W>
class XorBaseW : public sc_module {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;
  
  XorBaseW(sc_module_name);
private:
  sc_vector<XorBase2> m_xors;
  sc_vector<sc_signal<bool>> m_inner_sigs;
};

template<int W>
XorBaseW<W>::XorBaseW(sc_module_name name)
: sc_module(name)
, m_xors{"xor",W-1}
, m_inner_sigs{"signal_next_xor",W-2}
, inputs{"input",W}
, output{"output"}
{
  m_xors[0].a(inputs[0]);
  m_xors[0].b(inputs[1]);
 
  int inner_signal = 0;
  for (;inner_signal < W-2; ++inner_signal) {
    m_xors[inner_signal+1].a(m_inner_sigs[inner_signal]);
    m_xors[inner_signal].o(m_inner_sigs[inner_signal]);

    m_xors[inner_signal+1].b(inputs[inner_signal+2]);
  }

  m_xors[W-2].o(output);
  static_assert(W > 2, "Module width must be greather than 1.");
}
/*****************************   XorBaseW   ***************************/



/*******************************    Xor   *****************************/
template <int W>
class Xor : public std::conditional<(W>2),XorBaseW<W>,XorBase2>::type {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;
  Xor(sc_module_name);
};

template <>
Xor<2>::Xor(sc_module_name name)
: XorBase2(name)
,inputs{"inputs",2}
,output{"output"}
{
  a(inputs[0]);
  b(inputs[1]);
  o(output);
}

template <int W>
Xor<W>::Xor(sc_module_name name)
: XorBaseW<W>(name)
, inputs{"intput",W}
, output{"output"} {
  XorBaseW<W>::inputs.bind(inputs);
  XorBaseW<W>::output.bind(output);
}
/*******************************    Xor   *****************************/


#endif // XOR_XOR_H_
