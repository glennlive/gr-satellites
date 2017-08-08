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

     public:
      additive_scrambler_pdu_impl(int mask, int seed, int len, int bits_per_byte);
      ~additive_scrambler_pdu_impl();

      int mask() const;
      int seed() const;
      int len() const;
      int bits_per_byte() { return d_bits_per_byte; };

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void msg_handler(pmt::pmt_t pmt_msg);
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_IMPL_H */

