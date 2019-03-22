#ifndef MEMORY_RAM_H_
#define MEMORY_RAM_H_

#include <systemc>
#include <vector>
using namespace sc_core;

template<int W_DATA, int W_ADDR>
class Ram : public sc_module {
public:
  sc_in<sc_dt::sc_uint<W_DATA>> X;
  sc_in<sc_dt::sc_uint<W_ADDR>> addr;
  sc_in<bool> st;
  sc_in<bool> clk;
  sc_out<sc_dt::sc_uint<W_DATA>> output;
  Ram(sc_module_name);
private:
  void clk_cb();
  std::vector<sc_dt::sc_uint<W_DATA>> m_data;
};

template<int W_DATA, int W_ADDR>
Ram<W_DATA,W_ADDR>::Ram(sc_module_name name)
  : sc_module(name)
  , X{"data"}
  , addr{"address"}
  , st{"store"}
  , clk{"clock"}
  , output{"out"}
  , m_data( 1 << W_ADDR, 0 )
{
  SC_HAS_PROCESS(Ram);

  SC_METHOD(clk_cb);
  sensitive << clk.neg();
}

template<int W_DATA, int W_ADDR>
void Ram<W_DATA, W_ADDR>::clk_cb() {
  if (st.read()) {
    if (addr.read() < m_data.size()) {
      m_data[addr.read()] = X.read();
    }
  }
  sc_dt::sc_uint<W_DATA> data = 0xDEADBAAD;
  if (addr.read() < m_data.size()) {
    data = m_data[addr.read()];
  }
  
  output.write ( data );
}

#endif // MEMORY_RAM_H_
