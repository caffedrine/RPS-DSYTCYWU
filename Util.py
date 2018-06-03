#
#   Filename    : Util.py
#   Created on  : Jun, 2018
#   Author      : Alex C.
#   Description : Util functions
#

import sys
import time
import datetime

# Enable debug messages
DBG_ENB = True


def dbg(dbg_str, alert = 0, timestamp=0):

    st = datetime.datetime.fromtimestamp(time.time()).strftime('%H:%M:%S.%f')[:-3]
    if dbg_str[0].istitle():
        sys.stdout.write("[" + st + "] ")

    if alert == 1:
        sys.stdout.write("\033[1;31m")  # Set text to red
        sys.stdout.write(dbg_str)
        sys.stdout.write("\033[0;0m") # Reset text
        sys.stdout.flush()
    else:
        sys.stdout.write(dbg_str)
        sys.stdout.flush()
