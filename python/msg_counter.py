#!/usr/bin/env python
# -*- coding: utf-8 -*-

from gnuradio import gr
import pmt

class msg_counter(gr.basic_block):
    """
    docstring for block msg_counter
    """
    def __init__(self):
        gr.basic_block.__init__(self,
            name="msg_counter",
            in_sig=[],
            out_sig=[])
        self.message_port_register_in(pmt.intern('in'))
        self.set_msg_handler(pmt.intern('in'), self.count_msg)
        self.message_port_register_out(pmt.intern('out'))
        self._count = 0

    def count_msg(self, msg_pmt):
        msg = pmt.cdr(msg_pmt)
        if not pmt.is_u8vector(msg):
            print "ERRR!"
            return
        self._count += 1
        print "Count:", self._count
        self.message_port_pub(pmt.intern('out'), msg_pmt)
