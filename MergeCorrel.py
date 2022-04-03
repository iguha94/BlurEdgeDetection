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
#option = int(sys.argv[2])
EZ_computed =np.array([]).astype(np.float)
EZ_true = np.array([]).astype(np.float)
cur_EZ_computed = np.array([]).astype(np.float)
cur_EZ_true = np.array([]).astype(np.float)
corr=0
ccc=0
wb=Workbook()
sheet1=wb.add_sheet('SNR6')
sheet2=wb.add_sheet('SNR12')
sheet3=wb.add_sheet('SNR18')
for id in Prefix:
    filename=basepath+id+'.xlsx' #for ez use .xlsx , for scale use .xls
    xl_workbook = xlrd.open_workbook(filename)
    sheet_names = sheets#xl_workbook.sheet_names()
    EZ_computed=np.array([]).astype(np.float)
    EZ_true=np.array([]).astype(np.float)
    for sheet in sheet_names:
        xl_sheet = xl_workbook.sheet_by_name(sheet)
        cur_EZ_computed=[]
        cur_EZ_true=[]
        for row_id in range(0,xl_sheet.nrows):
            cur_EZ_computed.append(float(xl_sheet.cell(row_id, 2).value))  # for ez index will be 2 and 3 resp,else (0,1)
            cur_EZ_true.append(float(xl_sheet.cell(row_id, 3).value)/10.0)  # for EZ divide by 10

        EZ_computed=np.append(EZ_computed,cur_EZ_computed) # for ez index will be 2 and 3 resp,else (0,1)
        EZ_true=np.append(EZ_true,cur_EZ_true)  # for EZ divide by 10
    sheet=wb.get_sheet(id)
    for x in range(len(EZ_computed)):
        sheet.write(x,1,EZ_true[x])
        sheet.write(x,2,EZ_computed[x])
    
wb.save(basepath+'CorrelationAll15.xls')
