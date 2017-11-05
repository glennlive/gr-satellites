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

#ifndef INCLUDED_SATELLITES_DIGITAL_UPSAMPLER_IMPL_H
#define INCLUDED_SATELLITES_DIGITAL_UPSAMPLER_IMPL_H

#include <satellites/digital_upsampler.h>

namespace gr {
  namespace satellites {

    class digital_upsampler_impl : public digital_upsampler
    {
     private:
      unsigned int d_input_rate;
      unsigned int d_output_rate;
      float  d_input_sample_time;
      const float  d_output_sample_time;
      float        d_remainder_time;

     public:
      digital_upsampler_impl(unsigned int input_rate, unsigned int output_rate);
      ~digital_upsampler_impl();
      void forecast ( int             noutput_items,
                  gr_vector_int   &ninput_items_required);
      
      void set_input_rate(unsigned int input_rate);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
	               gr_vector_const_void_star &input_items,
          	       gr_vector_void_star &output_items);
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_DIGITAL_UPSAMPLER_IMPL_H */

