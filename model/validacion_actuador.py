# -*- coding: utf-8 -*-
"""
Created on Thu Feb 18 16:02:37 2021
@author: growolff

comportamiento del actuador para diferentes cargas en la salida
"""

import numpy as np
import scipy as sp
import scipy.signal

import matplotlib as mpl
from matplotlib import pyplot as plt
import numpy.polynomial.polynomial as poly
mpl.rcParams['text.usetex'] = True

from actuator import TSActuatorGonMovil

acGon = TSActuatorGonMovil(L=59,A=1.5,B=2.5,R=0.35/2)

def main():
    plotActuador()

def plotActuador():
    # masas de prueba
    m = [100,  250, 500, 1000]

    # resistencia en kohms
    aref = [0, 20, 52, 100, 120, 160]

    # posicion real del motor
    am4_1 = [0,19,53,99,121,142]
    # desplazamiento del acutador medido en la imagen
    xm4_1 = [0,0.56,4.3,18.2,34,41.7]

    a_max_mot = 180 # en pasos de motor: 4 por vuelta
    encoder_res = 4
    #af = np.linspace(0,a_max,50) # min, max, n_items
    a_mot = np.arange(0,a_max_mot,encoder_res) # min, max, step # en pasos de motor
    a_turn = a_mot/encoder_res
    a_rad = a_turn*2*np.pi
    x = acGon.x(a_rad)

    fig1,ax1 = plt.subplots()
    ax1.plot(am4_1,xm4_1, 'm+', label='m = 100 [g]')
    ax1.plot(a_rad,x, 'b--', label='modelo')
    #ax1.plot(m,v2, 'go', label='Voltaje para Rf = 100 kOhm')
    #ax1.plot(m,v3, 'bp', label='Voltaje para Rf = 150 kOhm')

    #ax1.plot(x,p1fit, 'm:', label='V ajustado para Rf = 50 kOhm')
    #ax1.plot(x,p2fit, 'g--', label='V ajustado para Rf = 100 kOhm')
    #ax1.plot(x,p3fit, 'b-.', label='V ajustado para Rf = 150 kOhm')
    ax1.set_xlabel("Angulo " + r'$\alpha/2\pi$ [vueltas]')
    ax1.set_ylabel(r'Desplazamiento $x(\alpha)$ [mm]')

    ax1.grid()
    ax1.legend(loc=0,fontsize='large')
    fig1.tight_layout()
    plt.show()

def plotAjusteV():
    v = [0.736, 1.48, 1.84, 3.44]
    r = [45.7, 25.8, 16.5, 8.8]
    m = [200, 300, 500, 1000]

    #pr_coefs = poly.polyfit(m,r,2)
    pv_coefs = poly.polyfit(m,v,1)

    x = np.linspace(200,1000,10) # desde, hasta, cantidad

    #Rfit = poly.polyval(x,pr_coefs)
    Vfit = poly.polyval(x,pv_coefs)

    fig1,ax1 = plt.subplots()

    ax2 = ax1.twinx()
    print(pv_coefs)
    ax1.plot(m,r, 'b--o', label='R sensor')
    ax2.plot(m,v, 'r:*',label='Voltaje medido')
    ax2.plot(x,Vfit,'g-', label='Curva ajustada V')
    ax2.set_ylabel('Voltaje [V]')
    #ax2.legend()

    ax1.set_xlabel('Masas de prueba [g]')
    #ax1.set_xlabel('Fuerza [g]')
    ax1.set_ylabel('Resistencia sensor[kOhm]')
    #ax1.legend()

    ax1.grid()
    fig1.legend(loc=9,fontsize='large')
    fig1.tight_layout()
    plt.show()


if __name__ == '__main__':
    main()
