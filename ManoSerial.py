# -*- coding: utf-8 -*-

import struct
from struct import *
import serial
from array import array
from threading import Lock, Thread
from collections import namedtuple

from io import BytesIO

import time as t

TO_SS1 = 11
TO_SS2 = 12
SEND_DATA_TRUE = 40
SEND_DATA_FALSE = 41
SET_POS_REF = 1

class ManoSerial(object):

    # estructura para enviar datos
    sendStruct = struct.Struct('<BBBB')
    '''
        uint8 s1 -> slave select: SS1, SS2, SS3
        uint8 s2 -> cmd: posRef, tensionRef
        uint8 s3 -> ref motor 1
        uint8 s4 -> ref motor 2
    '''

    def __init__(self, port='COM5',baudrate=500000):

        self.device = port
        self.baudrate = baudrate
        self.running = False

        self.ser = serial.Serial(self.device, self.baudrate, timeout=0.1)
        self.serial_mutex = Lock()
        self.thread = Thread(target=self.serialHandlerThread)

        # estructura para recibir datos
        self.recvFmt = '<BBH'

        self.s1 = 0
        self.s2 = 0
        self.s3 = 0
        self.s4 = 0

        self.tuple = namedtuple('tuple','d1 d2 d3 d4')

        self.recvMsg = []
        self.r1 = 0
        self.r2 = 0
        self.r3 = 0

    def __exit__(self, exc_type, exc_value, traceback):
        self.stopProcess()

    def close(self):
        """
        Close the serial port.
        """
        if self.ser:
            self.ser.flushInput()
            self.ser.flushOutput()
            self.ser.close()

    def stopProcess(self):
        self.ser.flushInput()
        self.ser.flushOutput()
        self.running = False

    def startProcess(self):
        self.running = True
        self.thread.start()

    def to_hex(self,data):
        return ":".join("{:02x}".format(c) for c in data)

    def serialize(self,buff):
        return buff.write(ManoSerial.sendStruct.pack(self.s1,self.s2,self.s3,self.s4))

    def sendCmd(self,s1=0,s2=0,s3=0,s4=0):
        self.s1 = s1
        self.s2 = s2
        self.s3 = s3
        self.s4 = s4
        #print(self.s1,self.s2,self.s3,self.s4)
        buff = BytesIO()
        self.serialize(buff)
        #print(self.to_hex(buff.getvalue()))
        packet = bytearray(buff.getvalue())
        #print(packet)
        packet_str = bytes(packet)
        with self.serial_mutex:
            self.write_serial(packet_str)

    def write_serial(self, data):
        """
        Write in the serial port.
        """
        #print("Write Data: " + str(data))
        self.ser.flushInput()
        self.ser.flushOutput()
        self.ser.write(data)


    def serialHandlerThread(self):

        recvStructLen = calcsize(self.recvFmt)
        while self.running is True:
            try:
                if self.ser.inWaiting():
                    msg = self.ser.read(recvStructLen)  # show the message as it is
                    self.recvMsg = unpack(self.recvFmt, msg)
                    self.r1 = self.recvMsg[0]
                    self.r2 = self.recvMsg[1]
                    self.r3 = self.recvMsg[2]
                    print(self.recvMsg)
            except Exception as e:
                print("reading error: ",e)
        self.ser.close()

def to_hex(data):
    return ":".join("{:02x}".format(c) for c in data)

def test():
    t.sleep(1)
    mega.sendCmd(s1=SEND_DATA_TRUE)
    t.sleep(0.1)
    mega.sendCmd(s1=TO_SS1,s2=255,s3=255)
    mega.sendCmd(s1=TO_SS1,s2=255,s3=255)
    t.sleep(1)
    mega.sendCmd(s1=TO_SS1,s2=50,s3=50)
    mega.sendCmd(s1=TO_SS1,s2=50,s3=50)
    t.sleep(1)
    mega.sendCmd(s1=TO_SS1,s2=0,s3=0)
    mega.sendCmd(s1=TO_SS1,s2=0,s3=0)
    t.sleep(0.5)
    mega.sendCmd(s1=SEND_DATA_FALSE)
    t.sleep(0.5)

    mega.stopProcess()

if __name__ == '__main__':
    mega = ManoSerial('COM5',500000)
    mega.startProcess()
    try:
        test()
    except Exception as e:
        print(e)
        mega.stopProcess()
