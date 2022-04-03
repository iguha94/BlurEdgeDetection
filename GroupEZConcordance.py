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

sheets=['1','3','5','6','7','8','9','10','12','13','14','15']
Prefix = ["SNR6", "SNR12", "SNR18"]

basepath = sys.argv[1]

for id in Prefix:
    print('--------------',id, '---------------------')
    filename=basepath+id+'.xls'
    xl_workbook = xlrd.open_workbook(filename)
    sheet_names = xl_workbook.sheet_names()
    for sheet in sheet_names:
        EZ_computed = []
        EZ_true = []
        xl_sheet = xl_workbook.sheet_by_name(sheet)
        for row_id in range(0,xl_sheet.nrows):
            EZ_computed.append((float(xl_sheet.cell(row_id,0).value)))
            EZ_true.append((float(xl_sheet.cell(row_id,1).value)))

        X = np.array(EZ_computed).astype(np.float)
        Y = np.array(EZ_true).astype(np.float)
        correlation = pearsonr(X, Y)
        corr = correlation[0]
        #print('EZcorrel: ', round(corr, 2))

        meanx = np.mean(X)
        stdvx = np.std(X)
        meany = np.mean(Y)
        stdvy = np.std(Y)

        numerator = 2 * corr * stdvx * stdvy
        denominator = pow(stdvx, 2) + pow(stdvy, 2) + pow((meanx - meany), 2)
        ccc = numerator / denominator
        bias=np.mean(np.subtract(EZ_computed,EZ_true))
        print('Phantom',sheet,': Correlation: ',round(corr,2),'CCC: ',round(ccc, 2),'bias: ',bias)

