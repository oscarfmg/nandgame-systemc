#ifndef MEMORY_REGISTER_H_
#define MEMORY_REGISTER_H_

#include <systemc>
#include "memory/dff/dff.h"
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
  Dff<W> m_dff;
};

template <int W>
Register<W>::Register(sc_module_name name)
  : sc_module(name)
  , store{"store"}
  , data{"data"}
  , clk{"clk"}
  , output{"out"}
  , m_dff{"dff"}
{
  m_dff.store(store);
  m_dff.data(data);
  m_dff.clk(clk);
  m_dff.output(output);
}

#endif //MEMORY_REGISTER_H_
