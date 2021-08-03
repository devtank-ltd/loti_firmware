""" LoTI binding package"""
## @package loti_firmware
#
# LoTI firmware wrapper
#
from __future__ import print_function, absolute_import

import sys

VERSION = 1.0

##
# @brief Publically exportable modules
#
__all__ = [
    "binding",
]

##
# @brief Import all variables in these modules into other namespaces
#
from .binding import io_board_py_t, uart_t, io_t, adc_t, pps_t
from .binding import debug_print as io_debug_print, set_debug_print as io_set_debug_print, get_debug_print as io_get_debug_print
