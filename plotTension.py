import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as md
import numpy as np
import datetime as dt
import time

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

headers = ['Time','Indice', 'Medio', 'Pulgar','Celda_Carga']

df = pd.read_csv('log_datos/tolueno.csv',delimiter=",", names=headers)

#time = pd.Timestamp(df.Time)
indice = df.Indice.values
medio = df.Medio
pulgar = df.Pulgar
celda = df.Celda_Carga

#time_formatted = time.strftime('%Y-%m-%d %X')
#print(time_formatted)

fig, ax = plt.subplots()

ax.plot(indice,'m-',linewidth=1,markersize=2,label='indice')
ax.plot(medio,'g-',linewidth=1,markersize=2,label='medio')
ax.plot(pulgar,'b-',linewidth=1,markersize=2,label='pulgar')
xfmt = md.DateFormatter('%M:%S')
#print(xfmt)
ax.xaxis.set_major_formatter(xfmt)

ax.legend(loc=2)

plt.show()
