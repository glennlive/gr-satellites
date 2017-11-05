/* -*- c++ -*- */
/*
 * Copyright 2017 Glenn Richardson <glenn@spacequest.com> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "digital_upsampler_impl.h"

namespace gr {
  namespace satellites {

    digital_upsampler::sptr
    digital_upsampler::make(unsigned int input_rate, unsigned int output_rate)
    {
      return gnuradio::get_initial_sptr
        (new digital_upsampler_impl(input_rate, output_rate));
    }

    /*
     * The private constructor
     */
    digital_upsampler_impl::digital_upsampler_impl(unsigned int input_rate, unsigned int output_rate)
      : gr::block("digital_upsampler",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
      d_input_rate(input_rate),
      d_output_rate(output_rate),
      d_input_sample_time(1.0/input_rate),
      d_output_sample_time(1.0/output_rate),
      d_remainder_time(0.0)
    {
      set_relative_rate(d_output_rate/d_input_rate);
      set_output_multiple(1024);
    }


    /*
     * Our virtual destructor.
     */
    digital_upsampler_impl::~digital_upsampler_impl()
    {
    }



    /* input rate setter */
    void
    digital_upsampler_impl::set_input_rate(unsigned int input_rate)
    {
      GR_LOG_DEBUG(d_debug_logger, boost::format("gr_digital_upsampler::set_input_rate(%d)") % input_rate);
      d_input_rate = input_rate;
      set_relative_rate(d_output_rate/d_input_rate);
      d_input_sample_time = 1.0 / input_rate;
    }

    /*
     * Our forecast method 
     */
    void digital_upsampler_impl::forecast(int noutput_items,
                                          gr_vector_int &ninput_items_required)
    {
      int n = noutput_items * d_input_rate / d_output_rate;
      ninput_items_required[0] = (n==0 ? 1 : n);
    }


    int
    digital_upsampler_impl::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        float                elapsed_time;
        int                  n_in_items = ninput_items[0];
        int                  i; // input item counter
        int                  o; // output item counter

        elapsed_time = d_remainder_time;  // get any leftover time from last call

        // Loop thru all output items 'till we run out of output or input
        i = 0;
        for (o = 0; (o < noutput_items) && (i < n_in_items); o++)
        {    
          elapsed_time += d_output_sample_time;  // one output sample time elapsed
          if(elapsed_time >= d_input_sample_time) // see if we've started a new bit
          {
            elapsed_time -= d_input_sample_time;  // adjust by one input bit time
            i++;                                  // move to next input bit
          }
          out[o] = in[i];                        // copy input bit value to output
        }

        d_remainder_time = elapsed_time;  // save any remainder time for next call

        // Tell runtime system how many input items we consumed on
        // each input stream.
          consume_each (i);

        // Tell runtime system how many output items we produced.
        GR_LOG_DEBUG(d_debug_logger, boost::format(
            "gr_digital_upsampler o [%d], i [%d], noutput_items [%d] n_in_items [%d]\n") % o % i % noutput_items % n_in_items);
        
        return o;
    }

  } /* namespace satellites */
} /* namespace gr */

