# -*- coding: utf-8 -*-

import time as t
from Dedo import *

def testMano(tensionA,tensionC):
    dbc = 0.1
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

    medio.closeControlado(tensionC)
    t.sleep(1)
    indice.closeControlado(tensionC)
    t.sleep(1)
    pulgar.closeControlado(tensionC)

    t.sleep(1)
    medio.apretar(tensionA)
    t.sleep(dbc)
    indice.apretar(tensionA)
    t.sleep(dbc)
    pulgar.apretar(tensionA)
    t.sleep(15)

    pulgar.open()
    t.sleep(dbc)
    indice.open()
    t.sleep(dbc)
    medio.open()
    t.sleep(5)

    pulgar.relax()
    t.sleep(dbc)
    indice.relax()
    t.sleep(dbc)
    medio.relax()
    t.sleep(10)

    pulgar.disable()
    t.sleep(dbc)
    indice.disable()
    t.sleep(dbc)
    medio.disable()

def testPinza(tensionA,tensionC):
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
    #medio.open()
    t.sleep(5)

    t.sleep(1)
    pulgar.closeControlado(int(tensionC))
    t.sleep(dbc)
    indice.closeControlado(int(tensionC))

    t.sleep(1)
    indice.apretar(tensionA)
    t.sleep(dbc)
    pulgar.apretar(tensionA)
    t.sleep(10)

    pulgar.open()
    t.sleep(dbc)
    indice.open()
    #t.sleep(dbc)
    #medio.open()
    t.sleep(5)

    pulgar.relax()
    t.sleep(dbc)
    indice.relax()
    #t.sleep(dbc)
    #medio.relax()
    t.sleep(10)

    pulgar.disable()
    t.sleep(dbc)
    indice.disable()
    t.sleep(dbc)
    medio.disable()

def testSweep(tension):
    indice.enable()
    t.sleep(0.1)
    indice.open()
    t.sleep(5)

    indice.sweep(tension)
    #indice.close()
    t.sleep(5)

    indice.open()
    t.sleep(5)

    indice.relax()
    t.sleep(5)
    indice.disable()


def testDedo(tension_apriete,tension_controlado):

    indice.enable()
    t.sleep(0.1)
    indice.open()
    t.sleep(5)


    indice.closeControlado(tension_controlado)
    indice.apretar(tension_apriete)
    #indice.close()
    t.sleep(10)

    indice.open()
    t.sleep(5)

    indice.relax()
    t.sleep(5)
    indice.disable()

def refTension(tension):
    # convierte la referencia de tension en gramos a un valor en 8bits [0-255]
    K = 0.011159057404310219
    return int(tension / K / 1000)

if __name__ == '__main__':

    mano_ser = ManoSerial('/dev/ttyACM0',500000)
    # SS1 40
    # SS2 53
    # SS3 38
    indice = Dedo(mano_ser, ss=TO_SS3, name="INDICE", max_mf=90, max_me=100)
    pulgar = Dedo(mano_ser, ss=TO_SS2, name="PULGAR", max_mf=80, max_me=85)
    medio = Dedo(mano_ser, ss=TO_SS1, name="MEDIO", max_mf=90, max_me=10)
    mano_ser.startProcess()

    t.sleep(1)
    # maxima tension 2846
    tendon_tens = 1000
    tapriete = refTension(tendon_tens)
    tcontrol = refTension(1000)
    print("tension tendon: " + str(tendon_tens) + "g -> " + str(tapriete) + " valor de bits")

    try:
        #testPinza(tapriete,tcontrol)
        testDedo(tapriete,tcontrol)
        #testMano(tapriete,tcontrol)
        #testSweep(tref)
        mano_ser.stopProcess()
    except Exception as e:
        print(e)
        mano_ser.stopProcess()
