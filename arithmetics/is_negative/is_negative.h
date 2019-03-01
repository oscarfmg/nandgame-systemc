#ifndef ARITHMETICS__IS_NEGATIVE_H_
#define ARITHMETICS__IS_NEGATIVE_H_

#include <systemc>
using namespace sc_core;

template <int W>
class IsNegative : public sc_module {
public:
  sc_in<sc_dt::sc_uint<W>> input;
  sc_out<bool> output;
  
  IsNegative(sc_module_name);
private:
  void is_negative();
};

template <int W>
IsNegative<W>::IsNegative(sc_module_name name)
  : sc_module(name)
  , input{"input"}
  , output{"output"}
{
  SC_HAS_PROCESS(IsNegative);
  SC_METHOD(is_negative);
  sensitive << input;
}

template <int W>
void IsNegative<W>::is_negative() {
  sc_dt::sc_uint<W> val = input.read();
  output.write(val[W-1]);
}

#endif // ARITHMETICS__IS_NEGATIVE_H_
