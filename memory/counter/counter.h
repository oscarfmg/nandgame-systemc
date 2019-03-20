#ifndef MEMORY_COUNTER_H_
#define MEMORY_COUNTER_H_

#include <systemc>
using namespace sc_core;

template<int W>
class Counter : public sc_module {
public:
  sc_in<bool> st;
  sc_in<sc_dt::sc_uint<W>> X;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W>> output;

  Counter(sc_module_name);
private:
  void count();
};

template<int W>
Counter<W>::Counter(sc_module_name name)
  : sc_module(name)
  , st{"store"}
  , X{"data"}
  , clk{"clk"}
  , output{"output"}
{
  SC_HAS_PROCESS(Counter);
  SC_METHOD(count);
  sensitive << clk.pos();
}

template<int W>
void Counter<W>::count() {
  if (st.read()) {
    output.write(X.read());
  } else {
    output.write(output.read()+1);
  }
}

#endif //MEMORY_COUNTER_H_
