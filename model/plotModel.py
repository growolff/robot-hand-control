# -*- coding: utf-8 -*-

import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.rcParams['text.usetex'] = True
import numpy as np

from actuator import TSActuatorGonMovil, TSActuatorGodler, TSActuatorGonFixed

#acGon = TSActuatorGodler(L=50,A=1.5, B=0, R=0.175)
acGon = TSActuatorGonMovil(L=59,A=1.5,B=2.5,R=0.35/2)
#acGon = TSActuatorGonFixed(L=50,A=1.5,B=2,R=0.35/2)

def main():

    a_max_mot = 180 # en pasos de motor: 4 por vuelta
    encoder_res = 4
    #af = np.linspace(0,a_max,50) # min, max, n_items
    a_mot = np.arange(0,a_max_mot,encoder_res) # min, max, step # en pasos de motor
    a_turn = a_mot/encoder_res
    a_rad = a_turn*2*np.pi

    # angulo al limite de torcimiento
    #print(np.arctan(2/np.pi)*180/np.pi) # np.deg2rad(x) # np.rad2deg(x)
    x = acGon.x(a_rad)
    #print(x)
    amax_rad = acGon.a_max()
    amax_turn = amax_rad/(2*np.pi)
    xmax = acGon.x(amax_rad)
    #xmax = acGon.x_max()
    print("a max: " + str(amax_turn))
    print("x max: " + str(xmax))
    #plotModel(a_turn,x,amax_turn,xmax)
    #plotRT(a_turn,a_rad)

    #plotSensB()
    #plotSensL()
    plotAnalisisR(a_turn,a_rad)

def plotRT(a_turn,a_rad):
    rt = acGon.RT(a_rad)

    rt_max = acGon.RT(40*2*np.pi)
    #f = 7.25 # n
    f = 10.8
    t = f/rt_max

    print (rt_max, t)
    fig, ax = plt.subplots()
    # ax.plot(a,(act.L - y),'m.--',linewidth=1,markersize=3,color='magenta')
    # ax.plot(a,(act1.L - y1),'bo--',linewidth=1,markersize=3,color='blue')
    ax.plot(a_turn,rt,'m.--',linewidth=1,markersize=2)
    #ax.plot(a,(y1),'bo--',linewidth=1,markersize=3,color='blue')

    ax.grid()
    #ax.set_title("Razón de transmisión")
    ax.set_xlabel("Rotación del motor " +  r'$\alpha/2\pi$ [vueltas]')
    ax.set_ylabel('Razón de transmisión ' + r'$F/T(\alpha)$')

    #fig.savefig("figure1.pdf")
    plt.show()

def plotModel(x, y, max_a, max_x):

    fig, ax = plt.subplots()
    ax.plot(x,y,'m',linewidth=1,markersize=3,color='magenta',label=r'$x(\alpha)$')
    ax.vlines(max_a,0,max_x+10,linewidth=1,linestyles='dashdot',color='green',label=r'$\alpha_{max}$')
    ax.hlines(max_x,0,max_a+10,linewidth=1,linestyles='dotted',color='blue', label=r'$x_{max}$')

    ax.grid()
    #ax.set_title("Desplazamiento de la cuerda trenzada")
    ax.set_xlabel("Angulo " + r'$\alpha/2\pi$ [vueltas]')
    ax.set_ylabel(r'Desplazamiento $x(\alpha)$ [mm]')

    ax.legend(loc=2)
    #fig.savefig("figure1.pdf")
    plt.show()

def x_max(L,A,B):
    # modelo de godler para B=0
    aux = np.sqrt(np.pi**2+4)
    return np.sqrt(L**2-(A-B)**2) - (2*L/aux)

def plotSensB():
    L = np.arange(20,60,10)
    xmax = []
    A = 1.5
    B = [2, 3, 4, 5]
    for b in range(len(B)):
        xmax.append(x_max(L,A,B[b]))

    fig, ax = plt.subplots()
    ax.plot(L,xmax[0],'m:',linewidth=1,markersize=3,label=r'$B = 2 mm$')
    ax.plot(L,xmax[1],'r--',linewidth=1,markersize=3,label=r'$B = 3 mm$')
    ax.plot(L,xmax[2],'g',linewidth=1,markersize=3,label=r'$B = 4 mm$')
    ax.plot(L,xmax[3],'bp-',linewidth=1,markersize=3,label=r'$B = 5 mm$')
    #ax.vlines(max_a,0,max_x+10,linewidth=1,linestyles='dashdot',color='green',label=r'$\alpha$ máximo')
    #ax.hlines(max_x,0,max_a+10,linewidth=1,linestyles='dotted',color='blue', label=r'$x$ máximo')
    ax.grid()
    ax.set_xlabel("Largo incial de la cuerda destrenzada L [mm]" )
    ax.set_ylabel("Desplazamiento máximo "+ r'$x_{max}$ [mm]')

    ax.legend()
    #fig.savefig("figure1.pdf")
    plt.show()

def plotSensL():
    L = np.arange(40,70,10)
    A = 1.5
    B = 5
    xmax = x_max(L,A,B)
    max_x = 27.5
    fig, ax = plt.subplots()
    ax.plot(L,xmax,'m',linewidth=1,markersize=3,label=r'$x_{max}(L)$')
    ax.hlines(max_x,40,60,linewidth=1,linestyles='dotted',color='blue', label=r'$x_{max} = 27.5$')
    #ax.vlines(max_a,0,max_x+10,linewidth=1,linestyles='dashdot',color='green',label=r'$\alpha$ máximo')
    ax.grid()
    ax.set_xlabel("Largo incial de la cuerda destrenzada L [mm]" )
    ax.set_ylabel("Desplazamiento máximo "+ r'$x_{max}$ [mm]')

    ax.legend()
    #fig.savefig("figure1.pdf")
    plt.show()

def plotAnalisisR(a_turn,a_rad):
    L = 40
    A = 1
    B = A
    R = [0.38/2, 0.35/2]
    tsa = []
    x = []
    a_max = []
    for r in range(len(R)):
        tsa.append(TSActuatorGonMovil(L=L,A=A,B=B,R=R[r]))
        x.append(tsa[r].x(a_rad))
        a_max.append(tsa[r].a_max())
    max_x = tsa[0].x(a_max[0])

    fig, ax = plt.subplots()
    ax.plot(a_turn,x[0],'m',linewidth=2,markersize=3,label=r'$R_1=0.38/2$ mm')
    ax.plot(a_turn,x[1],'r:',linewidth=2,markersize=3,label=r'$R_2=0.35/2$ mm')
    ax.vlines(a_max[0]/(2*np.pi),0,30,linewidth=1,linestyles='--',color='blue',label=r'$\alpha_{max}/2\pi$ para $R_1$')
    ax.vlines(a_max[1]/(2*np.pi),0,30,linewidth=1,linestyles='dashdot',color='black',label=r'$\alpha_{max}/2\pi$ para $R_2$')
    ax.hlines(max_x,0,30,linewidth=2,linestyles='-',color='green', label=r'$x_{max}$')
    ax.grid()
    ax.set_xlabel("Angulo " + r'$\alpha/2\pi$ [vueltas]')
    ax.set_ylabel(r'Desplazamiento $x(\alpha)$ [mm]')

    ax.legend()
    #fig.savefig("figure1.pdf")
    plt.show()

def plotAFAE(a,b):
    fig, ax = plt.subplots()
    # ax.plot(a,(act.L - y),'m.--',linewidth=1,markersize=3,color='magenta')
    # ax.plot(a,(act1.L - y1),'bo--',linewidth=1,markersize=3,color='blue')
    ax.plot(a,b,'m.--',linewidth=1,markersize=3,color='magenta')
    #ax.plot(b,(ac.L-y1),'bo--',linewidth=1,markersize=3,color='blue')
    #ax.plot(a,(ac.L-y),'bo--',linewidth=1,markersize=3,color='blue')

    ax.grid()
    ax.set_title("")

    #ax.set_xlabel('\frac{\alpha}{2*\pi} [vueltas]')
    #ax.set_ylabel("Distancia x(r'$\alpha$') [mm]")
    ax.set_xlabel("Angulo de rotación del motor flector " + "alphaF/2PI [vueltas]")
    ax.set_ylabel("Angulo de rotación del motor extensor " + "alphaE/2PI [vueltas]")

    #fig.savefig("figure1.pdf")
    plt.show()


if __name__ == '__main__':
    main()
