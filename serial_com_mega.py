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

def receiveData(t):
    print("Receiving data from controller...")
    time.sleep(1)
    # CMD ID REF P I D
    s.sendCMD(SEND_DATA_TRUE,1,255,255,0,0) # send data
    time.sleep(t)
    s.sendCMD(SEND_DATA_FALSE,0,200,100,0,0) # send data
    time.sleep(t)

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

    s = HandSerial(port='COM7')
    s.startProcess()
    try:
        main(sys.argv[0:])
    except Exception as e:
        print(e)
        s.stopProcess()
