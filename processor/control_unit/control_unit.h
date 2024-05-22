#ifndef PROCESSOR__CONTROL_UNIT_H
#define PROCESSOR__CONTROL_UNIT_H

#include <systemc>
#include "processor/combined_memory/combined_memory.h"
#include "processor/instruction_decoder/instruction_decoder.h"
#include "alu/alu/alu.h"
#include "alu/condition/condition.h"
#include "plumbing/selector/selector.h"

using namespace sc_core;
using namespace sc_dt;

template <int W>
class ControlUnit : public sc_module {
public:
  sc_in<sc_uint<W>> I;
  sc_in<bool> clk;
  sc_out<bool> j;
  sc_out<sc_uint<W>> A;

  ControlUnit(sc_module_name);
public:
  InstructionDecoder<W> m_instruction;
  CombinedMemory<W>     m_memory;
  Selector<W>  m_select1;
  Selector<W>  m_select2;
  Condition<W> m_condition;
  Alu<W>       m_alu;
private:
  sc_signal<bool> m_ci_inst2select1_sig;
  sc_signal<bool> m_sm_inst2select2_sig;
  sc_signal<bool> m_nx_inst2alu_sig;
  sc_signal<bool> m_zx_inst2alu_sig;
  sc_signal<bool> m_ny_inst2alu_sig;
  sc_signal<bool> m_zy_inst2alu_sig;
  sc_signal<bool> m_f_inst2alu_sig;
  sc_signal<bool> m_no_inst2alu_sig;
  sc_signal<bool> m_a_inst2mem_sig;
  sc_signal<bool> m_d_inst2mem_sig;
  sc_signal<bool> m_ram_inst2mem_sig;
  sc_signal<bool> m_gt_inst2cond_sig;
  sc_signal<bool> m_eq_inst2cond_sig;
  sc_signal<bool> m_lt_inst2cond_sig;
  sc_signal<sc_uint<W>> m_W_inst2select1_sig;
  
  sc_signal<sc_uint<W>> m_X_select12mem_sig;

  sc_signal<sc_uint<W>> m_select2aluY_sig;

  sc_signal<sc_uint<W>> m_data_alu2cond_sig;

  sc_signal<sc_uint<W>> m_A_mem2select2_sig;
  sc_signal<sc_uint<W>> m_D_mem2aluX_sig;
  sc_signal<sc_uint<W>> m_RAM_mem2select2_sig;

  void Areg_cb();
};


template <int W>
ControlUnit<W>::ControlUnit(sc_module_name name)
  : sc_module(name)
  , I{"I"}
  , clk{"clk"}
  , j{"j"}
  , A{"A"}
  , m_instruction{"instruction"}
  , m_memory{"memory"}
  , m_select1{"select1"}
  , m_select2{"select2"}
  , m_condition{"condition"}
  , m_alu{"alu"}
  , m_ci_inst2select1_sig{"signal_ci_inst2select1"}
  , m_sm_inst2select2_sig{"signal_sm_inst2select2"}
  , m_zx_inst2alu_sig{"signal_zx_inst2alu"}
  , m_nx_inst2alu_sig{"signal_nx_inst2alu"}
  , m_zy_inst2alu_sig{"signal_zy_inst2alu"}
  , m_ny_inst2alu_sig{"signal_ny_inst2alu"}
  , m_f_inst2alu_sig{"signal_f_inst2alu"}
  , m_no_inst2alu_sig{"signal_no_inst2alu"}
  , m_d_inst2mem_sig{"signal_d_inst2mem"}
  , m_a_inst2mem_sig{"signal_a_inst2mem"}
  , m_ram_inst2mem_sig{"signal_ram_inst2mem"}
  , m_gt_inst2cond_sig{"signal_gt_inst2cond"}
  , m_eq_inst2cond_sig{"signal_eq_inst2cond"}
  , m_lt_inst2cond_sig{"signal_lt_inst2cond"}
  , m_data_alu2cond_sig{"signal_data_alu2cond"}
  , m_select2aluY_sig{"signal_mem2aluY"}
  , m_A_mem2select2_sig{"signal_A_mem2select2"}
  , m_D_mem2aluX_sig{"signal_D_mem2aluX"}
  , m_RAM_mem2select2_sig{"signal_A*_mem2select2"}
  , m_W_inst2select1_sig{"signal_W_inst2select1"}
  , m_X_select12mem_sig{"signal_X_select1_to_mem"}
{
  m_instruction.I(I);
  m_instruction.ci(m_ci_inst2select1_sig);
  m_instruction.sm(m_sm_inst2select2_sig);
  m_instruction.zx(m_zx_inst2alu_sig);
  m_instruction.nx(m_nx_inst2alu_sig);
  m_instruction.zy(m_zy_inst2alu_sig);
  m_instruction.ny(m_ny_inst2alu_sig);
  m_instruction.f(m_f_inst2alu_sig);
  m_instruction.no(m_no_inst2alu_sig);
  m_instruction.a(m_a_inst2mem_sig);
  m_instruction.d(m_d_inst2mem_sig);
  m_instruction.ram(m_ram_inst2mem_sig);
  m_instruction.gt(m_gt_inst2cond_sig);
  m_instruction.eq(m_eq_inst2cond_sig);
  m_instruction.lt(m_lt_inst2cond_sig);
  m_instruction.W(m_W_inst2select1_sig);

  m_select1.d0(m_W_inst2select1_sig);
  m_select1.d1(m_data_alu2cond_sig);
  m_select1.s(m_ci_inst2select1_sig);
  m_select1.output(m_X_select12mem_sig);
  
  m_select2.s(m_sm_inst2select2_sig);
  m_select2.d0(m_A_mem2select2_sig);
  m_select2.d1(m_RAM_mem2select2_sig);
  m_select2.output(m_select2aluY_sig);
  
  m_condition.lt(m_lt_inst2cond_sig);
  m_condition.eq(m_eq_inst2cond_sig);
  m_condition.gt(m_gt_inst2cond_sig);
  m_condition.X(m_data_alu2cond_sig);
  m_condition.output(j);

  m_alu.zx(m_zx_inst2alu_sig);
  m_alu.nx(m_nx_inst2alu_sig);
  m_alu.zy(m_zy_inst2alu_sig);
  m_alu.ny(m_ny_inst2alu_sig);
  m_alu.f(m_f_inst2alu_sig);
  m_alu.no(m_no_inst2alu_sig);
  m_alu.X(m_D_mem2aluX_sig);
  m_alu.Y(m_select2aluY_sig);
  m_alu.output(m_data_alu2cond_sig);

  m_memory.st_a(m_a_inst2mem_sig);
  m_memory.st_d(m_d_inst2mem_sig);
  m_memory.st_ram(m_ram_inst2mem_sig);
  m_memory.X(m_X_select12mem_sig);
  m_memory.clk(clk);
  m_memory.A(m_A_mem2select2_sig);
  m_memory.D(m_D_mem2aluX_sig);
  m_memory.RAM(m_RAM_mem2select2_sig);

  SC_HAS_PROCESS(ControlUnit);

  SC_METHOD(Areg_cb);
  sensitive << m_memory.A;
}

template <int W>
void ControlUnit<W>::Areg_cb() {
  A.write(m_memory.A.read());
}

#endif // PROCESSOR__CONTROL_UNIT_H

