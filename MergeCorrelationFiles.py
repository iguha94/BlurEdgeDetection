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

NoiseLevel=['SNR6/','SNR12/','SNR18/']
Prefix=["SNR-6","SNR-12","SNR-18"]

basepath=sys.argv[1]
wb=Workbook()
sheet1=wb.add_sheet('SNR6')
sheet2=wb.add_sheet('SNR12')
sheet3=wb.add_sheet('SNR18')

sheet1.write(0,0,'EZ_TrueBlur')
sheet1.write(0,1,'EZ_ComputedBlur')
sheet1.write(0,3,'ScaleBased_TrueBlur')
sheet1.write(0,4,'ScaleBased_ComputedBlur')

sheet2.write(0,0,'EZ_TrueBlur')
sheet2.write(0,1,'EZ_ComputedBlur')
sheet2.write(0,3,'ScaleBased_TrueBlur')
sheet2.write(0,4,'ScaleBased_ComputedBlur')

sheet3.write(0,0,'EZ_TrueBlur')
sheet3.write(0,1,'EZ_ComputedBlur')
sheet3.write(0,3,'ScaleBased_TrueBlur')
sheet3.write(0,4,'ScaleBased_ComputedBlur')

for j in range(0,3):
    sheet=wb.get_sheet(j)
    rowcount1=1
    rowcount2=1
    for i in range(1,16):
        if i==2 or i==7 or i==10:
            continue
        id=str(i)
        basedirectory=basepath+'Phantom'+id+'/'
        datadirectory=basedirectory+NoiseLevel[j]
        ezcorrfile=datadirectory+'EZCorr.csv'
        
        with open(ezcorrfile, mode='r') as csv_file:
            csv_reader=csv.DictReader(csv_file)
            for row in csv_reader:
                sheet.write(rowcount1,0,float(row['TrueBlur']))
                sheet.write(rowcount1,1,float(row['AppliedBlur']))
                rowcount1=rowcount1+1
        
        scalecorrfile=datadirectory+'ScaleCorr.csv'

        with open(scalecorrfile, mode='r') as csv_file:
            csv_reader=csv.DictReader(csv_file)
            for row in csv_reader:
                sheet.write(rowcount2,3,float(row['TrueBlur']))
                sheet.write(rowcount2,4,float(row['AppliedBlur']))
                rowcount2=rowcount2+1
    
wb.save(basepath+'MergedCorrelationFile.xls')
        