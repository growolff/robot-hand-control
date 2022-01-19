# -*- coding: utf-8 -*-

import time as t
from Dedo import *

def testMano(FUERZA):
    dbc = 0.01
    pulgar.enable()
    t.sleep(dbc)
    indice.enable()
    t.sleep(dbc)
    medio.enable()
    t.sleep(dbc)
    indice.open()
    t.sleep(dbc)
    pulgar.open()
    t.sleep(dbc)
    medio.open()
    t.sleep(5)

    medio.closeControlado(FUERZA)
    t.sleep(dbc)
    indice.closeControlado(FUERZA)
    t.sleep(dbc)
    pulgar.closeControlado(FUERZA)
    t.sleep(10)

    pulgar.open()
    indice.open()
    medio.open()
    t.sleep(5)

    pulgar.relax()
    t.sleep(dbc)
    indice.relax()
    t.sleep(dbc)
    medio.relax()
    t.sleep(5)

    pulgar.disable()
    t.sleep(dbc)
    indice.disable()
    t.sleep(dbc)
    medio.disable()


def testDedo():

    medio.enable()
    t.sleep(0.1)
    medio.open()
    t.sleep(5)

    medio.closeControlado(200)
    t.sleep(10)

    medio.open()
    t.sleep(5)

    medio.relax()
    t.sleep(5)
    medio.disable()


if __name__ == '__main__':
    mano_ser = ManoSerial('COM4',500000)
    # SS1 40
    # SS2 53
    # SS3 38
    indice = Dedo(mano_ser, ss=TO_SS3, name="INDICE", max_mf=90, max_me=100)
    pulgar = Dedo(mano_ser, ss=TO_SS2, name="PULGAR", max_mf=80, max_me=80)
    medio = Dedo(mano_ser, ss=TO_SS1, name="MEDIO", max_mf=90, max_me=100)
    t.sleep(1)
    mano_ser.startProcess()
    try:
        #testDedo()
        testMano(150)
        mano_ser.stopProcess()
    except Exception as e:
        print(e)
        mano_ser.stopProcess()
