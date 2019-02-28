# -*- coding: utf-8 -*-
"""
Created on Wed Feb 13 17:27:41 2019

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

sheet1.write(0,0,'Phantom ID')
sheet1.write(0,1,'EZ Correlation')
sheet1.write(0,2,'ScaleBased Correlation')
sheet1.write(0,3,'EZ Mean True Blur')
sheet1.write(0,4,'EZ Mean Computed Blur')
sheet1.write(0,5,'ScaleBased Mean True Blur')
sheet1.write(0,6,'ScaleBased Mean Computed Blur')
sheet1.write(0,7,'EZ Noise stdv')
sheet1.write(0,8,'ScaleBased Noise stdv')
sheet1.write(0,9,'EZ Kernel stdv')
sheet1.write(0,10,'ScaleBased Kernel stdv')
sheet1.write(0,11,'Canny Kernel stdv')

sheet2.write(0,0,'Phantom ID')
sheet2.write(0,1,'EZ Correlation')
sheet2.write(0,2,'ScaleBased Correlation')
sheet2.write(0,3,'EZ Mean True Blur')
sheet2.write(0,4,'EZ Mean Computed Blur')
sheet2.write(0,5,'ScaleBased Mean True Blur')
sheet2.write(0,6,'ScaleBased Mean Computed Blur')
sheet2.write(0,7,'EZ Noise stdv')
sheet2.write(0,8,'ScaleBased Noise stdv')
sheet2.write(0,9,'EZ Kernel stdv')
sheet2.write(0,10,'ScaleBased Kernel stdv')
sheet2.write(0,11,'Canny Kernel stdv')

sheet3.write(0,0,'Phantom ID')
sheet3.write(0,1,'EZ Correlation')
sheet3.write(0,2,'ScaleBased Correlation')
sheet3.write(0,3,'EZ Mean True Blur')
sheet3.write(0,4,'EZ Mean Computed Blur')
sheet3.write(0,5,'ScaleBased Mean True Blur')
sheet3.write(0,6,'ScaleBased Mean Computed Blur')
sheet3.write(0,7,'EZ Noise stdv')
sheet3.write(0,8,'ScaleBased Noise stdv')
sheet3.write(0,9,'EZ Kernel stdv')
sheet3.write(0,10,'ScaleBased Kernel stdv')
sheet3.write(0,11,'Canny Kernel stdv')

for i in range(1,16):
    id=str(i)
    basedirectory=basepath+'Phantom'+id+'/'
    for j in range(0,3):
        sheet=wb.get_sheet(j)
        sheet.write(i,0,'Phantom'+id)
        datadirectory=basedirectory+NoiseLevel[j]
        
        dataListA=[]
        dataListB=[]
        ezcorrfile=datadirectory+'EZCorr.csv'

        with open(ezcorrfile, mode='r') as csv_file:
            csv_reader=csv.DictReader(csv_file)
            for row in csv_reader:
                dataListA.append(row['TrueBlur'])
                dataListB.append(row['AppliedBlur'])
        
        X=np.array(dataListA).astype(np.float)
        Y=np.array(dataListB).astype(np.float)
        correlation=pearsonr(X,Y)
        corr=correlation[0]
        print('EZcorrel: ',round(corr,2))
        sheet.write(i,1,round(corr,2))
        sheet.write(i,3,round(np.mean(X),2))
        sheet.write(i,4,round(np.mean(Y),2))
        
        dataListA=[]
        dataListB=[]
        scalecorrfile=datadirectory+'ScaleCorr.csv'

        with open(scalecorrfile, mode='r') as csv_file:
            csv_reader=csv.DictReader(csv_file)
            for row in csv_reader:
                dataListA.append(row['TrueBlur'])
                dataListB.append(row['AppliedBlur'])
        
        X=np.array(dataListA).astype(np.float)
        Y=np.array(dataListB).astype(np.float)
        correlation=pearsonr(X,Y)
        corr=correlation[0]
        print('Scalecorrel: ',round(corr,2))
        sheet.write(i,2,round(corr,2))
        sheet.write(i,5,round(np.mean(X),2))
        sheet.write(i,6,round(np.mean(Y),2))
    
wb.save(basepath+'CorrelationAll15.xls')
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
