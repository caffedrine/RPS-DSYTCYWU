#
#   Filename    : main.py
#   Created on  : Jun, 2018
#   Author      : Alex C.
#   Description : Serial port wrapper
#

from threading import Thread
from Util import *

import time
from serial import *


# After how many seconds should attempt to reconnect
RECONNECT_ATTEMPT_S = 10


class SerialService:
    def __init__(self, serial_port, baud_rate):
        # User defined config
        self.__port_name = serial_port
        self.__baud_rate = baud_rate

        # Serial port handler
        self.__hSerial = None

        # Store last error
        self.__last_error = ""

    def read(self, buff_len):
        return 1

    def write(self, data):
        if not self.is_alive():
            self.__set_last_error("Port is not open!")
            return -1

    def is_alive(self):
        if self.__hSerial is None:
            return False
        if self.__hSerial.isOpen is False:
            return False
        return True

    def connect(self):
        try:
            self.__hSerial = Serial(self.__port_name, self.__baud_rate)
            return True
        except Exception as e:
            self.__set_last_error(str(e))
            return False

    def close(self):
        try:
            self.__hSerial.close()
            return True
        except Exception as e:
            self.__set_last_error(str(e))
            return False

    def get_last_error(self):
        return self.__last_error

    def __set_last_error(self, err_str):
        self.__last_error = str(err_str)

    def get_port_name(self):
        return self.__port_name

    def get_baud_rate(self):
        return self.__baud_rate


def serial_recv_callback(port, data):
    dbg("[" + port.name + "] " + str(data) + "\n")


def start_serial_service(serial_port, baud_rate, callback_recv):
    # Create a new serial port instance
    serial_service = SerialService(serial_port, baud_rate)

    while True:
        if serial_service is not None and serial_service.is_alive():
            # Read data and pass it via callback function if available
            recv_data = serial_service.read(64)
            if recv_data == -1:
                continue
            else:
                callback_recv(serial_service, recv_data)

            # Reset loop to prevent reconnect
            continue

        # This code is reached if connection to given port failed
        dbg("Attempting to connect to %s with baud %s..." % (str(serial_port), str(baud_rate)) )
        if serial_service.connect() is False:
            dbg("failed\n", alert=1)
            dbg("ERROR: " + serial_service.get_last_error() + "\n", alert=1)
            dbg("Server will restart in " + str(RECONNECT_ATTEMPT_S) + " seconds...\n", alert=1)
            time.sleep(RECONNECT_ATTEMPT_S)
            continue
        dbg("done\n")


def start_serial_service_background(serial_port, baud_rate, callback_recv):
    # Launch socket connection in background in a separate thread
    sockets_thread = Thread(target=start_serial_service, args=[serial_port, baud_rate, callback_recv] )
    sockets_thread.daemon = True
    sockets_thread.start()
    sockets_thread.join()