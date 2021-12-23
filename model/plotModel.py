# -*- coding: utf-8 -*-

import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.rcParams['text.usetex'] = True
import numpy as np

from actuator import TSActuatorGon, TSActuatorGodler

acGod = TSActuatorGodler(L=44,A=1.5,R=0.175)
acGon = TSActuatorGon(L=47,A=1.5,B=2,R=0.35/2)

def main():

    #for i in range(len(af)) :
        #x = ac.x(af[i])
        #ae.append(int(idx_me.antagonistAlhpa(x)))
    #print(a_mot)
    #print(a_turn)
    #print(a_rad)
    plotTSAGon()

def plotTSAGon():

    a_max_mot = 550 # en pasos de motor: 16 por vuelta

    #af = np.linspace(0,a_max,50) # min, max, n_items
    a_mot = np.arange(0,a_max_mot,16) # min, max, step # en pasos de motor
    a_turn = a_mot/16
    a_rad = a_turn*2*np.pi

    print(np.arctan(2/np.pi)*180/np.pi) # np.deg2rad(x) # np.rad2deg(x)
    x = acGon.x(a_rad)
    amax_rad = acGon.a_max()
    amax_turn = amax_rad/(2*np.pi)
    print("a max: " + str(amax_turn))
    print("x max: " + str(acGon.x(amax_rad)))
    plot(a_turn,x,amax_turn)

def plotTSAGod():
    a_max_mot = 550 # en pasos de motor: 16 por vuelta

    #af = np.linspace(0,a_max,50) # min, max, n_items
    a_mot = np.arange(0,a_max_mot,16) # min, max, step # en pasos de motor
    a_turn = a_mot/16
    a_rad = a_turn*2*np.pi

    # angulo beta del límite de torcedura
    # print(np.arctan(2/np.pi)*180/np.pi) # np.deg2rad(x) # np.rad2deg(x)

    x = acGod.x(a_rad)
    amax_rad = acGod.a_max()
    amax_turn = amax_rad/(2*np.pi)
    print("a max: " + str(amax_turn))
    print("x max: " + str(acGod.x(amax_rad)))
    plot(a_turn,x,amax_turn)


def plot(x, y, max):

    fig, ax = plt.subplots()
    ax.plot(x,y,'m.--',linewidth=1,markersize=3,color='magenta')
    ax.vlines(max,0,20,linewidth=1,color='green')

    ax.grid()
    ax.set_title("Acortamiento de la cuerda trenzada")

    ax.set_xlabel("Angulo de rotación del motor " + "alpha/2PI [vueltas]")
    ax.set_ylabel("Desplazamiento x [mm]")

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


def plotDX(a):
    y = ac.dx(a)
    #y1 = ac1.dx(b)

    fig, ax = plt.subplots()
    # ax.plot(a,(act.L - y),'m.--',linewidth=1,markersize=3,color='magenta')
    # ax.plot(a,(act1.L - y1),'bo--',linewidth=1,markersize=3,color='blue')
    ax.plot(a,(y),'m.--',linewidth=1,markersize=3,color='magenta')
    #ax.plot(b,(ac.L-y1),'bo--',linewidth=1,markersize=3,color='blue')
    #ax.plot(a,(ac.L-y),'bo--',linewidth=1,markersize=3,color='blue')

    ax.grid()
    ax.set_title("Acortamiento de la cuerda trenzada")

    #ax.set_xlabel('\frac{\alpha}{2*\pi} [vueltas]')
    #ax.set_ylabel("Distancia x(r'$\alpha$') [mm]")
    ax.set_xlabel("Angulo de rotación del motor " + "alpha/2PI [vueltas]")
    ax.set_ylabel("Distancia x(alpha) [mm]")

    #fig.savefig("figure1.pdf")
    plt.show()


def plotRT(a):
    y = act.RT(a)
    y1 = act1.RT(a)

    fig, ax = plt.subplots()
    # ax.plot(a,(act.L - y),'m.--',linewidth=1,markersize=3,color='magenta')
    # ax.plot(a,(act1.L - y1),'bo--',linewidth=1,markersize=3,color='blue')
    ax.plot(a,(y),'m.--',linewidth=1,markersize=3,color='magenta')
    ax.plot(a,(y1),'bo--',linewidth=1,markersize=3,color='blue')

    ax.grid()
    ax.set_title("Razón de transmisión")
    #ax.set_xlabel("Angulo de rotación del motor" + r'\alpha/2PI [vueltas]')
    #ax.set_xlabel('\frac{\alpha}{2*\pi} [vueltas]')
    ax.set_ylabel("Distancia x(alpha) [mm]")

    #fig.savefig("figure1.pdf")
    plt.show()


if __name__ == '__main__':
    main()
