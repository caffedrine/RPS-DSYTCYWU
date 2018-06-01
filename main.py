import socket
import sys


#  TCP Server Class
class TcpServer:
    def __init__(self, port):
        # Store given port
        self.port = port

        # Other global variables
        self.client_connection = None
        self.client_address = None

        # Create a TCP/IP socket
        try:
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Bind the socket to the port
            self.server_address = ('127.0.0.1', self.port)
            print >> sys.stderr, 'Started TCP listener on %s port %s...' % self.server_address
            self.server.bind(self.server_address)

            # Listen for incoming connections
            self.server.listen(1)

        except Exception as e:
            print >> sys.stderr, 'Socket error: ' + str(e)
            self.server = None

    # Waiting for incoming clients
    def waitForConnection(self):
        if self.server is None:
            print >> sys.stderr, 'Invalid socket handler...'
            return 1

        # Wait for a connection
        print >> sys.stderr, 'Waiting for clients...'
        try:
            self.client_connection, client_address = self.server.accept()
            print >> sys.stderr, 'Connection established with %s port %s...' % self.client_address
            return 0
        except Exception as e:
            print >> sys.stderr, 'Failed to establish a connection with a client: ' + str(e)
            self.client_connection = None
            self.client_address = None
            return 1

    # Write data to current connected client
    def write(self, data):
        if self.client_connection is None:
            print >> sys.stderr, 'Invalid client socket handler...'
            return 1

        if data:
            try:
                self.client_connection.sendall(data)
                print >> sys.stderr, 'SEND: ' + data
                return 0
            except:
                print >> sys.stderr, 'Socket WRITE error: ' + sys.exc_info()[0]
                return 1

    # Read data from client
    def read(self, dataCount):
        if self.client_connection is None:
            print >> sys.stderr, 'Invalid client socket handler...'
            return 1

        try:
            data = self.client_connection.recv(dataCount)
            print >> sys.stderr, 'RECV: ' + data
            return data
        except:
            print >> sys.stderr, 'Socket RECV error: ' + sys.exc_info()[0]
            return 1

    # Close connection
    def close(self):
        # Clean up the connection
        self.client_connection.close()

# Testing class
client = TcpServer(1337)
if client.waitForConnection() == 0:
    while True:
        data = client.read(1)
        client.write(data)

