# -*- coding: utf-8 -*

# Operating system
import os
import sys, getopt

import serial
import time

from threading import Lock, Thread
from collections import namedtuple

#from PyQt5.Qt import *
#from pyqtgraph.Qt import QtCore, QtGui
from array import array

from HandSerial import HandSerial
from variables import *

from struct import *

import numpy as np

def receiveData(t):
    print("Receiving data from controller...")
    time.sleep(1)
    # CMD ID REF P I D
    s.sendCMD(SEND_DATA_TRUE,1,255,255,0,0) # send data
    time.sleep(t)
    #s.sendCMD(SEND_DATA_FALSE,0,200,100,0,0) # send data
    #time.sleep(t)

    a = range(0,260,5)
    for ref in a:
        s.sendCMD(SET_POS_REF,0,ref,100,0,0) # send data
        time.sleep(0.01)
    for ref in a:
        s.sendCMD(SET_POS_REF,0,255-ref,100,0,0) # send data
        time.sleep(0.01)

def main(argv):
    tsleep = int(argv[1])
    try:
        receiveData(tsleep)
        #moveTwoMotors()
        #pote_test(0)
        s.stopProcess()
    except Exception as e:
        print(e)
        s.stopProcess()


if __name__ == "__main__":

    s = HandSerial(port='COM5',baudrate=115200)
    s.startProcess()
    try:
        main(sys.argv[0:])
    except Exception as e:
        print(e)
        s.stopProcess()
