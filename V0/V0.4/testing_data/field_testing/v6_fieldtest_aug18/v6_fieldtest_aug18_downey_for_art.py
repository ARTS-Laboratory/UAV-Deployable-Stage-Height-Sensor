# -*- coding: utf-8 -*-
"""
Created on Tue Aug 24 10:32:30 2021

@author: casmi
"""

import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime
import pandas as pd
import pylab as pl
import numpy as np
plt.rcParams["font.family"] = "Times New Roman"
#mpl.rcParams.update({'font.size': 14})

import IPython as IP
IP.get_ipython().magic('reset -sf')
plt.close('all')


df0 = pd.read_csv('USGS_aug18.csv',parse_dates=     {"Datetime" : [0,1]})
df1 = pd.read_csv('v6_fieldtest_aug18.csv',parse_dates=     {"Datetime" : [0,1]})




plt.figure(figsize=(7.4,3.5))
plt.plot(df0['Datetime'], df0['Stage (cm)'], '-', label='USGS 02169506', linewidth=0.75)
plt.plot(df1['Datetime'], df1['Flipped Dist'], '--', label='stage sensor package', linewidth=0.75)
plt.legend(loc=1, framealpha=1)
plt.xlabel('datetime')
plt.ylabel('distance (cm)')
plt.locator_params(axis="x", nbins=5)
plt.xticks(rotation=45)
plt.tight_layout()
plt.grid(True)
plt.savefig('v6_fieldtest_aug18_mirrored_for_art.pdf')
