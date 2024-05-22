#ifndef PROCESSOR_COMPUTER_H
#define PROCESSOR_COMPUTER_H

#include <systemc>
#include "processor/control_unit/control_unit.h"
#include "processor/program_engine/program_engine.h"

using namespace sc_core;
using namespace sc_dt;

template <int W>
class Computer : public sc_module {
public:
  sc_in<bool> clk;
  Computer(sc_module_name, std::string program);
private:
  //TODO: this is temp
public:
  ControlUnit<W> m_cu;
  ProgramEngine<W> m_pe;
  sc_signal<bool> m_jmp_sig;
  sc_signal<sc_uint<W>> m_addr_sig;
  sc_signal<sc_uint<W>> m_inst_sig;
};

template<int W>
Computer<W>::Computer(sc_module_name name, std::string path)
  : sc_module(name)
  , clk{"clk"}
  , m_cu{"control"}
  , m_pe{"program",path}
  , m_jmp_sig{"signal_jmp"}
  , m_addr_sig{"signal_address"}
  , m_inst_sig{"signal_instruction"}
{
  m_cu.I(m_inst_sig);
  m_cu.clk(clk);
  m_cu.A(m_addr_sig);
  m_cu.j(m_jmp_sig);

  m_pe.j(m_jmp_sig);
  m_pe.addr(m_addr_sig);
  m_pe.clk(clk);
  m_pe.output(m_inst_sig);
}


#endif // PROCESSOR_COMPUTER_H
