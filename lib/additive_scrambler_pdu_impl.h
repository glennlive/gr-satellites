/* -*- c++ -*- */

#ifndef INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_IMPL_H
#define INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_IMPL_H

#include <satellites/additive_scrambler_pdu.h>
#include <gnuradio/digital/lfsr.h>

namespace gr {
  namespace satellites {

    class additive_scrambler_pdu_impl : public additive_scrambler_pdu
    {
     private:
      digital::lfsr d_lfsr;
      int d_len;
      int d_seed;
      int d_bits_per_byte;
      endianness_t d_endianness;

      void scramble_msg(pmt::pmt_t pmt_msg);

     public:
      additive_scrambler_pdu_impl(int mask, int seed, int len, int bits_per_byte, endianness_t endianness);
      ~additive_scrambler_pdu_impl();

      int mask() const;
      int seed() const;
      int len() const;
      int bits_per_byte() { return d_bits_per_byte; };

    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_IMPL_H */

