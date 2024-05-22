#ifndef PLUMBING_SELECTOR_H_
#define PLUMBING_SELECTOR_H_

#include <systemc>

using namespace sc_core;

template <int W>
class Selector : public sc_module {
public:
  sc_in<bool> s;
  sc_in<sc_dt::sc_uint<W>> d0;
  sc_in<sc_dt::sc_uint<W>> d1;
  sc_out<sc_dt::sc_uint<W>> output;
  Selector(sc_module_name);
private:
  void select();
};

template <int W>
Selector<W>::Selector(sc_module_name name)
  : sc_module(name)
  , s{"s"}
  , d0{"d0"}
  , d1{"d1"}
  , output{"output"}
{
  SC_HAS_PROCESS(Selector);
  SC_METHOD(select);
  sensitive << d0 << d1 << s;
}

template <int W>
void Selector<W>::select() {
  if( s.read() ) {
    output.write(d1.read());
  } else {
    output.write(d0.read());
  }
}

#endif // PLUMBING_SELECTOR_H_
