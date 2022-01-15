# -*- coding: utf-8 -*-

from model.actuator import TSActuatorGonMovil
from ManoSerial import *
import numpy as np
import time as t

class Dedo(object):

    def __init__(self, mano, name, ss, max_me, max_mf):
        self.ss = ss
        self.mano = mano
        self.ac = TSActuatorGonMovil()

        # flector and extensor actuator limits in radians
        self.max_mf_angle = max_mf
        self.max_me_angle = max_me

        # delay between commands
        self.dbc = 0.05

        # steps for position control ref
        self.step = 10

        self.name = name

    def enableME(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_E)

    def enableMF(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_F)

    def enable(self):
        print("Enable " + str(self.name))
        self.enableME()
        self.enableMF()

    def disableME(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_E)

    def disableMF(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_F)

    def disable(self):
        print("Disable " + str(self.name))
        self.disableME()
        self.disableMF()

    def relax(self):
        self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s3=0,s4=0)

    def open(self):
        print("Open " + str(self.name))
    #    for i in range(0,self.max_me_angle,self.step):
        self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s3=0,s4=self.max_me_angle)
    #        t.sleep(self.dbc)

    def debug_led(self):
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=255,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=150,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=50,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=0,s4=0)

    def close(self):
        print("Close " + str(self.name))
        #for i in range(0,self.max_me_angle,self.step):
        #    aRad = self.ac.toRad(i)
        #    ## revisar este metodo :)
        #    alphaE = self.ac.getAlphaAntagonista(self.max_me_angle,aRad)
        #    print(i,aRad)
        self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s3=self.max_mf_angle,s4=0)
        #    t.sleep(self.dbc)


def main():
    mano_ser = ManoSerial('COM4',500000)
    indice = Dedo(mano_ser, "INDICE", TO_SS3, max_mf=110, max_me=110)
    pulgar = Dedo(mano_ser, "PULGAR", TO_SS2, max_mf=90, max_me=90)
    medio = Dedo(mano_ser, "MEDIO", TO_SS1, max_mf=110, max_me=110)
    t.sleep(1)

    #dedo1.debug_led()
    #dedo2.debug_led()
    #dedo3.debug_led()
    #t.sleep(1)


    pulgar.enable()
    t.sleep(0.1)
    pulgar.open()
    t.sleep(0.1)
    indice.enable()
    t.sleep(0.1)
    indice.open()
    t.sleep(0.1)
    medio.enable()
    t.sleep(0.1)
    medio.open()


    t.sleep(5)
    pulgar.close()
    t.sleep(0.05)
    indice.close()
    t.sleep(0.05)
    medio.close()
    t.sleep(0.05)

    t.sleep(5)
    pulgar.open()
    t.sleep(0.05)
    indice.open()
    t.sleep(0.05)
    medio.open()

    t.sleep(2)
    pulgar.relax()
    t.sleep(0.05)
    indice.relax()
    t.sleep(0.05)
    medio.relax()

    t.sleep(1)
    pulgar.disable()
    t.sleep(0.05)
    indice.disable()
    t.sleep(0.05)
    medio.disable()

if __name__ == '__main__':
    main()
