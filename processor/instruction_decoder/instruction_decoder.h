#ifndef PROCESSOR__INSTRUCTION_DECODER_H
#define PROCESSOR__INSTRUCTION_DECODER_H

#include <systemc>
using namespace sc_core;

template <int WIDTH>
class InstructionDecoder : public sc_module {
public:
  sc_in<sc_dt::sc_uint<WIDTH>> I;
  sc_out<sc_dt::sc_uint<WIDTH>> W;
  sc_out<bool> ci;
  sc_out<bool> sm; //source
  sc_out<bool> zx, nx, zy, ny, f, no; //computation
  sc_out<bool> a, d, ram; //destination
  sc_out<bool> gt, eq, lt; //condition
  InstructionDecoder(sc_module_name);

  enum PIN_MAP {
    LT_FLAG, EQ_FLAG, GT_FLAG,
    RAM_FLAG, D_FLAG, A_FLAG,
    NO_FLAG, F_FLAG,
    NY_FLAG, ZY_FLAG, NX_FLAG, ZX_FLAG,
    SM_FLAG,
    CI_FLAG = WIDTH-1
  };
private:
  void decode();
};

template <int WIDTH>
InstructionDecoder<WIDTH>::InstructionDecoder(sc_module_name name)
  : sc_module(name)
  , I{"Instruction"}
  , W{"data_word"}
  , ci{"computation_instruction"}
  , sm{"source"}
  , zx{"zero_x"}, nx{"not_x"}, zy{"zero_y"}, ny{"not_y"}
  , f{"function"}, no{"not_out"}
  , a{"a"}, d{"d"}, ram{"a*"}
  , gt{"greather_than_zero"}, eq{"equal_zero"}, lt{"less_than_zero"}
{
  static_assert(WIDTH >= 16, "WIDTH must be greather or equal than 16");
  SC_HAS_PROCESS(InstructionDecoder);

  SC_METHOD(decode);
  sensitive << I;
}

template <int WIDTH>
void InstructionDecoder<WIDTH>::decode() {
  sc_dt::sc_uint<WIDTH> w = I.read();

  if (w[WIDTH-1] == 1) {
    W.write(0);
  } else {
    W.write(w);
    w = 0;
    w[A_FLAG] = 1; //a flag must be 1 for data instruction
  }

  ci = w[CI_FLAG];
  sm = w[SM_FLAG];
  zx = w[ZX_FLAG]; nx = w[NX_FLAG];  zy = w[ZY_FLAG];  ny = w[NY_FLAG];
   f = w[ F_FLAG]; no = w[NO_FLAG];
   a = w[ A_FLAG];  d = w[ D_FLAG]; ram = w[RAM_FLAG];
  gt = w[GT_FLAG]; eq = w[EQ_FLAG];  lt = w[LT_FLAG];
}

#endif // PROCESSOR__INSTRUCTION_DECODER_H
