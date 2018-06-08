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


class SerialPort:
    def __init__(self, serial_port, baud_rate):
        # User defined config
        self.__port_name = serial_port
        self.__baud_rate = baud_rate

        # Serial port handler
        self.__hSerial = None

        # Store last error
        self.__last_error = ""

    def read(self, buff_len):
        if not self.is_alive():
            self.__set_last_error("Port is not open!")
            return -1
        if buff_len <= 0:
            self.__set_last_error("Can't read this length")
            return -1
        try:
            return self.__hSerial.read(buff_len)
        except Exception as e:
            self.__set_last_error("Port is not open!")
            self.__hSerial.close()
            return -1

    def write(self, data):
        if not self.is_alive():
            self.__set_last_error("Port is not open!")
            return -1
        if len(data) == 0:
            self.__set_last_error("Can't read this length")
            return -1
        try:
            return self.__hSerial.write(data)
        except Exception as e:
            self.__set_last_error("Port is not open!")
            self.__hSerial.close()
            return -1

    def is_alive(self):
        if self.__hSerial is None:
            return False
        if self.__hSerial.isOpen() is False:
            return False

        # Will try to read some data. If there is actualt data available, then error will be prompted on write/read
        if self.is_data_available() <= 0:
            try:
                self.__hSerial.read(1)
            except Exception as e:
                self.__set_last_error(str(e))
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

    def is_data_available(self):
        try:
            return self.__hSerial.in_waiting
        except Exception as e:
            self.__set_last_error(str(e))
            return -1

    def get_last_error(self):
        return self.__last_error

    def __set_last_error(self, err_str):
        self.__last_error = str(err_str)

    def get_port_name(self):
        return self.__port_name

    def get_baud_rate(self):
        return self.__baud_rate


class SerialService:
    def __init__(self):
        self.__serial_service = None

    def get_instance(self):
        return self.__serial_service

    def serial_recv_callback(self, port, data):
        dbg("[" + port.name + "] " + str(data) + "\n")

    def start_serial_service(self, serial_port, baud_rate, callback_recv):
        # Create a new serial port instance
        self._serial_service = SerialPort(serial_port, baud_rate)
        while True:
            if self._serial_service is not None and self._serial_service.is_alive():
                # if some data is available
                if self._serial_service.is_data_available() > 0:
                    # Read data and pass it via callback function if available
                    recv_data = self._serial_service.read(64)
                    if recv_data == -1:
                        continue
                    else:
                        callback_recv(self._serial_service, recv_data)

                # Reset loop to prevent reconnect
                continue

            # This code is reached if connection to given port failed
            dbg("Attempting to connect to %s with baud %s..." % (str(serial_port), str(baud_rate)))
            if self._serial_service.connect() is False:
                dbg("failed\n", alert=1)
                dbg("ERROR: " + self._serial_service.get_last_error() + "\n", alert=1)
                dbg("I will try again in " + str(RECONNECT_ATTEMPT_S) + " seconds...\n", alert=1)
                time.sleep(RECONNECT_ATTEMPT_S)
                continue
            dbg("done\n")

    def start_serial_service_background(self, serial_port, baud_rate, callback_recv):
        # Launch socket connection in background in a separate thread
        sockets_thread = Thread(target=self.start_serial_service, args=[serial_port, baud_rate, callback_recv])
        sockets_thread.daemon = True
        sockets_thread.start()
        sockets_thread.join()
