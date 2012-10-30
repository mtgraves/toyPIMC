#!/usr/bin/python 
#
# gensubmit.py
# Adrian Del Maestro
# 06.03.2009
#
# Generate a torque submit script for the pimc code which creates
# a pbs file for various sets of parameters.

import os,sys
from argparse import ArgumentParser

def bluemoon(staticPIMCOps,numOptions,optionValue,outName,run):
    ''' Write a pbs submit script for bluemoon '''

    # Open the pbs file and write its header
    fileName = 'submit-pimc%s_bluemoon.pbs' % run
    pbsFile = open(fileName,'w')
    pbsFile.write('''#!/bin/bash
#PBS -S /bin/bash\n
#PBS -l nodes=1:ppn=1
#PBS -l walltime=30:00:00
#PBS -N QHO_PIMC
#PBS -V
#PBS -j oe
#PBS -o output/pimc-${PBS_JOBID}\n
#PBS -M Max.Graves@uvm.edu
#PBS -m n\n
# Start job script
cd $HOME/projects/toyPIMC
echo \"Starting run at: `date`\"

case ${PBS_ARRAYID} in\n''')

    # Create the command string and make the case structure
    for n in range(numOptions):
        command =  './pimc.e '

        for flag,val in optionValue.iteritems():
            command += '-%s %s ' % (flag,val[n])
        command += staticPIMCOps
        pbsFile.write('%d)\nsleep %d\n%s\n;;\n' % (n,2*n,command))
    
    pbsFile.write('esac\necho \"Finished run at: `date`\"')
    pbsFile.close();
    
    print '\nSubmit jobs with: qsub -t 0-%d %s\n' % (numOptions-1,fileName)


# -----------------------------------------------------------------------------
# Begin Main Program 
# -----------------------------------------------------------------------------
def main(): 

    # setup the command line parser options 
    parser = ArgumentParser(description="Build submission scripts for bluemoon") 
    parser.add_argument("file", help='configuration file')
    parser.add_argument('-r', type=str, dest="run", default="",
            help="optional JobId number that will be added to the scripts name")
    # parse the command line options
    args = parser.parse_args() 
    inFileName = args.file

    # We open up the input file, and read in all lines.
    inFile = open(inFileName,'r')
    inLines = inFile.readlines()

    # The first line of the file contains all static pimc options
    staticPIMCOps = inLines[0].rstrip('\n')

    # The next lines contains the short-name of the pimc option
    # and a list of values.  
    optionValue = {}
    numOptions = []
    for line in inLines[1:]:
        option = line.split()
        flag = option.pop(0)
        # Now we determine if we have a range or if we have actually included the values
        if option[0].find(':') != -1:

            # determine if we have floats or ints
            if option[0].find('.') != -1:
                type_converter = lambda x: float(x)
            else:
                type_converter = lambda x: int(x)

            dataRange = option[0].split(':')

            # Parse the range string
            dataMin = type_converter(dataRange[0])
            dataMax = type_converter(dataRange[1])
            dataStep = type_converter(dataRange[2])

            # construct the option list
            vals = [dataMin]
            while vals[-1] < dataMax:
                vals.append(vals[-1]+dataStep)

            # assign the option list
            optionValue[flag] = vals
            numOptions.append(len(vals))
        else:
            # store the typed out values
            optionValue[flag] = option
            numOptions.append(len(option))
    
    # We try to extract the temperature and volume to make an output string
    outName = ''
    findInput = staticPIMCOps.split();
    n = 0
    for input in findInput:
        if input == '-T':
            outName += '-%06.3f' % float(findInput[n+1])
            break
        n += 1
    n = 0
    for input in findInput:
        if input == '-L':
            outName += '-%07.3f' % float(findInput[n+1])
            break
        n += 1

    # We make sure all option strings have the same length 
    if len(numOptions) > 0:
        for n in numOptions:
            if n != numOptions[0]:
                print 'Not all parameters have the same number of values!'
                sys.exit()
        numOptions = numOptions[0]

    bluemoon(staticPIMCOps,numOptions,optionValue,outName,args.run)


# ----------------------------------------------------------------------
if __name__ == "__main__": 
    main()
