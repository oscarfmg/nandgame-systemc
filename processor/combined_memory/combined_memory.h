#ifndef PROCESSOR__COMBINED_MEMORY_H
#define PROCESSOR__COMBINED_MEMORY_H

#include <systemc>
#include "memory/register/register.h"
#include "memory/ram/ram.h"

using namespace sc_core;

template <int W>
class CombinedMemory : public sc_module {
public:
  sc_in<bool> st_a;
  sc_in<bool> st_d;
  sc_in<bool> st_ram;
  sc_in<sc_dt::sc_uint<W>> X;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W>> A;
  sc_out<sc_dt::sc_uint<W>> D;
  sc_out<sc_dt::sc_uint<W>> RAM;
  CombinedMemory(sc_module_name);
private:
  Register<W> m_regA;
  Register<W> m_regD;
  Ram<W,W>    m_ram;
  sc_signal<sc_dt::sc_uint<W>> m_reg2ram_sig;
  void write_regA();
};

template <int W>
CombinedMemory<W>::CombinedMemory(sc_module_name name)
  : sc_module(name)
  , st_a{"a"}, st_d{"d"}, st_ram{"a*"}
  , X{"X"}, clk{"clk"}
  , A{"A"}, D{"D"}, RAM{"A*"}
  , m_regA{"register_A"}
  , m_regD{"register_D"}
  , m_ram{"ram"}
  , m_reg2ram_sig{"signal_regA_to_ram"}
{
  m_regA.store(st_a);
  m_regA.data(X);
  m_regA.clk(clk);
  m_regA.output(m_reg2ram_sig);

  m_regD.store(st_d);
  m_regD.data(X);
  m_regD.clk(clk);
  m_regD.output(D);

  m_ram.X(X);
  m_ram.addr(m_reg2ram_sig);
  m_ram.st(st_ram);
  m_ram.clk(clk);
  m_ram.output(RAM);

  SC_HAS_PROCESS(CombinedMemory);
  SC_METHOD(write_regA);
  sensitive << m_regA.output;
}

template <int W>
void CombinedMemory<W>::write_regA() {
  A.write(m_regA.output.read());
}

#endif // PROCESSOR__COMBINED_MEMORY_H
