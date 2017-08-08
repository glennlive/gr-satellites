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
#include <cstdio>
#include <iostream>
#include "varlen_packet_tagger_pdu_impl.h"

extern "C" {
#include <fec.h>
#include "golay24.h"
}

#define MAX_MTU 256

unsigned char dout[MAX_MTU];
namespace gr {
  namespace satellites {

    varlen_packet_tagger_pdu::sptr
    varlen_packet_tagger_pdu::make(const std::string &sync_key,
                               const std::string &packet_key,
                               int length_field_size,
                               int max_packet_size,
                               endianness_t header_endianness,
                               endianness_t output_endianness,
                               bool use_golay)
    {
      return gnuradio::get_initial_sptr
        (new varlen_packet_tagger_pdu_impl(sync_key,
                                        packet_key,
                                        length_field_size,
                                        max_packet_size,
                                        header_endianness,
                                        output_endianness,
                                        use_golay));
    }

    varlen_packet_tagger_pdu_impl::varlen_packet_tagger_pdu_impl(
        const std::string &sync_key,
        const std::string &packet_key,
        int length_field_size,
        int max_packet_size,
        endianness_t header_endianness,
        endianness_t output_endianness,
        bool use_golay) : gr::block("varlen_packet_tagger_pdu",
                        io_signature::make(1, 1, sizeof(char)),
                        io_signature::make(0, 0, 0)),
      d_header_length(length_field_size),
      d_mtu(max_packet_size),
      d_hend(header_endianness),
      d_oend(output_endianness),
      d_use_golay(use_golay),
      d_have_sync(false)
    {

      message_port_register_out(pmt::mp("out"));

      d_sync_tag = pmt::string_to_symbol(sync_key);
      d_packet_tag = pmt::string_to_symbol(packet_key);
      d_ninput_items_required = d_header_length + 1;

      if (d_use_golay)
          d_header_length = 24;
    }


    varlen_packet_tagger_pdu_impl::~varlen_packet_tagger_pdu_impl()
    {
    }

    int
    varlen_packet_tagger_pdu_impl::bits2len(const unsigned char *in)
    {
      // extract the packet length from the header
      int ret = 0;
      if (d_hend == GR_MSB_FIRST) {
        for (int i=0; i<d_header_length; i++) {
          ret = (ret<<0x01)+in[i];
        }
      } else {
        for (int i=d_header_length-1; i>=0; i--) {
          ret = (ret<<0x01)+in[i];
        }
      }
      return ret;
    }

    void
    varlen_packet_tagger_pdu_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {/*
      unsigned ninputs = ninput_items_required.size();
      for(unsigned i = 0; i < ninputs; i++)
        ninput_items_required[i] = d_ninput_items_required;*/
    }

    int
    varlen_packet_tagger_pdu_impl::general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      //unsigned char *out = (unsigned char *) output_items[0];
      int packet_len = 0;
      std::vector<tag_t> tags;
      register uint32_t golay_field;
      int golay_res;

      unsigned char *p;
      
      if (d_have_sync) {
        if (d_header_length > ninput_items[0]) {
          // not enough data yet
          return 0;
        }
        
        if (d_use_golay) {
          golay_field = bits2len(in);
          golay_res = decode_golay24(&golay_field);
          if (golay_res >= 0) {
            packet_len = (0xFFF & golay_field);
          } else {
            GR_LOG_WARN(d_debug_logger, "Golay decode failed.");
            d_have_sync = false;
            consume_each(1); // skip ahead
            return 0;
          }          

        } else { 
          packet_len = bits2len(in);
        }

        if (packet_len > d_mtu) {
          GR_LOG_WARN(d_debug_logger,
                      boost::format("Packet length %d > mtu %d.") \
                                    % packet_len % d_mtu );
          d_have_sync = false;
          consume_each(1); // skip ahead
          return 0;
        }
        
        d_ninput_items_required = d_header_length + packet_len;


        if (ninput_items[0] >= packet_len + d_header_length) {

          if (d_use_golay) {
            GR_LOG_DEBUG(d_debug_logger,
                         boost::format("Header: 0x%06x, Len: %d") \
                                       % (0xFFFFFF & golay_field) % (0xFFF & packet_len) );
            if (golay_res >= 0 ) {
              GR_LOG_DEBUG(d_debug_logger,
                           boost::format("Golay decoded. Errors: %d, Length: %d") \
                                         % golay_res % packet_len);
            }
          }
      
          p = (unsigned char*) &in[d_header_length];
          switch(d_oend) {
          default:
          case GR_MSB_FIRST:
            for (int i=0; i<packet_len; i++) {
              uint8_t tmp = 0;
              for (int j=0; j<8; j++ ) {
                tmp = (tmp<<1) | (0x01 & p[j]);
              }
              p += 8;
              dout[i] = tmp;
            }
            break;
          case GR_LSB_FIRST:
            for (int i=0; i<packet_len; i++) {
              uint8_t tmp = 0;
              for (int j=7; j>=0; j-- ) {
                tmp = (tmp<<1) | (0x01 & p[j]);
              }
              p += 8;
              dout[i] = tmp;
            }
            break;
          }
	  message_port_pub(pmt::mp("out"),
			   pmt::cons(pmt::PMT_NIL,
				     pmt::init_u8vector(packet_len, dout)));
          d_have_sync = false;

          // consuming only the header allows for
          // ... multiple syncs per 'packet', 
          // ... in case the sync was incorrectly tagged
          consume_each(d_header_length);
          d_ninput_items_required = d_header_length + 1;
          return 0;
        } 

      } else {
        // find the next sync tag, drop all other data
        get_tags_in_range(tags, 0, nitems_read(0),
                          nitems_read(0) + ninput_items[0],
                          d_sync_tag);
        if (tags.size() > 0) {
          d_have_sync = true;
          consume_each( tags[0].offset - nitems_read(0) );
        } else {
          consume_each(ninput_items[0]);
        }
      }
      return 0;
    }

  } /* namespace satellites */
} /* namespace gr */
