# -*- coding: utf-8 -*-
"""
Created on Sat Jan 1 2021
@author: growolff

Ecuaciones de los limites de los Dedos
"""

import numpy as np

# Largo de cuerda que hay que tirar para cerrar el dedo completo
# depende de los angulos maximos de flexion de las falanges
# y del radio donde se soporta la cuerda
# para el extensor es el radio del arco de las articulaciones r*theta
# para el flector es el punto donde se afirma la cuerda en la falange movil

def lArticulacion(th,r1,r2):
    rad = th*np.pi/180
    return round(np.sqrt(r1**2 + (r2*np.cos(np.pi/2-rad))**2) - r2*np.sin(np.pi/2-rad),1)

# angulos maximos de flexion de falanges 1:mcp, 2:pip, 3:dip
it = [90, 90, 70]
# radios para calcular el largo maximo del extensor
ir = [7.5, 6.5, 4.5]
# radios para calcular el largo maximo del flector
irMcp = [9.3, 7.75]
irPip = [6.75, 6.75]
irDip = [5.75, 5.75]

mt = [90, 90, 70]
mr = [7.5, 6.5, 4.5]
mrMcp = [9.3, 7.75]
mrPip = [6.75, 6.75]
mrDip = [5.75, 5.75]

pt = [70, 65, 0]
pr = [7.5, 6.5, 0]
prMcp = [7.75, 9.5]
prDip = [6.75, 7]

deg2rad = np.pi/180
iAext = []
iAflec= []
mAext = []
mAflec = []
pAext = []
pAflec = []


for i in range(len(it)):
    iAext.append(round(it[i]*ir[i],1))
    mAext.append(mt[i]*mr[i])
    pAext.append(pt[i]*pr[i])
    print(round(iAext[i]*deg2rad,1))
    #print(iAext[i]*deg2rad)

iAmcpflec = lArticulacion(it[0],irMcp[0],irMcp[1])
iApipflec = lArticulacion(it[1],irPip[0],irPip[1])
iAdipflec = lArticulacion(it[2],irDip[0],irDip[1])
iLtotalFlec = iAmcpflec + iApipflec + iAdipflec
print(iAmcpflec , iApipflec , iAdipflec,iLtotalFlec)

mAmcpflec = lArticulacion(mt[0],mrMcp[0],mrMcp[1])
mApipflec = lArticulacion(mt[1],mrPip[0],mrPip[1])
mAdipflec = lArticulacion(mt[2],mrDip[0],mrDip[1])
mLtotalFlec = mAmcpflec + mApipflec + mAdipflec
print(mAmcpflec,mApipflec,mAdipflec,mLtotalFlec)

pAmcpflec = lArticulacion(pt[0],prMcp[0],prMcp[1])
pAdipflec = lArticulacion(pt[1],prDip[0],prDip[1])
pLtotalFlec = pAmcpflec + pAdipflec
print(pAmcpflec,pAdipflec ,pLtotalFlec)
#print(iLtotalFlec, mLtotalFlec, pLtotalFlec)

iLtotalExt = (iAext[0]+iAext[1]+iAext[2])*deg2rad
mLtotalExt = (mAext[0]+mAext[1]+mAext[2])*deg2rad
pLtotalExt = (pAext[0]+pAext[1]+pAext[2])*deg2rad
print(iLtotalExt, mLtotalExt, pLtotalExt)
