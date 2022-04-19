import SimpleITK
import matplotlib.pyplot as plt
import sys 
import os 
import numpy as np 
from PIL import Image

imagefile=sys.argv[1]
blurpointsfile=sys.argv[2]
outfile=sys.argv[3]

image=SimpleITK.ReadImage(imagefile)
array=SimpleITK.GetArrayFromImage(image)

size=array.shape
outarr=np.zeros(size)

file=open(blurpointsfile,'r')
lines=file.readlines()

for line in lines:
    line=line.rstrip("\n")
    tokens=line.split(',')
    print(line)
    xcord=int(tokens[0])
    ycord=int(tokens[1])
    sigma=int(tokens[2])
    outarr[ycord][xcord]=sigma

#outimage=SimpleITK.GetImageFromArray(outarr)
#SimpleITK.WriteImage(outimage,outfile)

img = Image.fromarray(outarr)
img = img.convert("L")
img.save(outfile,format="png")
img.show()

