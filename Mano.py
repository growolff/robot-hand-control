# -*- coding: utf-8 -*-

from model.actuator import TSActuatorGonMovil
from Dedo import *
import numpy as np
import time as t

MAX_MF_PULGAR = 90
MAX_ME_PULGAR = 90

MAX_MF_MEDIO = 110
MAX_ME_MEDIO = 110

MAX_MF_INDICE = 110
MAX_ME_INDICE = 110

class Mano(ManoSerial):
    def __init__(self, port='/dev/ttyACM0',baudrate=500000):
        super(Mano, self).__init__(port,baudrate)
        #self.actuator = TSActuatorGon(L=59,A=1.5,B=2.5,R=0.175)
        self.indice = Dedo(self, name="INDICE", ss=TO_SS3, max_mf=MAX_MF_INDICE, max_me=MAX_ME_INDICE)
        self.pulgar = Dedo(self, name="PULGAR", ss=TO_SS2, max_mf=MAX_MF_PULGAR, max_me=MAX_ME_PULGAR)
        self.medio = Dedo(self, name="MEDIO", ss=TO_SS1, max_mf=MAX_MF_MEDIO, max_me=MAX_ME_MEDIO)

        self.tbc = 0.5

    def enable(self):
        self.indice.enable()
        t.sleep(self.tbc)
        self.pulgar.enable()
        t.sleep(self.tbc)
        self.medio.enable()
        t.sleep(self.tbc)

    def disable(self):
        self.indice.disable()
        t.sleep(self.tbc)
        self.pulgar.disable()
        t.sleep(self.tbc)
        self.medio.disable()
        t.sleep(self.tbc)

    def relax(self,finger=None):
        if finger == "indice":
            self.indice.relax()
        if finger == "pulgar":
            self.pulgar.relax()
        if finger == "medio":
            self.medio.relax()
        elif finger == None:
            self.indice.relax()
            t.sleep(self.tbc)
            self.pulgar.relax()
            t.sleep(self.tbc)
            self.medio.relax()
            t.sleep(self.tbc)

    def open(self,finger=None):
        if finger == "indice":
            self.indice.open()
        if finger == "pulgaobjectr":
            self.pulgar.open()
        if finger == "medio":
            self.medio.open()
        elif finger == None:
            self.indice.open()
            t.sleep(self.tbc)
            self.pulgar.open()
            t.sleep(self.tbc)
            self.medio.open()
            t.sleep(self.tbc)

    def close(self,finger=None):
        if finger == "indice":
            self.indice.close()
        if finger == "pulgar":
            self.pulgar.close()
        if finger == "medio":
            self.medio.close()
        elif finger == None:
            self.indice.close()
            t.sleep(self.tbc)
            self.pulgar.close()
            t.sleep(self.tbc)
            self.medio.close()
            t.sleep(self.tbc)


def main():
    mano = Mano()
    mano.enable()

    mano.open("indice")
    t.sleep(5)

    mano.close()
    t.sleep(5)

    mano.open()
    t.sleep(2)

    mano.close()
    t.sleep(2)

    mano.relax()

    mano.disable()

if __name__ == '__main__':
    main()
