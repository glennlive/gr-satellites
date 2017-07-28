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
#include "encode_rs_impl.h"

extern "C" {
#include <fec.h>
}

#define RS_MTU 255
#define RS_BASIS_CONVENTIONAL 0

namespace gr {
  namespace satellites {

    encode_rs::sptr
    encode_rs::make(int basis)
    {
      return gnuradio::get_initial_sptr
        (new encode_rs_impl(basis));
    }

    /*
     * The private constructor
     */
    encode_rs_impl::encode_rs_impl(int basis)
      : gr::block("encode_rs",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0)),
      d_basis(basis)
    {
      message_port_register_out(pmt::mp("out"));
      message_port_register_in(pmt::mp("in"));
      set_msg_handler(pmt::mp("in"),
          boost::bind(&encode_rs_impl::msg_handler, this, _1));
    }    

    /*
     * Our virtual destructor.
     */
    encode_rs_impl::~encode_rs_impl()
    {
    }

    void
    encode_rs_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    }

    int
    encode_rs_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      return 0;
    }

    void
    encode_rs_impl::msg_handler (pmt::pmt_t pmt_msg) 
    {
      pmt::pmt_t msg = pmt::cdr(pmt_msg);
      uint8_t data[RS_MTU];
      int frame_len = pmt::length(msg);
      size_t offset(0);
      int ret;

      assert(frame_len <= RS_MTU);

      memset(data, 0, RS_MTU);
      memcpy(data, pmt::uniform_vector_elements(msg, offset), frame_len);

      if (d_basis == RS_BASIS_CONVENTIONAL) {
        encode_rs_8(data, &data[frame_len], RS_MTU - 32 - frame_len);
      } else {
        encode_rs_ccsds(data, &data[frame_len], RS_MTU - 32 - frame_len);
      }


      frame_len += 32;
      message_port_pub(pmt::mp("out"),
                       pmt::cons(pmt::PMT_NIL,
                       pmt::init_u8vector(frame_len, data)));

      GR_LOG_DEBUG(d_debug_logger,
                   boost::format("Reed-Solomon encoded packet. len:%d.") \
                   % frame_len );
    }

  } /* namespace satellites */
} /* namespace gr */

