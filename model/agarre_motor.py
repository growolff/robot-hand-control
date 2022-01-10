# -*- coding: utf-8 -*-
"""
Created on Wen Jan 6 2022
@author: growolff

determinacion de capacidad de motor
"""

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

'''
%% Gripping force

% fuerza que hacen todos los dedos al objeto [N]
% F = 10;
% masa del objeto que quiero tomar [Kg]
m = 1;
g = 9.8; % gravedad

% el objeto se puede estar moviendo
a = 0;

% coeficiente de roce est�tico
u = 1;
% factor de seguridad
sf = 1.2;

Fcrit = m*(g+a)/u * sf;
'''

M = 1 #kg
g = 9.8 # M/s2
a = 0

# roce estatico
u = 0.45 # silicona - plastico
# u = 0.6 # silicona - goma dura
# u = 0.5 # silicona - carton
# factor de seguridad
sf = 1.5

# fuerza agarre mano
F = M*(g+a)/u * sf

#fuerza agarre dedo
Fdedo = F/3

print(F, Fdedo)
