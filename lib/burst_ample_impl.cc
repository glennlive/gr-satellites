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
#include "burst_ample_impl.h"

namespace gr {
  namespace satellites {

    burst_ample::sptr
    burst_ample::make(const std::string &packet_key,
                      const std::vector<uint8_t> preamble, 
                      const std::vector<uint8_t> interamble,
                      const std::vector<uint8_t> postamble)
    {
      return gnuradio::get_initial_sptr
        (new burst_ample_impl(packet_key, preamble, interamble, postamble));
    }

    /*
     * The private constructor
     */
    burst_ample_impl::burst_ample_impl(const std::string &packet_key,
                       const std::vector<uint8_t> preamble, 
                       const std::vector<uint8_t> interamble,
                       const std::vector<uint8_t> postamble)
      : gr::tagged_stream_block("burst_ample",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)), packet_key),
      d_required_output_len(1),
      d_preamble(preamble),
      d_interamble(interamble),
      d_postamble(postamble)
    {}

    /*
     * Our virtual destructor.
     */
    burst_ample_impl::~burst_ample_impl()
    {
    }

    int
    burst_ample_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = d_required_output_len; 
      return noutput_items ;
    }

    int
    burst_ample_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      // recompute required output length
      d_required_output_len = ninput_items[0] + d_preamble.size() + d_postamble.size();
      if (noutput_items < d_required_output_len) {
        set_min_noutput_items(d_required_output_len);
        return 0;
      }
      d_required_output_len = 1;

      // TODO: add timing to wait for more packets...

      // output our preamble
      memcpy(out, (const void *) &d_preamble[0], d_preamble.size());
      out += d_preamble.size();

      // copy our data
      memcpy(out, in, ninput_items[0]);
      out += ninput_items[0];

      // output our postamble
      memcpy(out, (const void *) &d_postamble[0], d_postamble.size());
      out += d_postamble.size();

      // TODO: add sob/eob tags

      // Tell runtime system how many output items we produced.
      return d_preamble.size() + ninput_items[0] + d_postamble.size();
    }

  } /* namespace satellites */
} /* namespace gr */

