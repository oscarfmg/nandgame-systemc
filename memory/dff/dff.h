#ifndef MEMORY_DFF_H_
#define MEMORY_DFF_H_

#include <systemc>
using namespace sc_core;

template <int W>
class Dff : public sc_module {
public:
  sc_in<bool> store;
  sc_in<sc_dt::sc_uint<W>> data;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W>> output;
  Dff(sc_module_name);
private:
  void store_data();
};

template <int W>
Dff<W>::Dff(sc_module_name name)
  : sc_module(name)
  , store{"store"}
  , data{"data"}
  , clk{"clk"}
  , output{"out"}
{
  SC_HAS_PROCESS(Dff);
  SC_METHOD(store_data);
  sensitive << clk.pos();
}

template <int W>
void Dff<W>::store_data() {
  if (store.read()) {
    output.write(data.read());
  }
}

#endif //MEMORY_DFF_H_
