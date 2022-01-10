# -*- coding: utf-8 -*-
"""
Created on Thu Feb 18 16:02:37 2021
@author: growolff

calibracion del sensor de fuerza a101 de flexiforce
"""

import numpy as np
import scipy as sp
import scipy.signal

import matplotlib as mpl
from matplotlib import pyplot as plt
import numpy.polynomial.polynomial as poly
mpl.rcParams['text.usetex'] = True

# rf1 = 50k
# rf2 = 100k
# rf3 = 150k

def main():
    plotCalibracion()

def plotCalibracion():
    m = [0, 100,  243, 462, 1000]
    # resistencia en kohms
    r1 = [1000, 335, 145, 47, 41]
    r2 = [1000, 230, 63, 25.6, 13.5]
    r3 = [1000, 165, 63.5, 37.4, 15.6]
    # voltaje pasado por un 12bit ADC
    # v1 = [20 150 300 890 1750];
    # v2 = [20 430 1100 1950 3270];
    # v3 = [20 500 1890 2900 3995];
    v1 = [0, 0.02, 0.35, 0.97, 1.35]
    v2 = [0, 0.1, 0.95, 2, 3.25]
    v3 = [0, 0.3, 1.25, 2.4, 4.6]

    P1 = poly.polyfit(m,v1,1)
    P2 = poly.polyfit(m,v2,1)
    P3 = poly.polyfit(m,v3,1)

    x = np.linspace(0,1000,10) # desde, hasta, cantidad
    p1fit = poly.polyval(x,P1)
    p2fit = poly.polyval(x,P2)
    p3fit = poly.polyval(x,P3)

    fig1,ax1 = plt.subplots()
    ax1.plot(m,v1, 'm+', label='Voltaje para Rf = 50 kOhm')
    ax1.plot(m,v2, 'go', label='Voltaje para Rf = 100 kOhm')
    ax1.plot(m,v3, 'bp', label='Voltaje para Rf = 150 kOhm')

    ax1.plot(x,p1fit, 'm:', label='V ajustado para Rf = 50 kOhm')
    ax1.plot(x,p2fit, 'g--', label='V ajustado para Rf = 100 kOhm')
    ax1.plot(x,p3fit, 'b-.', label='V ajustado para Rf = 150 kOhm')
    ax1.set_xlabel('Masa de prueba [gr]')
    ax1.set_ylabel('Voltaje [V]')

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

    Rfit = poly.polyval(x,pr_coefs)
    Vfit = poly.polyval(x,pv_coefs)

    fig1,ax1 = plt.subplots()

    ax2 = ax1.twinx()
    print(pv_coefs)
    ax1.plot(m,r, 'b--o', label='R sensor')
    ax2.plot(m,v, 'r:*',label='Voltaje medido')
    ax2.plot(x,Vfit,'g-', label='Curva ajustada V')
    ax2.set_ylabel('Voltaje [V]')
    #ax2.legend()

    ax1.set_xlabel('Fuerza [gr]')
    ax1.set_ylabel('Resistencia sensor[kOhm]')
    #ax1.legend()

    ax1.grid()
    fig1.legend(loc=9,fontsize='large')
    fig1.tight_layout()
    plt.show()


if __name__ == '__main__':
    main()
