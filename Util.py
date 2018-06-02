#
#   Filename    : Util.py
#   Created on  : Jun, 2018
#   Author      : Alex C.
#   Description : Util functions
#

import sys

# Enable debug messages
DBG_ENB = True


def dbg(dbg_str, alert = 0):
    if alert == 1:
        sys.stdout.write("\033[1;31m")  # Set text to red
        sys.stdout.write(dbg_str)
        sys.stdout.write("\033[0;0m") # Reset text
        sys.stdout.flush()
    else:
        sys.stdout.write(dbg_str)
        sys.stdout.flush()
