#
#   Filename    : Sockets.py
#   Created on  : Jun, 2018
#   Author      : Alex C.
#   Description : Python sockets wrapper
#

import socket
import select
import sys
from exceptions import Exception


# Local TCP client connection
class TcpClient:
    def __init__(self, fd, port, address):

        # Client fd descriptor
        self.__hSocket = fd
        self.__hSocket.setblocking(1)
        self.set_keepalive_linux(self.__hSocket)

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
    def set_keepalive_linux(self, sock, after_idle_sec=1, interval_sec=3, max_fails=5):
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
            return TcpClient(client_socket, client_address[1], client_address[0])

        except Exception as e:
            self.__set_last_error(str(e))
            return None

    def get_last_error(self):
        return self.__last_error

    def __set_last_error(self, err_str):
        self.__last_error = err_str


# Sockets @ test function
def sockets_test():
    # Testing class
    server = TcpServer(1337)
    client = server.get_new_client()

    while True:
        data = client.recv(1)
        client.write(data)
