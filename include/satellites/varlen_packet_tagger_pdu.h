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

#ifndef INCLUDED_SATELLITES_VARLEN_TAGGER_PACKET_PDU_H
#define INCLUDED_SATELLITES_VARLEN_TAGGER_PACKET_PDU_H

#include <satellites/api.h>
#include <gnuradio/block.h>
#include <gnuradio/endianness.h>
#include <pmt/pmt.h>

namespace gr {
  namespace satellites {

    /*!
     * \brief extract packet size from bit stream and creat pdu of packet data.
     * \ingroup satellites
     *
     */
    class SATELLITES_API varlen_packet_tagger_pdu : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<varlen_packet_tagger_pdu> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of satellites::varlen_packet_tagger_pdu.
       *
       * To avoid accidental use of raw pointers, satellites::varlen_packet_tagger_pdu's
       * constructor is in a private implementation
       * class. satellites::varlen_packet_tagger_pdu::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &sync_key, const std::string &packet_key, int length_field_size, int max_packet_size, endianness_t header_endianness, endianness_t output_endianness, bool use_golay, int bits_per_byte);
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_VARLEN_PACKET_PDU_H */

