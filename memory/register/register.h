#ifndef MEMORY_REGISTER_H_
#define MEMORY_REGISTER_H_

#include <systemc>
using namespace sc_core;

template <int W>
class Register : public sc_module {
public:
  sc_in<bool> store;
  sc_in<sc_dt::sc_uint<W>> data;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W>> output;
  Register(sc_module_name);
private:
  void store_data();
};

template <int W>
Register<W>::Register(sc_module_name name)
  : sc_module(name)
  , store{"store"}
  , data{"data"}
  , clk{"clk"}
  , output{"out"}
{
  SC_HAS_PROCESS(Register);
  SC_METHOD(store_data);
  sensitive << clk.neg();
}

template <int W>
void Register<W>::store_data() {
  if (store.read()) {
    output.write(data.read());
  }
}

#endif //MEMORY_REGISTER_H_
