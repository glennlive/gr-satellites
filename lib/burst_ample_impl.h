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

#ifndef INCLUDED_SATELLITES_BURST_AMPLE_IMPL_H
#define INCLUDED_SATELLITES_BURST_AMPLE_IMPL_H

#include <satellites/burst_ample.h>

namespace gr {
  namespace satellites {

    class burst_ample_impl : public burst_ample
    {
     private:
       std::vector<uint8_t> d_preamble;   // bits to prepend to packet
       std::vector<uint8_t> d_postamble;  // bits to append to packet
       std::vector<uint8_t> d_interamble; // bits to use between packets
       int d_required_output_len; // number of output items required

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      burst_ample_impl(const std::string &packet_key,
                       const std::vector<uint8_t> preamble, 
                       const std::vector<uint8_t> interamble,
                       const std::vector<uint8_t> postamble);
      ~burst_ample_impl();

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_BURST_AMPLE_IMPL_H */

