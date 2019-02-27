#ifndef XOR_XOR_H_
#define XOR_XOR_H_

#include <systemc>
#include <type_traits>
using namespace sc_core;

class XorBase2 : public sc_module {
public:
  sc_in<bool> a;
  sc_in<bool> b;
  sc_out<bool> o;

  XorBase2(sc_module_name);
private:
  void do_xor();
};

XorBase2::XorBase2(sc_module_name name)
: sc_module(name)
{
  SC_HAS_PROCESS(XorBase2);
  SC_METHOD(do_xor);
  sensitive << a << b;
}

void XorBase2::do_xor() {
  o.write(a.read() != b.read()); // != acts as XOR on booleans
}

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
{
  static_assert(W > 2, "Module width must be greather than 1.");
}

template <int W>
class Xor : public std::conditional<(W>2),XorBaseW<W>,XorBase2>::type {
public:

  Xor(sc_module_name);
private:
};


template <int W>
Xor<W>::Xor(sc_module_name name)
//: (name)
: std::conditional<(W>2),XorBaseW<W>,XorBase2>::type(name)
{
}
#endif // XOR_XOR_H_
