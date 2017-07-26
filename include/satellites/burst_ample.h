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

#ifndef INCLUDED_SATELLITES_BURST_AMPLE_H
#define INCLUDED_SATELLITES_BURST_AMPLE_H

#include <satellites/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace satellites {

    /*!
     * \brief pad packets with pre- and post-ambles needed for hardware PAs
     *
     * \ingroup satellites
     *
     */
    class SATELLITES_API burst_ample : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<burst_ample> sptr;

      /*!
       * \param packet_key    tag key used to mark packets
       * \param preamble      bits before packets
       * \param interamble    bits between packets
       * \param postamble     bits after packet
       */
       static sptr make (const std::string &packet_key,
                         const std::vector<uint8_t> preamble, 
                         const std::vector<uint8_t> interamble,
                         const std::vector<uint8_t> postamble);
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_BURST_AMPLE_H */

