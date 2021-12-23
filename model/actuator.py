# -*- coding: utf-8 -*-

import numpy as np

class TSActuatorGodler():
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
        return np.sqrt(self.L*self.L-self.A*self.A)-np.sqrt((self.L*self.L)-pow((self.A+self.R*alpha),2))

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

class TSActuatorGon():
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
        self.L0 = np.sqrt(self.L**2 + (self.A-self.B)**2)

    def a_max(self):
        beta = np.arctan(2/np.pi)
        return (self.L*np.tan(beta) - self.B - self.A)/self.R

    def x(self,alpha):
        # modelo mio para B =/= 0 y destorcedor fijo
        aux = self.A + self.B + self.R*alpha
        return np.sqrt(self.L**2 + aux**2) - self.L0

    def xx(self,alpha):
        LB = np.sqrt(self.B**2 + (0.5*(self.L-(2/np.pi)*self.R*alpha))**2)
        LA = np.sqrt(self.A**2 + (0.5*(self.L-(2/np.pi)*self.R*alpha))**2)
        return LA + LB + (2/np.pi)*self.R*alpha - self.L0

    def antagonistAlhpa(self,x):
        # modelo mio para B =/= 0 y destorcedor fijo
        aux = np.sqrt(pow(x+self.L0,2) - self.L*self.L) + self.B - self.A
        if np.isnan(aux):
            return 0
        return aux/self.R
