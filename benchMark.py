#! -*-coding: utf-8-*-

from pathlib import Path
import os

benchMarkPathName = r'D:\ResultsOfISH\Others\Data'
outPutPathName = r'D:\Result\Others\Result'
statisticOutPathName = r'D:\Result\Others\Result\statistic\result.txt'
programPathName = r'.\x64\Release\TwoStageSearch2DStripPacking.exe'

benchMarkPath = Path(benchMarkPathName)

for benchmark in benchMarkPath.iterdir():
    partOfPath = str(benchmark).split('.')
    extName = partOfPath[-1]
    if extName == 'txt':
        end = -1
        for i in range(-1, -len(partOfPath) - 1, -1):
            if partOfPath[i] != 'txt':
                end = i + 1
                break

        caseName = ('.'.join(str(benchmark).split('.')[:end])).split('\\')[-1]
        os.system(programPathName + ' ' + caseName + ' "' + str(benchmark) + '" "' + outPutPathName + '" "' + statisticOutPathName +'"')
        print(caseName + " completed.", end="\n\n")

