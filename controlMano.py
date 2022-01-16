# -*- coding: utf-8 -*-

import time as t
from Dedo import *

def readMotorData():

    t.sleep(1)
    mega.sendCmd(s1=SEND_DATA_TRUE)

    a = range(0,60)
    for i in a:
      mega.sendCmd(s1=TO_SS1,s2=0xEE)
      print(mega.getMsg())
      t.sleep(0.1)

    mega.sendCmd(s1=SEND_DATA_FALSE)
    t.sleep(0.5)
    mega.stopProcess()

def abrirDedo():
    print("Abrir dedo")
    medio.enable()
    medio.open()

    t.sleep(3)
    medio.close()

    t.sleep(3)
    medio.relax()

    t.sleep(3)
    medio.disable()



def relax():
    mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=0,s4=0)
    t.sleep(1)


if __name__ == '__main__':
    mano_ser = ManoSerial('COM4 ',500000)
    medio = Dedo(mano_ser, "MEDIO", TO_SS1, max_mf=130, max_me=110)
    #mano_ser.startProcess()
    t.sleep(1)
    try:
        #readMotorData()
        abrirDedo()

        mano_ser.stopProcess()
    except Exception as e:
        print(e)
        mega.stopProcess()
