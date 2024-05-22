#ifndef PROCCESOR__PROGRAM_ENGINE_H
#define PROCCESOR__PROGRAM_ENGINE_H

#include <fstream>
#include <systemc>
#include "memory/ram/ram.h"
#include "memory/counter/counter.h"

using namespace sc_core;
using namespace sc_dt;

template <int W>
class ProgramEngine : public sc_module {
public:
  sc_in<bool> j;
  sc_in<sc_uint<W>> addr;
  sc_in<bool> clk;
  sc_out<sc_uint<W>> output;
  
  ProgramEngine(sc_module_name);
  ProgramEngine(sc_module_name, std::string);
private:
  Counter<W> m_counter;
  Ram<W,W>   m_rom;
  sc_signal<bool> m_rom_st_sig;
  sc_signal<sc_uint<W>> m_rom_data_sig;
  sc_signal<sc_uint<W>> m_cntr_out_sig;
};

template <int W>
ProgramEngine<W>::ProgramEngine(sc_module_name name)
  : sc_module(name)
  , j{"jump"}
  , addr{"address"}
  , clk{"clk"}
  , output{"output"}
  , m_counter{"counter"}
  , m_rom{"rom"}
  , m_cntr_out_sig{"signal_counter_out"}
  , m_rom_st_sig{"signal_store_rom"}
  , m_rom_data_sig{"signal_data_rom"}
{
  m_counter.st(j);
  m_counter.clk(clk);
  m_counter.X(addr);
  m_counter.output(m_cntr_out_sig);

  m_rom.st(m_rom_st_sig);
  m_rom.addr(m_cntr_out_sig);
  m_rom.X(m_rom_data_sig);
  m_rom.clk(clk);
  m_rom.output(output);

  m_rom_st_sig.write(false);
  m_rom_data_sig.write(0);
}

template <int W>
ProgramEngine<W>::ProgramEngine(sc_module_name name, std::string path)
  : ProgramEngine(name)
{
  m_rom.loadProgram(path);
}

#endif // PROCCESOR__PROGRAM_ENGINE_H
