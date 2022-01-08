# -*- coding: utf-8 -*-

import numpy as np

class TSActuatorGodler(object):
    '''
        Modelo de dos cuerdas trenzadas con pivote destorcedor movil
        L: distancia inicial (destrenzada) de la cuerda entre eje rotatorio y destorcedor en mm
        A: radio del destorcedor A en mm
        B: radio del eje rotatorio B en mm, para Godler B=0, pero se puede agregar el valor real para mejorar el modelo
        R: radio cuerda trenzada, cuerda usada es 0.35 mm diametro
    '''
    def __init__(self, L=44,R=0.175,A=1.5,B=0):
        self.A = A # un-twister pivot rad
        self.B = B # rotatory pivot rad
        self.L = L # initial (untwisted) distance between string pivots
        self.R = R # string rad
        self.L0 = np.sqrt(self.L*self.L + (self.A-self.B)*(self.A-self.B))

    def x(self,alpha):
        # modelo de godler para B=0
        return self.L0 -np.sqrt((self.L*self.L)-pow((self.A+self.B+self.R*alpha),2))

    def a_max(self):
        # modelo de godler para B=0
        aux = np.sqrt(np.pi*np.pi+4)
        return (np.pi*self.L - self.A*aux)/(self.R*aux)

    def x_max(self):
        # modelo de godler para B=0
        aux = np.sqrt(np.pi*np.pi+4)
        return np.sqrt(self.L*self.L-self.A*self.A) - (2*self.L/aux)

    def RT(self,alpha):
        # modelo de godler para B=0
        num = np.sqrt(pow(self.L,2)-pow(self.A+self.R*alpha,2))
        den = self.R*(self.A+self.R*alpha)
        return num/den

class TSActuatorGonFixed():
    '''
        Modelo de dos cuerdas trenzadas con pivote destorcedor fijo
        L: distancia fija entre eje rotatorio y destorcedor en mm
        A: radio del destorcedor A en mm
        B: radio del eje rotatorio B en mm
        R: radio cuerda trenzada, cuerda usada es 0.35 mm diametro
    '''
    def __init__(self, L=40,R=0.175,A=0,B=0):
        self.A = A # pivot rad
        self.B = B # pivot rad
        self.L = L # fixed distance between string pivots
        self.R = R # string radii
        self.L0 = np.sqrt(self.L*self.L + (self.A-self.B)*(self.A-self.B))

    def a_max(self):
        beta = np.arctan(2/np.pi)
        return (self.L*np.tan(beta) - self.B - self.A)/self.R

    def xRu(self,alpha):
        return np.sqrt(self.L**2 + (self.A + self.B + alpha*self.R)**2) - self.L0

    def x(self,alpha):
        # modelo mio para B =/= 0 y destorcedor fijo
        aux = self.A + self.B + self.R*alpha
        return np.sqrt(self.L*self.L + aux*aux) - self.L0

    def xJGM(self,alpha):
        LB = np.sqrt(self.B**2 + (0.5*(self.L-(2/np.pi)*self.R*alpha))**2)
        LA = np.sqrt(self.A**2 + (0.5*(self.L-(2/np.pi)*self.R*alpha))**2)
        return LA + LB + (2/np.pi)*self.R*alpha - self.L0


class TSActuatorGonMovil():
    '''
        Modelo de dos cuerdas trenzadas con pivote destorcedor movil y B=/=0
        L: distancia fija entre eje rotatorio y destorcedor en mm
        A: radio del destorcedor A en mm
        B: radio del eje rotatorio B en mm
        R: radio cuerda trenzada, cuerda usada es 0.35 mm diametro
    '''
    def __init__(self, L=40,R=0.175,A=1.5,B=2):
        self.A = A # pivot rad
        self.B = B # pivot rad
        self.L = L # fixed distance between string pivots
        self.R = R # string radii
        self.L0 = np.sqrt(self.L**2 + (self.A-self.B)**2)

    def a_max(self):
        # modelo mio para B=/=0
        aux = np.sqrt(np.pi**2+4)
        return (np.pi*self.L - (self.A+self.B)*aux)/(self.R*aux)

    def x(self,alpha):
        # modelo mio para B =/= 0 y destorcedor movil
        return self.L0 - np.sqrt(self.L**2-(self.A+self.B+self.R*alpha)**2)

    def x_max(self):
        aux = np.sqrt(np.pi**2+4)
        return self.L0 - (2*self.L/aux)

    def RT(self,alpha):
        num = np.sqrt(self.L**2-(self.A+self.B+self.R*alpha)**2)
        den = self.R*(self.A+self.B+self.R*alpha)
        return num/den
