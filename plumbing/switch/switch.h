#ifndef PLUMBING_SWITCH_H_
#define PLUMBING_SWITCH_H_

#include <systemc>

using namespace sc_core;

template <int W>
class Switch : public sc_module {
public:
  sc_in<bool> s;
  sc_in<sc_dt::sc_uint<W>> d;
  sc_out<sc_dt::sc_uint<W>> c0;
  sc_out<sc_dt::sc_uint<W>> c1;
  Switch(sc_module_name);
private:
  void switch_data();
};

template<int W>
Switch<W>::Switch(sc_module_name name)
  : sc_module(name)
  , s{"s"}
  , d{"d"}
  , c0{"c0"}
  , c1{"c1"}
{
  SC_HAS_PROCESS(Switch);
  SC_METHOD(switch_data);
  sensitive << s << d;
}

template<int W>
void Switch<W>::switch_data() {
  if ( s.read() ) {
    c1.write(d.read());
    c0.write(0);
  } else {
    c0.write(d.read());
    c1.write(0);
  }
}

#endif // PLUMBING_SWITCH_H_
