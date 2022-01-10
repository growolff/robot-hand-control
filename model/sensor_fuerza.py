# -*- coding: utf-8 -*-

import numpy as np

# distancia del pivote al sensor en m
Ds = 0.0185
# distancia del pivote al eje de rotacion en m
de = 0.012

# Fs es la fuerza medida en el sensor en kg
Fs = 1

# tension de la cuerda
t = Fs * Ds / de

print(t)
