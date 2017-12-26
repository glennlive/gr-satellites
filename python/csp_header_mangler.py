#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2017 Glenn Richardson <glenn@spacequest.com>.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

import array

from gnuradio import gr
import pmt

from csp_header import CSP

class csp_header_mangler(gr.basic_block):
    """
    docstring for block csp_header_mangler
    """
    def __init__(self, crc32, hmac, swap_endian):
        gr.basic_block.__init__(self,
                                name="csp_header_mangler",
                                in_sig=[],
                                out_sig=[])

        self.message_port_register_in(pmt.intern('in'))
        self.set_msg_handler(pmt.intern('in'), self.handle_msg)
        self.message_port_register_out(pmt.intern('out'))

        self.swap_endian = swap_endian
        self.use_crc = crc32
        self.use_hmac = hmac

    def handle_msg(self, msg_pmt):
        """ process incomming packets """
        msg = pmt.cdr(msg_pmt)
        if not pmt.is_u8vector(msg):
            print "[ERROR] Received invalid message type. Expected u8vector"
            return

        packet = array.array("B", pmt.u8vector_elements(msg))
        header = packet[:4]
        if self.swap_endian:
            header.reverse()
        csp = CSP(header)

        # mangle
        csp.crc = 1 if self.use_crc else 0
        csp.hmac = 1 if self.use_crc else 0

        packet = csp.to_bytes() + packet[4:]
        msg_pmt = pmt.cons(pmt.PMT_NIL, pmt.init_u8vector(len(packet), bytearray(packet)))
        self.message_port_pub(pmt.intern('out'), msg_pmt)

