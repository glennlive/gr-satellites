/* -*- c++ -*- */

#define SATELLITES_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "satellites_swig_doc.i"

%{
#include "satellites/decode_rs.h"
#include "satellites/ao40_syncframe.h"
#include "satellites/ao40_deinterleaver.h"
#include "satellites/ao40_rs_decoder.h"
#include "satellites/ax100_decode.h"
#include "satellites/u482c_decode.h"
#include "satellites/lilacsat1_demux.h"
#include "satellites/additive_scrambler_pdu.h"
#include "satellites/varlen_packet_framer.h"
#include "satellites/varlen_packet_tagger.h"
#include "satellites/varlen_packet_tagger_pdu.h"
#include "satellites/burst_ample.h"
#include "satellites/encode_rs.h"
%}

%include "satellites/decode_rs.h"
GR_SWIG_BLOCK_MAGIC2(satellites, decode_rs);

%include "satellites/ao40_syncframe.h"
GR_SWIG_BLOCK_MAGIC2(satellites, ao40_syncframe);
%include "satellites/ao40_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(satellites, ao40_deinterleaver);
%include "satellites/ao40_rs_decoder.h"
GR_SWIG_BLOCK_MAGIC2(satellites, ao40_rs_decoder);
%include "satellites/ax100_decode.h"
GR_SWIG_BLOCK_MAGIC2(satellites, ax100_decode);
%include "satellites/u482c_decode.h"
GR_SWIG_BLOCK_MAGIC2(satellites, u482c_decode);
%include "satellites/lilacsat1_demux.h"
GR_SWIG_BLOCK_MAGIC2(satellites, lilacsat1_demux);
%include "satellites/additive_scrambler_pdu.h"
GR_SWIG_BLOCK_MAGIC2(satellites, additive_scrambler_pdu);
%include "satellites/varlen_packet_tagger.h"
GR_SWIG_BLOCK_MAGIC2(satellites, varlen_packet_tagger);
%include "satellites/varlen_packet_tagger_pdu.h"
GR_SWIG_BLOCK_MAGIC2(satellites, varlen_packet_tagger_pdu);
%include "satellites/varlen_packet_framer.h"
GR_SWIG_BLOCK_MAGIC2(satellites, varlen_packet_framer);
%include "satellites/burst_ample.h"
GR_SWIG_BLOCK_MAGIC2(satellites, burst_ample);
%include "satellites/encode_rs.h"
GR_SWIG_BLOCK_MAGIC2(satellites, encode_rs);
