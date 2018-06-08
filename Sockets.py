#
#   Filename    : Sockets.py
#   Created on  : Jun, 2018
#   Author      : Alex C.
#   Description : Python sockets wrapper
#

import socket
import select
import sys
from sys import platform as _platform
from time import sleep
from exceptions import *

# Local TCP client connection
class TcpClient:
    def __init__(self, fd, port, address):

        # Client fd descriptor
        self.__hSocket = fd
        self.__hSocket.setblocking(1)
        self.set_keepalive(self.__hSocket)

        # Client info
        self.__port = port
        self.__address = address

        # Variable used to store errors
        self.__last_error = ""

    # Check whether connection is alive or not
    def is_alive(self):
        # Send a ping packet to make sure client is alive
        if self.__hSocket is None:
            return False

        # Use select to detect any sudden disconnection
        try:
            select.select([self.__hSocket, ], [self.__hSocket, ], [], 5)
            return True
        # except select.error:
        #     self.hSocket.close()
        #     return 0
        except:
            self.__hSocket.close()
            return False

    # Return the number of written bytes
    def write(self, data):
        if self.__hSocket is None:
            self.__set_last_error("invalid client socket fd")
            return -1

        if self.is_alive() is 0:
            self.__set_last_error("client connection broken")
            return -1

        if data:
            try:
                self.__hSocket.sendall(str(data))
                return len(str(data))
            except Exception as e:
                self.__set_last_error("socket write() error: " + str(e))
                return -1
        else:
            return 0

    # Read data from client
    def recv(self, data_count):
        if self.__hSocket is None:
            self.__set_last_error("invalid client socket fd")
            return -1

        if self.is_alive() is 0:
            self.__set_last_error("client connection broken")
            return -1

        try:
            # Read from client
            data = self.__hSocket.recv(data_count)

            if data == '':
                self.__hSocket.close()
                return -1

            # Return data we just got from client
            return data
        except Exception as e:
            self.__set_last_error("socket recv() error" + str(e))
            return -1

    # Close connection
    def close(self):
        # Clean up the connection
        try:
            self.__hSocket.close()
            self.__hSocket = None
            return True
        except:
            return False

    # Function to set TCP keep-alive
    def set_keepalive(self, sock, after_idle_sec=1, interval_sec=3, max_fails=5):
        # If platform is windows
        if _platform == "win32" or _platform == "win64":
            sock.ioctl(socket.SIO_KEEPALIVE_VALS, (1, 10000, 3000))
        else:
            # Set TCP keepalive on an open socket.
            #
            # It activates after 1 second (after_idle_sec) of idleness,
            # then sends a keepalive ping once every 3 seconds (interval_sec),
            # and closes the connection after 5 failed ping (max_fails), or 15 seconds

            sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
            sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, after_idle_sec)
            sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, interval_sec)
            sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, max_fails)

    # Return client's port
    def get_port(self):
        return self.__port

    # Return client's ip address
    def get_address(self):
        return self.__address

    # Set last error
    def __set_last_error(self, err_str):
        self.__last_error = err_str

    # Get last error
    def get_last_error(self):
        return self.__last_error


# TCP Server Connection
class TcpServer:
    def __init__(self, port):
        # Store given port
        self.__port = port

        # Socket fd
        self.__server = socket.socket

        # Variable which store the server status: started/stopped
        self.__server_started = False

        # Store last error
        self.__last_error = ""

        # Start TCP server on given port
        if self.start(self.__port) is False:
            self.__server = None

    # Start server
    def start(self, port):
        if self.__server is not None:
            self.__server = None
            self.__server_started = False

        # Create a TCP/IP socket and start server
        try:
            # Socket settings and stuff
            self.__server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.__server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

            # Bind the socket to the port
            self.__server.bind(('', port))

            # Listen for incoming connections
            self.__server.listen(5)

            # update status
            self.__server_started = True

            # If socket was created print a message
            # print "Started TCP listener on all interfaces on port %s..." % self.port
            return True
        except Exception as e:
            self.__set_last_error("socket error: " + str(e))
            self.__server = None
            self.__server_started = False
            return False

    # Stop server
    def stop(self):
        if self.__server is None:
            return False

        try:
            self.__server.close()
            self.__server_started = False
            return True
        except Exception as e:
            self.__set_last_error(str(e))
            return False

    # Check whether server is started or not
    def is_started(self):
        if self.__server is None:
            return False
        return self.__server_started

    # Waiting for incoming clients
    def get_new_client(self):
        # Return error is server socket handler is invalid
        if self.__server is None:
            self.__set_last_error("invalid server socket handler")
            return None

        # Return error if server was not started
        if not self.is_started():
            self.__set_last_error("server not started")
            return None

        # Wait for a connection
        try:
            # Wait fot a client to connect
            client_socket, client_address = self.__server.accept()

            # Print a message if a client connected
            # print "Connection established with %s on port %s..." % client_address

            # If a client is connected, return it
            client = TcpClient(client_socket, client_address[1], client_address[0])

            # but first remove 21 bytes of garbage from windows!
            # TODO: Inspect
            client.recv(21);

            # Return client handler
            return client

        except Exception as e:
            self.__set_last_error(str(e))
            return None

    def get_last_error(self):
        return self.__last_error

    def __set_last_error(self, err_str):
        self.__last_error = err_str


# Function used to print debug messages
def dbg(dbg_str, alert=0):
    if alert == 1:
        sys.stdout.write("\033[1;31m")  # Set text to red
        sys.stdout.write(dbg_str)
        sys.stdout.write("\033[0;0m")   # Reset text
        sys.stdout.flush()
    else:
        sys.stdout.write(dbg_str)
        sys.stdout.flush()


# Test function - this will echo received data from first connected client
# Will handle sudden disconnects and the rest!
def sockets_test(TCP_PORT):
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
        data = client.recv(16)
        if data == -1:
            continue
        client.write(data)
