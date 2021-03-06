# -*- coding: utf-8 -*-
"""
Created on Tue Mar  2 15:39:04 2021

@author: casmi
"""

# import modules
import IPython as IP
IP.get_ipython().magic('reset -sf')


from pandas import read_csv
#from matplotlib import pyplot

import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as md
import datetime as dt
import numpy as np
import scipy as sp
import json as json
import scipy.linalg as solve
from matplotlib import colors


plt.close('all')


# set default fonts and plot colors
plt.rcParams.update({'image.cmap': 'viridis'})
cc = plt.rcParams['axes.prop_cycle'].by_key()['color']
plt.rcParams.update({'font.serif':['Times New Roman', 'Times', 'DejaVu Serif',
 'Bitstream Vera Serif', 'Computer Modern Roman', 'New Century Schoolbook',
 'Century Schoolbook L',  'Utopia', 'ITC Bookman', 'Bookman', 
 'Nimbus Roman No9 L', 'Palatino', 'Charter', 'serif']})
plt.rcParams.update({'font.family':'serif'})
plt.rcParams.update({'font.size': 10})
plt.rcParams.update({'mathtext.rm': 'serif'})
plt.rcParams.update({'mathtext.fontset': 'custom'})
plt.close('all')


#%%



USGS = read_csv('USGS_v3_fieldtest_jan15.csv', header=0, parse_dates=[0], index_col=0, squeeze=True)

usgs_tt = np.linspace(0,12,24) 
usgs_yy = USGS.values




ARTS = read_csv('ARTS_v3_fieldtest_jan15.csv', header=0, parse_dates=[0], index_col=0, squeeze=True)
ARTS_tt = np.linspace(0,12,24) 
ARTS_yy = ARTS.values


plt.figure(figsize=(6.5,3))
plt.plot(usgs_tt,usgs_yy, '-v', label='USGS')
plt.plot(ARTS_tt,ARTS_yy, '--o', label='ARTS')
plt.legend(framealpha=1)
plt.xlabel('time (s)')
plt.ylabel('distance (cm)')
plt.tight_layout()
plt.grid(True)
plt.savefig('image.png',dpi=500)



