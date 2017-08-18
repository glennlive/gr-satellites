/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "additive_scrambler_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/blocks/pdu.h>

#define MAX_MTU 256*8

namespace gr {
  namespace satellites {

    additive_scrambler_pdu::sptr
    additive_scrambler_pdu::make(int mask, int seed, int len, int bits_per_byte, endianness_t endianness)
    {
      return gnuradio::get_initial_sptr
        (new additive_scrambler_pdu_impl(mask, seed, len, bits_per_byte, endianness));
    }

    /*
     * The private constructor
     */
    additive_scrambler_pdu_impl::additive_scrambler_pdu_impl(int mask, int seed, int len, int bits_per_byte, endianness_t endianness)
      : gr::block("additive_scrambler_pdu",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0)),
        d_lfsr(mask, seed, len),
        d_len(len),
	d_seed(seed),
	d_bits_per_byte(bits_per_byte),
        d_endianness(endianness)
    {
      if (d_bits_per_byte < 1 || d_bits_per_byte > 8) {
	throw std::invalid_argument("bits_per_byte must be in (1, 8)");
      }
      message_port_register_in(pmt::mp("in"));
      message_port_register_out(pmt::mp("out"));
      set_msg_handler(pmt::mp("in"),
	              boost::bind(&additive_scrambler_pdu_impl::scramble_msg, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    additive_scrambler_pdu_impl::~additive_scrambler_pdu_impl()
    {
    }
/*
    void
    additive_scrambler_pdu_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    }*/

    int
    additive_scrambler_pdu_impl::mask() const
    {
      return d_lfsr.mask();
    }

    int
    additive_scrambler_pdu_impl::seed() const
    {
      return d_seed;
    }

    int
    additive_scrambler_pdu_impl::len() const
    {
      return d_len;
    }

    void
    additive_scrambler_pdu_impl::scramble_msg(pmt::pmt_t pdu)
    {
      pmt::pmt_t msg = pmt::cdr(pdu);
      size_t offset(0);
      const uint8_t *in = (const uint8_t *) pmt::uniform_vector_elements(msg, offset);
      int frame_len = pmt::length(msg);
      uint8_t out[MAX_MTU];

      assert(frame_len <= MAX_MTU);

      d_lfsr.reset();
      for (int i=0; i<frame_len; i++) {
	unsigned char scramble_byte = 0x0;
        if (d_endianness == GR_MSB_FIRST) {
          for (int k=0; k < d_bits_per_byte; k++) {
            scramble_byte ^= (d_lfsr.next_bit() << (d_bits_per_byte - 1 - k) );
          }
        } else {
          for (int k=0; k < d_bits_per_byte; k++) {
            scramble_byte ^= (d_lfsr.next_bit() << k);
          }
        }
	out[i] = in[i] ^ scramble_byte;
      }
      message_port_pub(pmt::mp("out"), pmt::cons(pmt::PMT_NIL,
                       pmt::init_u8vector(frame_len, (const uint8_t*)&out[0])));

    }

  } /* namespace satellites */
} /* namespace gr */

