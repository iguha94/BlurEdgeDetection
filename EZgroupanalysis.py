# -*- coding: utf-8 -*-
"""
Created on Mon Feb 18 19:00:16 2019

@author: iguha
"""

import sys
import csv
from scipy.stats.stats import pearsonr
import re
import math
from xlwt import Workbook
import numpy as np
import xlrd

sheets=['1','3','4','6','8','9','11','12','13','14','15']
Prefix = ["SNR6", "SNR12", "SNR18"]

basepath = sys.argv[1]
option = int(sys.argv[2])
EZ_computed =np.array([]).astype(np.float)
EZ_true = np.array([]).astype(np.float)
cur_EZ_computed = np.array([]).astype(np.float)
cur_EZ_true = np.array([]).astype(np.float)
corr=0
ccc=0

def computerhoCCC(EZ_computed,EZ_true):
    X = EZ_computed #np.array(EZ_computed).astype(np.float)
    Y = EZ_true# np.array(EZ_true).astype(np.float)
    correlation = pearsonr(X, Y)
    corr = correlation[0]
    # print('EZcorrel: ', round(corr, 2))

    meanx = np.mean(X)
    stdvx = np.std(X)
    meany = np.mean(Y)
    stdvy = np.std(Y)

    numerator = 2 * corr * stdvx * stdvy
    denominator = pow(stdvx, 2) + pow(stdvy, 2) + pow((meanx - meany), 2)
    ccc = numerator / denominator
    return corr,ccc

for id in Prefix:
    filename=basepath+id+'.xls' #for ez use .xlsx , for scale use .xls
    xl_workbook = xlrd.open_workbook(filename)
    sheet_names = sheets#xl_workbook.sheet_names()
    if option == 1:
        EZ_computed=np.array([]).astype(np.float)
        EZ_true=np.array([]).astype(np.float)
    for sheet in sheet_names:
        xl_sheet = xl_workbook.sheet_by_name(sheet)
        cur_EZ_computed=[]
        cur_EZ_true=[]
        for row_id in range(0,xl_sheet.nrows):
            cur_EZ_computed.append(float(xl_sheet.cell(row_id, 0).value))  # for ez index will be 2 and 3 resp,else (0,1)
            cur_EZ_true.append(float(xl_sheet.cell(row_id, 1).value))  # for EZ divide by 10
        #print(len(cur_EZ_true),' , ',len(cur_EZ_computed))
        '''-----Comment this block for EZ correlation computation-----'''
        max_computed=np.max(cur_EZ_true)
        max_true=np.max(cur_EZ_computed)
        if max_computed>max_true:
            max=max_true
        else:
            max=max_computed
        if id=='SNR24':
            cur_EZ_true=np.clip(cur_EZ_true,0,max)
        '''-----Comment upto this for EZ correlation computation-----'''
        EZ_computed=np.append(EZ_computed,cur_EZ_computed) # for ez index will be 2 and 3 resp,else (0,1)
        EZ_true=np.append(EZ_true,cur_EZ_true)  # for EZ divide by 10

        corr,ccc=computerhoCCC(cur_EZ_computed,cur_EZ_true)
        meanerror = (np.abs(np.array(EZ_computed).astype(np.float) - np.array(EZ_true).astype(np.float))).mean(axis=None)
        print('Phantom ID: ',sheet,' Correlation: ',corr,' CCC: ',ccc,' Mean Error: ',(meanerror))
        print(sheet, ',',meanerror)
    print('------------------------------------------------------------')
    if option==1:
        corr,ccc=computerhoCCC(EZ_computed,EZ_true)
        meanerror=(np.abs(np.array(EZ_computed).astype(np.float)-np.array(EZ_true).astype(np.float))).mean(axis=None)
        print('SNR',id,': Cumulative Correlation: ',round(corr,2),' Cumulative CCC: ',round(ccc, 2), 'Mean squared Error: ',(meanerror))
if option!=1:
    corr, ccc = computerhoCCC(EZ_computed, EZ_true)
    meanerror = (np.abs(np.array(EZ_computed).astype(np.float) - np.array(EZ_true).astype(np.float))).mean(axis=None)
    print('Considering all phantoms of all SNR; Cumulative Correlation: ', round(corr, 2), ' Cumulative CCC: ', round(ccc, 2),'Mean squared Error: ',(meanerror))
