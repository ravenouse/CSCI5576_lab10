#!/usr/bin/python3

import os

import sys
import getopt
import glob



def WriteOnePCfile(PElist,timeList,varName, plt_path):

    g = open(plt_path +'pc_' + varName,'w')
    
    for time in sorted(timeList):
        
        print( 'splot \\',file=g)
        for PE in sorted(PElist):
            print( '"'+varName+'_' + str(PE) + '_' + str(time) + '.plt" w l , \\',file=g)
        print( '',file=g)
        print( 'pause .01',file=g)
        
    g.close()


# ==
# ||
# || Main Program
# ||
# ==

def writePlotCmd(argv):
    if argv:
        plt_path = argv[0]
    else:
        plt_path = '../plt/'
    
    # List of all ptcl files
    ptclList = glob.glob(plt_path+"phi*.plt")
    print(ptclList)
    # Parse the filenames counting the number of PEs for this run

    PElist = []
    timeList = []
    
    for i in range(0,len(ptclList)):
        name = ptclList[i];
        tmp = name.split('_')

        PE  = tmp[1]
        if PE not in PElist:
            PElist.append(PE)

        tmp2 = tmp[2].split(".")
        time = int(tmp2[0])
        if time not in timeList:
            timeList.append(int(time))

            
    WriteOnePCfile(PElist,timeList,'phi', plt_path)


if __name__ == "__main__":
    writePlotCmd(sys.argv[1:])
