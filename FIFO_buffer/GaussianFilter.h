#ifndef GAUSS_FILTER_H_
#define GAUSS_FILTER_H_

#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class GaussianFilter : public sc_module {
public:
    sc_in_clk i_clk;  // Clock input
    sc_in<bool> i_rst;  // Reset input

    // FIFO channels for RGB color inputs
    sc_fifo_in<unsigned char> i_r;
    sc_fifo_in<unsigned char> i_g;
    sc_fifo_in<unsigned char> i_b;

    // FIFO channel for input buffer control signal
    sc_fifo_in<bool> i_buffer; 

    // FIFO channel for outputting filter results
    sc_fifo_out<int> o_result;

    // Constructor
    SC_HAS_PROCESS(GaussianFilter);
    GaussianFilter(sc_module_name n);
    ~GaussianFilter() = default;

private:
    void do_filter();  // Filter processing function
    int val[MASK_N];   // Array for storing intermediate filter results
    // Consider re-introducing buffer for RGB values if needed for optimization
    // unsigned char pixel_buffer[5][5][3];
};

#endif // GAUSS_FILTER_H_
