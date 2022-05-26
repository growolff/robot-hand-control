# -*- coding: utf-8 -*-

from model.actuator import TSActuatorGonMovil
from ManoSerial import *
import numpy as np
import time as t

class Dedo(object):

    def __init__(self, serial, ss, name, max_me, max_mf):
        self.mano = serial
        self.ss = ss
        self.name = name

        self.ac = TSActuatorGonMovil()

        # flector and extensor actuator limits in radians
        self.max_mf_angle = max_mf
        self.max_me_angle = max_me

        # delay between commands
        self.dbc = 0.1

        # steps for position control ref
        self.stepClose = 4
        self.stepOpen = 4

    def setTensionControl(self):
        print("Set tension control " + str(self.name))
        self.mano.sendCmd(s1=self.ss,s2=SET_CONTROL,s5=TENSION_CONTROL)

    def setPositionControl(self):
        print("Set position control " + str(self.name))
        self.mano.sendCmd(s1=self.ss,s2=SET_CONTROL,s5=POSITION_CONTROL)

    def enableME(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_E)

    def enableMF(self):
        self.mano.sendCmd(s1=self.ss,s2=ENABLE_MOTOR_F)

    def enable(self):
        print("Enable " + str(self.name))
        self.enableME()
        t.sleep(self.dbc)
        self.enableMF()

    def disableME(self):
        self.mano.sendCmd(s1=self.ss,s2=DISABLE_MOTOR_E)

    def disableMF(self):
        self.mano.sendCmd(s1=self.ss,s2=DISABLE_MOTOR_F)

    def disable(self):
        print("Disable " + str(self.name))
        self.disableME()
        t.sleep(self.dbc)
        self.disableMF()

    def relax(self):
        self.setPositionControl()
        print("Relax " + str(self.name))
        t.sleep(self.dbc)
        self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s3=0,s4=0)

    def open(self):
        self.setPositionControl()
        print("Open " + str(self.name))
        th = np.linspace(0,self.max_me_angle,self.stepOpen)
        for i in range(len(th)):
            self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s4=int(th[i]))
            t.sleep(self.dbc)


    def close(self):
        print("Close " + str(self.name))
        thF = np.linspace(0,self.max_me_angle,self.stepClose)
        for i in range(len(thF)):
            thE = int(self.alphaAntagonista(thF[i]))
            self.mano.sendCmd(s1=self.ss,s2=SET_POS_REF,s3=int(thF[i]),s4=thE)
            t.sleep(self.dbc)

    def closeControlado(self,tens_ref):
        self.setTensionControl()
        print("Close control " + str(self.name))
        me_close = np.linspace(self.max_me_angle,0,4)
        for i in range(len(me_close)):
            self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=int(me_close[i]))
            t.sleep(0.3)
        #self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=0) # Extensor 5 vueltas

    def apretar(self,tens_ref):
        self.setTensionControl()
        print("Apretar " + str(self.name))
        self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=0) # Extensor 5 vueltas

    def mantener(self,tens_ref,pos_ref):
        self.setTensionControl()
        print("mantener" + str(self.name))
        self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=pos_ref) # Extensor 5 vueltas

    def sweep(self,tens_ref):
        self.setTensionControl()
        print("sweep" + str(self.name))
        me_close = np.linspace(self.max_me_angle,0,10)
        me_open = np.linspace(0,self.max_me_angle,10)
        for i in range(len(me_close)):
            print(me_close[i])
            self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=int(me_close[i]))
            t.sleep(0.1)

        t.sleep(1)

        for i in range(len(me_open)):
            print(me_open[i])
            self.mano.sendCmd(s1=self.ss,s2=SET_TENS_REF,s3=tens_ref,s4=int(me_open[i]))
            t.sleep(0.2)


    def alphaAntagonista(self,theta):
        a = 65
        b = -0.09058717985726175
        c = 4.245916345362804e-05
        d = -7.200294474934631e-06
        return  a + theta*b + theta**2 * c + theta**3 * d

    def debug_led(self):
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=255,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=150,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=50,s4=0)
        t.sleep(0.5)
        self.mano.sendCmd(s1=self.ss,s2=LED_DEBUG,s3=0,s4=0)


def main():
    mano_ser = ManoSerial('COM4',500000)
    indice = Dedo(mano_ser, "INDICE", TO_SS3, max_mf=80, max_me=110)
    pulgar = Dedo(mano_ser, "PULGAR", TO_SS2, max_mf=70, max_me=90)
    medio = Dedo(mano_ser, "MEDIO", TO_SS1, max_mf=80, max_me=110)
    t.sleep(1)

    #dedo1.debug_led()
    #dedo2.debug_led()
    #dedo3.debug_led()
    #t.sleep(1)


    pulgar.enable()
    t.sleep(0.08)
    pulgar.open()
    t.sleep(0.08)
    indice.enable()
    t.sleep(0.08)
    indice.open()
    t.sleep(0.08)
    medio.enable()
    t.sleep(0.08)
    medio.open()


    t.sleep(5)
    medio.close()
    t.sleep(0.05)
    indice.close()
    t.sleep(0.05)
    pulgar.close()
    t.sleep(0.05)

    t.sleep(10)

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

    t.sleep(5)
    pulgar.disable()
    t.sleep(0.05)
    indice.disable()
    t.sleep(0.05)
    medio.disable()

if __name__ == '__main__':
    main()
