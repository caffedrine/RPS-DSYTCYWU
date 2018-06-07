#
#   Filename    : NetworkService.py
#   Created on  : Jun, 2018
#   Author      : Alex C
#   Description : Module to manage TCP connection(s)
#

from threading import Thread

from Sockets import *
from Util import *

def tcp_recv_internal_callback(client, data):
    dbg("[" + client.get_address + ":" + client.get_address + "] " + str(data) + "\n")

# This is the function which manage network connection!
# it shall be called from another thread
def start_networking_task(TCP_PORT, recvCallback=tcp_recv_internal_callback):
    first_time = True
    while True: # used to keep connection link
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
                dbg("ERROR: " + server.get_last_error() + "...\n", alert=1)
                dbg("Server will restart in 15 seconds...\n", alert=1)
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
            recvCallback(client, data)

def start_networking_task_background(TCP_PORT, recvCallback=tcp_recv_internal_callback):
    # Launch socket connection in background in a separate thread
    sockets_thread = Thread(target=start_networking_task, args=[TCP_PORT, recvCallback] )
    sockets_thread.daemon = True
    sockets_thread.start()
    sockets_thread.join()