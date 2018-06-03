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
from Sockets import *
from Util import *

# Global configurations
TCP_PORT = 1337


# This is the function which manage network connection!
# it shall be called from another thread
def start_networking_task():

    first_time = True
    while True:
        # If client fd is writable
        if first_time is True or client is None or client.is_alive() == 0:
            if first_time is False:    # Don't print this message on first attempt
                dbg("Broken/lost client connection detected...\n", alert=1)

            # Create start TCP listener in order to get new clients
            dbg("Starting a new TCP server on port %s..." % str(TCP_PORT))
            server = TcpServer(TCP_PORT)
            if server.is_started() is False:  # Do not continue if port already in use
                dbg("failed\n", alert=1)
                dbg("ERROR: " + server.get_last_error() + "\n", alert=1)
                dbg("Retrying in 15 seconds...\n", alert=1)
                sleep(15)
                continue
            dbg("done\n")

            # Wait for clients to connect
            dbg("Waiting for clients to connect...")
            client = server.get_new_client()
            if client is None:
                dbg("failed\n", alert=1)
                dbg("ERROR: " + client.get_last_error + "\n", alert=1)
                dbg("Retrying in 15 seconds...\n", alert=1)
                sleep(15)
                continue
            dbg("done: " + str(client.get_address()) + " " + str(client.get_port()) + "\n")

            # Stop TCP Server as the connection with client was already established
            dbg("Stopping TCP server to prevent multiple clients...")
            server.stop()
            if server.is_started() is True:
                dbg("failed\n", alert=1)
                dbg("WARN: " + server.get_last_error() + "\n", alert=1)
            dbg("done\n")

            # Trigger the flag to know that a client was connected at least
            first_time = False

        # Process received data and stuff
        data = client.recv(64)
        if data == -1:
            continue
        else:
            dbg("RECV: " + str(data) + "\n")

        client.write(data)


# Main function
def main():
    # Launch socket connection in background in a separate thread
    sockets_thread = Thread(target=start_networking_task, args=[] )
    sockets_thread.start()
    sockets_thread.join()

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

