#include <cmath>

#include "GaussianFilter.h"

GaussianFilter::GaussianFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// gaussian mask
const double mask[MASK_N][MASK_X][MASK_Y] = 
{
  1.0/273, 4.0/273, 7.0/273, 4.0/273, 1.0/273,
  4.0/273, 16.0/273, 26.0/273, 16.0/273, 4.0/273,
  7.0/273, 26.0/273, 41.0/273, 26.0/273, 7.0/273,
  4.0/273, 16.0/273, 26.0/273, 16.0/273, 4.0/273,
  1.0/273, 4.0/273, 7.0/273, 4.0/273, 1.0/273,
};


void GaussianFilter::do_filter() {
  while (true) {
    for (unsigned int i = 0; i < MASK_N; ++i) {
      val[i] = 0;
    }
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        //0.299 R + 0.587 G + 0.114 B
        unsigned char gray = (0.299 * i_r.read() + 0.587 * i_g.read() + 0.114 * i_b.read());
        for (unsigned int i = 0; i != MASK_N; ++i) {
          val[i] += gray * mask[i][u][v];
        }
      }
    }
    
    double total = 0;
    for (unsigned int i = 0; i != MASK_N; ++i) {
      total += val[i] * val[i];
    }

    int result = (int)(std::sqrt(total));
    o_result.write(result);
    wait(10); //emulate module delay
  }
}
