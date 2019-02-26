#ifndef NAND_NAND_H_
#define NAND_NAND_H_

#include <systemc>

using namespace sc_core;

template<int W>
class Nand : public sc_module {
public:
  sc_vector<sc_in<bool>> inputs;
  sc_out<bool> output;

  Nand(sc_module_name);
private:
  void do_nand();
};

template<int W>
Nand<W>::Nand(sc_module_name name)
: sc_module(name)
, inputs {"input",W}
, output {"output"} {
  SC_HAS_PROCESS(Nand);

  SC_METHOD(do_nand);
  for (auto &input : inputs) {
    sensitive << input;
  }
}

template<int W>
void Nand<W>::do_nand() {
  bool result = true;
  for (int i = 0; result && i < inputs.size(); ++i) {
    result = (result && inputs[i].read());
  }
  output.write(!result);
}

#endif //NAND_NAND_H_
