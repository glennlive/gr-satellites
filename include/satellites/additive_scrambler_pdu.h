/* -*- c++ -*- */

#ifndef INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_H
#define INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_H

#include <satellites/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace satellites {

    /*!
     * \brief <+description of block+>
     * \ingroup satellites
     *
     */
    class SATELLITES_API additive_scrambler_pdu : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<additive_scrambler_pdu> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of satellites::additive_scrambler_pdu.
       *
       * To avoid accidental use of raw pointers, satellites::additive_scrambler_pdu's
       * constructor is in a private implementation
       * class. satellites::additive_scrambler_pdu::make is the public interface for
       * creating new instances.
       */
      static sptr make(int mask, int seed, int len, int bits_per_byte);

      virtual int mask() const = 0;
      virtual int seed() const = 0;
      virtual int len() const = 0;
      virtual int bits_per_byte() = 0;
    };

  } // namespace satellites
} // namespace gr

#endif /* INCLUDED_SATELLITES_ADDITIVE_SCRAMBLER_PDU_H */

