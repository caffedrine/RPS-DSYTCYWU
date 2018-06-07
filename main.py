#
#   Filename    : main.py
#   Created on  : Jun, 2018
#   Author      : Alex C
#   Description : RPI Testbench
#

# Standard modules
from threading import Thread
from time import sleep
import sys
import os

# Owned modules
from NetworkService import *
from Sockets import *
from Util import *


def tcp_recv_callback(client, data):
    dbg("[" + str(client.get_address()) + ":" + str(client.get_port()) + "] " + str(data) + "\n")


# Main function
def main():
    # Start TCP networking on port 1337
    start_networking_task_background(1337, tcp_recv_callback)

    while True:
        a = 0


# Execute main function
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print >> sys.stderr, 'Keyboard interrupt detected! Program will end now... '
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)

