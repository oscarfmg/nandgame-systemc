#ifndef ALU__UNARY_ALU_H_
#define ALU__UNARY_ALU_H_

#include <systemc>
using namespace sc_core;

template<int W>
class UnaryAlu : public sc_module {
public:
  sc_in<bool> z;
  sc_in<bool> n;
  sc_in<sc_dt::sc_uint<W>> X;
  sc_out<sc_dt::sc_uint<W>> output;
  
  UnaryAlu(sc_module_name);
private:
  void process();
};

template<int W>
UnaryAlu<W>::UnaryAlu(sc_module_name name)
  : sc_module(name)
  , z{"zero"}
  , n{"negate"}
  , X{"data"}
  , output{"output"}
{
  SC_HAS_PROCESS(UnaryAlu);

  SC_METHOD(process);
  sensitive << z << n << X;
}

template <int W>
void UnaryAlu<W>::process() {
  sc_dt::sc_uint<W> data;
  if (z.read()) {
    data = 0;
  } else {
    data = X.read();
  }

  if (n.read()) {
    data = ~data;
  }
  output.write(data);
}

#endif
