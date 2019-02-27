#ifndef XOR_XOR_H_
#define XOR_XOR_H_

#include <systemc>
#include <type_traits>
#include <fmt/format.h>
using namespace sc_core;

class XorBase2 : public sc_module {
public:
  sc_in<bool> a;
  sc_in<bool> b;
  sc_out<bool> o;

  XorBase2(sc_module_name);
private:
  void do_xor();
  void print_xor();
};

void XorBase2::print_xor() {
  std::string s = fmt::format("@ {:>5} :: {} (A: {}, B: {}, Out: {:>5})\n",
                              sc_time_stamp().to_string(),
                              name(),
                              static_cast<int>(a.read()),
                              static_cast<int>(b.read()),
                              o.read());
//  std::cout << s;
}

XorBase2::XorBase2(sc_module_name name)
: sc_module(name)
, a{"a"}
, b{"b"}
, o{"o"}
{
  SC_HAS_PROCESS(XorBase2);
  SC_METHOD(do_xor);
  sensitive << a << b;

  SC_METHOD(print_xor);
  sensitive << o;
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




template <int W>
class Xor : public std::conditional<(W>2),XorBaseW<W>,XorBase2>::type {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;
  Xor(sc_module_name);
  //private:
  void print_this();
};

template<>
void Xor<2>::print_this() {
//  std::cout << a.read() << b.read() << o.read() << std::endl;
}

template<int W>
void Xor<W>::print_this() {
}
/*template <int W>
Xor<W>::Xor(sc_module_name name)
: std::conditional<(W>2),XorBaseW<W>,XorBase2>::type(name)
{
}*/

template <>
Xor<2>::Xor(sc_module_name name)
: XorBase2(name)
,inputs{"inputs",2}
,output{"output"}
{
  a(inputs[0]);
  b(inputs[1]);
  o(output);

  SC_HAS_PROCESS(Xor);

  SC_METHOD(print_this);
  sensitive << a << b << o;
}

template <int W>
Xor<W>::Xor(sc_module_name name)
: XorBaseW<W>(name)
, inputs{"intput",W}
, output{"output"} {
  XorBaseW<W>::inputs.bind(inputs);
  XorBaseW<W>::output.bind(output);
  SC_HAS_PROCESS(Xor);
  SC_METHOD(print_this);
}

/*template<>
void Xor<2>::print_this() {
}

template<int W>
void Xor<W>::print_this() {
}*/

#endif // XOR_XOR_H_
