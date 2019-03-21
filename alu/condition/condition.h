#ifndef ALU_CONDITION_H
#define ALU_CONDITION_H

#include <systemc>
using namespace sc_core;

template <int W>
class Condition : public sc_module {
public:
  sc_in<bool> lt;
  sc_in<bool> eq;
  sc_in<bool> gt;
  sc_in<sc_dt::sc_uint<W>> X;
  sc_out<bool> output;

  Condition(sc_module_name);
private:
  void process();
};

template <int W>
Condition<W>::Condition(sc_module_name name)
  : sc_module(name)
  , lt{"less_than"}
  , eq{"equal_to_zero"}
  , gt{"greather_than"}
  , X{"data"}
  , output{"output"}
{
  SC_HAS_PROCESS(Condition);

  SC_METHOD(process);
  sensitive << lt << eq << gt << X;
}

template <int W>
void Condition<W>::process() {
  bool is_neg = (X.read()[W-1] == 1);
  bool is_zero = (X.read() == 0);
  bool is_pos = ! (is_neg || is_zero);

  bool out = false;
  if (lt.read()) {
    out |= is_neg;
  }

  if (eq.read()) {
    out |= is_zero;
  }

  if (gt.read()) {
    out |= is_pos;
  }

  output.write(out);
}

#endif // ALU_CONDITION_H
