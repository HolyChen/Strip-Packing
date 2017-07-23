# -*- coding: utf-8 -*_

from pathlib import Path
import re

class Result:
    nameMap = {
                # C
                'lw161.txt': 'C11', 'lw172.txt': 'C12', 'lw163.txt': 'C13',
                'lw251.txt': 'C21', 'lw252.txt': 'C22', 'lw253.txt': 'C23',
                'lw281.txt': 'C31', 'lw292.txt': 'C32', 'lw283.txt': 'C33',
                'lw491.txt': 'C41', 'lw492.txt': 'C42', 'lw493.txt': 'C43',
                'lw731.txt': 'C51', 'lw732.txt': 'C52', 'lw733.txt': 'C53',
                'lw971.txt': 'C61', 'lw972.txt': 'C62', 'lw973.txt': 'C63',
                'lw1961.txt': 'C71', 'lw1972.txt': 'C72', 'lw1963.txt': 'C73'
                }

    def __init__(self, caseName, seed, fileName):
        self.caseName = caseName
        self.seed = int(seed)
        self.fileName = fileName
        self.testSet = self.classify()
        self.rename()


    def __repr__(self):
        # return '{0} {1} {2}'.format(self.caseName, self.seed, self.fileName)
        return '{0} {1} {2}'.format(self.testSet, self.caseName, self.seed)


    def __lt__(self, rhs):
        if self.testSet < rhs.testSet:
            return True
        if self.testSet == rhs.testSet:
            if self.caseName == rhs.caseName:
                return self.seed < rhs.seed
            else:
                patternName = re.compile(r'\d+')
                nameL = patternName.split(self.caseName)[0]
                nameR = patternName.split(rhs.caseName)[0]
                if nameL < nameR :
                    return True
                elif nameL == nameR:
                    pattern = re.compile(r'\D+')
                    numL = pattern.split(self.caseName)
                    numR = pattern.split(rhs.caseName)
                    length = min(len(numL), len(numR))
                    for i in range(0, length):
                        if numL[i].isdigit() and numR[i].isdigit():
                            if int(numL[i]) < int(numR[i]):
                                return True
                            elif int(numL[i]) > int(numR[i]):
                                return False
                    else:
                        return self.caseName < rhs.caseName


        return False


    def classify(self):
        fullCaseFileName = self.caseName + '.txt'
        # C
        if fullCaseFileName in ['lw161.txt', 'lw163.txt', 'lw172.txt', 'lw251.txt',
                'lw252.txt', 'lw253.txt', 'lw281.txt', 'lw283.txt','lw292.txt', 'lw491.txt',
                'lw492.txt', 'lw493.txt', 'lw731.txt', 'lw732.txt', 'lw733.txt', 'lw971.txt', 
                'lw972.txt', 'lw973.txt', 'lw1961.txt', 'lw1963.txt', 'lw1972.txt']:
            return 'C'
        elif fullCaseFileName in ['n1.txt', 'n2.txt', 'n3.txt', 'n4.txt', 'n5.txt', 'n6.txt', 'n7.txt',
                'n8.txt', 'n9.txt', 'n10.txt', 'n11.txt', 'n12.txt', 'n13.txt']:
            return 'N'
        elif fullCaseFileName in ['50.txt', '100.txt', '500.txt', '1000.txt', '5000.txt', '10000.txt', '15000.txt']:
            return 'CX'
        elif fullCaseFileName in ['N1a.txt', 'N1b.txt', 'N1c.txt', 'N1d.txt', 'N1e.txt',
                                  'N2a.txt', 'N2b.txt', 'N2c.txt', 'N2d.txt', 'N2e.txt',
                                  'N3a.txt', 'N3b.txt', 'N3c.txt', 'N3d.txt', 'N3e.txt',
                                  'N4a.txt', 'N4b.txt', 'N4c.txt', 'N4d.txt', 'N4e.txt',
                                  'N5a.txt', 'N5b.txt', 'N5c.txt', 'N5d.txt', 'N5e.txt',
                                  'N6a.txt', 'N6b.txt', 'N6c.txt', 'N6d.txt', 'N6e.txt',
                                  'N7a.txt', 'N7b.txt', 'N7c.txt', 'N7d.txt', 'N7e.txt']:
            return 'NT'
        elif fullCaseFileName in ['T1a.txt', 'T1b.txt', 'T1c.txt', 'T1d.txt', 'T1e.txt',
                                  'T2a.txt', 'T2b.txt', 'T2c.txt', 'T2d.txt', 'T2e.txt',
                                  'T3a.txt', 'T3b.txt', 'T3c.txt', 'T3d.txt', 'T3e.txt',
                                  'T4a.txt', 'T4b.txt', 'T4c.txt', 'T4d.txt', 'T4e.txt',
                                  'T5a.txt', 'T5b.txt', 'T5c.txt', 'T5d.txt', 'T5e.txt',
                                  'T6a.txt', 'T6b.txt', 'T6c.txt', 'T6d.txt', 'T6e.txt',
                                  'T7a.txt', 'T7b.txt', 'T7c.txt', 'T7d.txt', 'T7e.txt']:
            return 'NT'
        elif fullCaseFileName in ['nice1.txt', 'path1.txt', 'nice2.txt', 'path2.txt', 'nice3.txt', 'path3.txt',
                                  'nice4.txt', 'path4.txt', 'nice5.txt', 'path5.txt', 'nice6.txt', 'path6.txt']:
            return 'NP'
        elif fullCaseFileName in ['nice101t.txt.txt', 'nice102t.txt.txt', 'nice105t.txt.txt', 'nice11t.txt.txt', 'nice12t.txt.txt',
                                  'nice15t.txt.txt', 'nice21t.txt.txt', 'nice22t.txt.txt', 'nice25t.txt.txt', 'nice3.txt.txt',
                                  'nice31t.txt.txt', 'nice32t.txt.txt', 'nice35t.txt.txt', 'nice41t.txt.txt', 'nice42t.txt.txt',
                                  'nice45t.txt.txt', 'nice51t.txt.txt', 'nice52t.txt.txt', 'nice55t.txt.txt', 'nice6.txt.txt',
                                  'nice61t.txt.txt', 'nice62t.txt.txt', 'nice65t.txt.txt', 'nice71t.txt.txt', 'nice72t.txt.txt', 'nice75t.txt.txt',
                                  'nice81t.txt.txt', 'nice82t.txt.txt', 'nice85t.txt.txt', 'nice91t.txt.txt', 'nice92t.txt.txt', 'nice95t.txt.txt',
                                  'path101t.txt.txt', 'path102t.txt.txt', 'path105t.txt.txt', 'path11t.txt.txt', 'path12t.txt.txt',
                                  'path15t.txt.txt', 'path21t.txt.txt', 'path22t.txt.txt', 'path25t.txt.txt', 'path3.txt.txt',
                                  'path31t.txt.txt', 'path32t.txt.txt', 'path35t.txt.txt', 'path41t.txt.txt', 'path42t.txt.txt',
                                  'path45t.txt.txt', 'path51t.txt.txt', 'path52t.txt.txt', 'path55t.txt.txt', 'path6.txt.txt',
                                  'path61t.txt.txt', 'path62t.txt.txt', 'path65t.txt.txt', 'path71t.txt.txt', 'path72t.txt.txt', 'path75t.txt.txt',
                                  'path81t.txt.txt', 'path82t.txt.txt', 'path85t.txt.txt', 'path91t.txt.txt', 'path92t.txt.txt', 'path95t.txt.txt']:
            return 'NPT'
        elif fullCaseFileName in ['ngcut01.txt', 'ngcut02.txt', 'ngcut03.txt', 'ngcut04.txt', 'ngcut05.txt', 'ngcut06.txt',
                                  'ngcut07.txt', 'ngcut08.txt', 'ngcut09.txt', 'ngcut10.txt', 'ngcut11.txt', 'ngcut12.txt',
                                  'gcut01.txt', 'gcut02.txt', 'gcut03.txt', 'gcut04.txt', 'gcut05.txt', 'gcut06.txt', 'gcut07.txt',
                                  'gcut08.txt', 'gcut09.txt', 'gcut10.txt', 'gcut11.txt', 'gcut12.txt', 'gcut13.txt',
                                  'cgcut1.txt', 'cgcut2.txt', 'cgcut3.txt', 
                                  'beng01.txt', 'beng02.txt', 'beng03.txt', 'beng04.txt', 'beng05.txt',
                                  'beng06.txt', 'beng07.txt', 'beng08.txt', 'beng09.txt', 'beng10.txt']:
            return '2sp'
        elif re.compile(r'C_\d+_\d+').fullmatch(self.caseName):
            return 'BWMV'
        elif re.compile(r'zdf\d+').fullmatch(self.caseName):
            return 'ZDF'
        elif fullCaseFileName in 'babu.txt':
            return 'Babu'


    def rename(self):
        fullCaseFileName = self.caseName + '.txt'
        if self.testSet == 'C':
            self.caseName = self.nameMap[fullCaseFileName]
            return
        elif self.testSet == 'CX':
            self.caseName = self.caseName + 'cx'
            return
        elif self.testSet == 'N':
            self.caseName = self.caseName.upper()
            return
        # NPT
        elif self.testSet == 'NPT':
            name = self.caseName.split('t.txt')[0]
            name0 = name[0:4]
            nameK = name[4:-1]
            nameN = name[-1]
            self.caseName = name0 + '_' + nameN + 't_' + nameK
        elif self.testSet == 'BWMV':
            name = self.caseName.split('.txt')[0]
            order = int(name.split('_')[1])
            num = int(name.split('_')[-1])
            n =  str(20 * ((((num - 1) % 50) // 10) + 1))
            k = str((num - 1) % 10 + 1)
            self.caseName = 'C_' + str(order) + '_' + n + '_' + k


    def readAll(self):
        with open(self.fileName) as fin:
            self.height = float(fin.readline().split('h: ')[-1])
            self.time = float(fin.readline().split('time: ')[-1])
            l3 = fin.readline().split(' ')
            self.width, self.lowerBound = float(l3[0]), float(l3[1])
            self.n = float(fin.readline())

  

resultDirName = './Result'
statisticFileName = './statistic.txt'

resultList = []

resultDir = Path(resultDirName)
for resultFile in resultDir.iterdir():
    fullFileName = str(resultFile)
    parts = fullFileName.split('.')
    if parts[-1] != 'pack':
        continue
    otherPart = '.'.join(parts[:-1])
    resultName = otherPart.split('\\')[-1].split('_seed')
    caseName = resultName[0]
    seed = int(resultName[1])
    result = Result(caseName, seed, fullFileName)
    resultList.append(result)

resultList.sort()

with open(statisticFileName, 'w') as fout:
    fout.write('Case n W LB H1 T1 H2 T2 H3 T3 H4 T4 H5 T5 H6 T6 H7 T7 H8 T8 H9 T9 H10 T10\n')

    lastSet = resultList[0].testSet
    for result in resultList:
        result.readAll()

        if result.seed == 1:
            if lastSet != result.testSet:
                fout.write('\n')
                lastSet = result.testSet
            fout.write('{0} {1} {2} {3}'.format(result.caseName, result.n, result.width, result.lowerBound))

        fout.write(' {0} {1}'.format(result.height, result.time))
        if result.seed == 10:
            fout.write('\n')








