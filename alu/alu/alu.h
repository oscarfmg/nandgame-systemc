#ifndef ALU_ALU_H
#define ALU_ALU_H

#include <systemc>
using namespace sc_core;

template <int W>
class Alu : public sc_module {
public:
  sc_in<bool> zx, nx;
  sc_in<bool> zy, ny;
  sc_in<bool> f;
  sc_in<bool> no;
  sc_in<sc_dt::sc_uint<W>> X, Y;
  sc_out<sc_dt::sc_uint<W>> output;
  
  Alu(sc_module_name);
private:
  void process();
};

template <int W>
Alu<W>::Alu(sc_module_name name)
  : sc_module(name)
  , zx{"zx"}, nx{"nx"}
  , zy{"zy"}, ny{"ny"}
  , f{"f"}, no{"no"}
  , X{"X"}, Y{"Y"}
  , output{"output"}
{
  SC_HAS_PROCESS(Alu);

  SC_METHOD(process);
  sensitive << zx << nx << zy << ny;
  sensitive << f << no << X << Y;
}

template <int W>
void Alu<W>::process() {
  sc_dt::sc_uint<W> _X = X.read();
  sc_dt::sc_uint<W> _Y = Y.read();
  sc_dt::sc_uint<W> out;
  if ( zx.read() ) {  _X = 0;  }
  if ( zy.read() ) {  _Y = 0;  }
  if ( nx.read() ) {  _X = ~_X; }
  if ( ny.read() ) {  _Y = ~_Y; }

  if ( f.read() ) {
    out = _X + _Y;
  } else {
    out = _X & _Y;
  }

  if ( no.read() ) {
    out = ~out;
  }

  output.write(out);
}

#endif // ALU_ALU_H
