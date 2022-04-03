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

sheet1.write(0,0,'Phantom_ID')
sheet1.write(0,1,'EZ_Correlation')
sheet1.write(0,2,'ScaleBased_Correlation')
sheet1.write(0,3,'EZ_Mean_True_Blur')
sheet1.write(0,4,'EZ_Mean_omputed_Blur')
sheet1.write(0,5,'ScaleBased_Mean_True Blur')
sheet1.write(0,6,'ScaleBased_Mean_Computed_Blur')
sheet1.write(0,7,'EZ_CCC')
sheet1.write(0,8,'ScaleBased_CCC')
sheet1.write(0,9,'EZ_Kernel_stdv')
sheet1.write(0,10,'ScaleBased_Kernel_stdv')
sheet1.write(0,11,'Canny Kernel stdv')

sheet2.write(0,0,'Phantom_ID')
sheet2.write(0,1,'EZ_Correlation')
sheet2.write(0,2,'ScaleBased_Correlation')
sheet2.write(0,3,'EZ_Mean_True_Blur')
sheet2.write(0,4,'EZ_Mean_omputed_Blur')
sheet2.write(0,5,'ScaleBased_Mean_True Blur')
sheet2.write(0,6,'ScaleBased_Mean_Computed_Blur')
sheet2.write(0,7,'EZ_CCC')
sheet2.write(0,8,'ScaleBased_CCC')
sheet2.write(0,9,'EZ_Kernel_stdv')
sheet2.write(0,10,'ScaleBased_Kernel_stdv')
sheet2.write(0,11,'Canny Kernel stdv')

sheet3.write(0,0,'Phantom_ID')
sheet3.write(0,1,'EZ_Correlation')
sheet3.write(0,2,'ScaleBased_Correlation')
sheet3.write(0,3,'EZ_Mean_True_Blur')
sheet3.write(0,4,'EZ_Mean_omputed_Blur')
sheet3.write(0,5,'ScaleBased_Mean_True Blur')
sheet3.write(0,6,'ScaleBased_Mean_Computed_Blur')
sheet3.write(0,7,'EZ_CCC')
sheet3.write(0,8,'ScaleBased_CCC')
sheet3.write(0,9,'EZ_Kernel_stdv')
sheet3.write(0,10,'ScaleBased_Kernel_stdv')
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

        meanx=np.mean(X)
        stdvx=np.std(X)
        meany=np.mean(Y)
        stdvy=np.std(Y)

        numerator = 2*corr*stdvx*stdvy
        denominator = pow(stdvx,2)+pow(stdvy,2)+pow((meanx-meany),2)
        ccc=numerator/denominator

        print('EZ CCC: ',round(ccc,2))

        sheet.write(i,1,round(corr,2))
        sheet.write(i,3,round(np.mean(X),2))
        sheet.write(i,4,round(np.mean(Y),2))
        sheet.write(i,7,round(ccc,2))
        
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

        meanx=np.mean(X)
        stdvx=np.std(X)
        meany=np.mean(Y)
        stdvy=np.std(Y)

        numerator = 2*corr*stdvx*stdvy
        denominator = pow(stdvx,2)+pow(stdvy,2)+pow((meanx-meany),2)
        ccc=numerator/denominator

        print('scale CCC: ',round(ccc,2))

        sheet.write(i,2,round(corr,2))
        sheet.write(i,5,round(np.mean(X),2))
        sheet.write(i,6,round(np.mean(Y),2))
        sheet.write(i,8,round(ccc,2))
    
wb.save(basepath+'ConcordCorrelationAll15.xls')
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
