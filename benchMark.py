#! -*-coding: utf-8-*-

from pathlib import Path
import os

benchMarkPathName = r'D:\DataOfISH\Other'
outPutPathName = r'D:\Result\Others\Result'
statisticOutPathName = r'D:\Result\Others\Result\statistic\result.txt'
programPathName = r'.\x64\Release\TwoStageSearch2DStripPacking.exe'

benchMarkPath = Path(benchMarkPathName)

flag = False
count = 0
for benchmark in benchMarkPath.iterdir():
    partOfPath = str(benchmark).split('.')
    extName = partOfPath[-1]
    if extName == 'txt':
        count += 1
        caseName = ('.'.join(str(benchmark).split('.')[:-1])).split('\\')[-1]
        if not flag and caseName != 'zdf2':
        	print('case {0}: {1} has completed.'.format(count, caseName), end='\n\n')
        	continue
        else:
        	flag = True
        if flag:
        	print('case {0}: {1} begin...'.format(count, caseName))
        	os.system(programPathName + ' ' + caseName + ' "' + str(benchmark) + '" "' + outPutPathName + '" "' + statisticOutPathName +'"')
        	print('case {0}: {1} completed.'.format(count, caseName), end="\n\n")

