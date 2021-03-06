# -*- coding: utf-8 -*-
"""
Created on Fri Mar 26 14:13:34 2021

@author: casmi
"""

import matplotlib.pyplot as plt
from datetime import datetime
import pandas as pd
import pylab as pl

#%%
# =============================================================================
# data = pd.read_csv('USGS_v3_fieldtest_jan15.csv')
# USGS_time = data['Time']
# USGS_values = data['USGS']
# 
# data2 = pd.read_csv('ARTS_v3_fieldtest_jan15.csv')
# ARTS_time = data2['Time']
# ARTS_values = data2['ARTS']
# 
# 
# pl.xticks(rotation = 90)
# plt.plot_date(USGS_time, USGS_values, linestyle='solid')
# plt.plot_date(ARTS_time, ARTS_values, linestyle = 'solid')
# =============================================================================

#%%
# =============================================================================
# data = pd.read_csv('USGS_v3_fieldtest_jan15.csv')
# USGS_time = data['Time']
# print(USGS_time)
# 
# USGS_time = pd.to_datetime(USGS_time)
# print(USGS_time)
# data.set_index(USGS_time)
# data.plot()
# =============================================================================

#%%
# =============================================================================
# ax = plt.gca()
# 
# data = pd.read_csv('USGS_v3_fieldtest_jan15.csv')
# USGS_time = data['Time']
# USGS_values = data['USGS']
# 
# data2 = pd.read_csv('ARTS_v3_fieldtest_jan15.csv')
# ARTS_time = data2['Time']
# ARTS_values = data2['ARTS']
# 
# ax.scatter(USGS_time, USGS_values)
# ax.scatter(ARTS_time, ARTS_values)
# =============================================================================

#%%

df = pd.read_csv('USGS_v3_fieldtest_jan15.csv',parse_dates=     {"Datetime" : [0,1]})
print(df)
df2 = pd.read_csv('ARTS_v3_fieldtest_jan15.csv', parse_dates = {"Datetime" : [0,1]})
#df.Datetime=pd.to_datetime(df.Datetime)
df.set_index('Datetime')

plt.figure(figsize=(6.5,3))
plt.plot(df['Datetime'], df['USGS'], '-v', label='USGS')
plt.plot(df2['Datetime'], df2['ARTS'], '--o', label='ARTS')
plt.legend(framealpha=1)
plt.xlabel('time')
plt.ylabel('distance (cm)')
plt.tight_layout()
plt.grid(True)
