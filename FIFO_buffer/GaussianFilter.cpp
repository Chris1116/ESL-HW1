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
{{
  {1.0/273, 4.0/273, 7.0/273, 4.0/273, 1.0/273},
  {4.0/273, 16.0/273, 26.0/273, 16.0/273, 4.0/273},
  {7.0/273, 26.0/273, 41.0/273, 26.0/273, 7.0/273},
  {4.0/273, 16.0/273, 26.0/273, 16.0/273, 4.0/273},
  {1.0/273, 4.0/273, 7.0/273, 4.0/273, 1.0/273}
}};


void GaussianFilter::do_filter() {
  // Initializes a 5x5 buffer to store pixel values.
  unsigned char buffer[5][5] = {0}; 
  while (true) {
    // Initializes an array to store the filtered value (the sum of the products of the mask and the pixel values).
    for (unsigned int i = 0; i < MASK_N; ++i) {
      val[i] = 0;
    }
    
    // If there's new data in i_buffer 
    if(i_buffer.read()) {
      // Populate the buffer with grayscale values calculated from input RGB values and apply filtering
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          unsigned char gray = round((i_r.read() * 0.299 + i_g.read() * 0.587 + i_b.read() * 0.114));
          buffer[v][u] = gray;
          for (unsigned int i = 0; i < MASK_N; ++i) {
            val[i] += (double)gray * mask[i][v][u];
          }
        }
      }
    }
    // Otherwise 
    else {
      // shifts the buffer contents to emulate a shift register and reads a new pixel
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          if(u != (MASK_X - 1)) {
            buffer[v][u] = buffer[v][u + 1]; // emulate shift register (buffer shifing left)
            // Apply Gaussian mask to the shifted buffer
            for (unsigned int i = 0; i < MASK_N; ++i) {
              val[i] += (double)buffer[v][u] * mask[i][v][u];
            }
          }
          else {
            unsigned char gray = round((i_r.read() * 0.299 + i_g.read() * 0.587 + i_b.read() * 0.114));
            buffer[v][u] = gray;
            // Apply Gaussian mask to the new pixel data
            for (unsigned int i = 0; i < MASK_N; ++i) {
              val[i] += (double)gray * mask[i][v][u];
            }
          }
        }
      }
    }

    double total = 0;
    for (unsigned int i = 0; i < MASK_N; ++i) {
      total += val[i] * val[i];
    }

    //int result = (int)(std::sqrt(total));
    int result = static_cast<int>(sqrt(total));
    o_result.write(result);
    wait(10); //emulate module delay
  }
}




       



