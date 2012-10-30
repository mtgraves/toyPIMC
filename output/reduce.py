# reduce-one.py
# Adrian Del Maestro
# 09.03.2009
# 
# Reduce and average results for a single PIMC run based on a single parameter
# which varies.  This could be density for fixed system size, etc.

import os,sys,glob
import loadgmt,kevent
import pimchelp
from optparse import OptionParser
from pylab import *

# ======================================================================
# ----------------------------------------------------------------------
# THIS SECTION WAS YANKED FROM PIMCHELP.PY
# ----------------------------------------------------------------------
def getFileList(self,type,idList=None):
    ''' Get a list of input files based on their type, and possibly a number
            of unique ID's'''

    fileNames = []

    # We want all the file names here
    if not idList:
        lsCommand = 'ls -1 %s%s-%s-%s' % (self.baseDir,self.prefix,type,self.dataName)
        fileNames = os.popen(lsCommand).read().split('\n')
        fileNames.pop()

        # Now sort them
        fileNames  = sortFileNames(fileNames) 

    # Otherwise we just go through and get the ID's we need
    else:
        for id in idList: 
            lsCommand = 'ls -1 %s%s-log-*%s.dat' % (self.baseDir,self.prefix,id)
            fileNames.append(os.popen(lsCommand).read().rstrip('\n'))

    return fileNames

# -----------------------------------------------------------------------------
def getSimulationParameters(self):
    '''Get the full list of parameter maps for each input file and a list of
        ID numbers. '''

    # Get the list of log files
    fileNames = self.getFileList("log")

    self.params = {}
    self.id = []
    for fname in fileNames:
        ID = self.getID(fname)
        self.id.append(ID)
        self.params[ID] = self.getParameterMap(fname)

# -----------------------------------------------------------------------------
def getHeadersFromFile(fileName):
    ''' Get the data column headers from a PIMC output file. '''

    inFile = open(fileName,'r');
    inLines = inFile.readlines();
    if inLines[0].find('PIMCID') != -1:
        headers = inLines[1].split()
    else:
        headers = inLines[0].split()
    headers.pop(0)
    inFile.close()
    return headers

# -------------------------------------------------------------------------------
def getFileString(options,reduce=True):
    ''' Using the command line flags, form the input file string that will
        be used to open all data files. '''

    # we simply go through all possible options and figure out what the
    # reduce variable is
    out = ""
    if options.T is not None:
        flagT = "%06.3f" % options.T
        out += '-T-%s' % flagT
    else:
        flagT = "*"

    if options.M is not None:
        flagM = "%04d" % options.M
        out += '-M-%s' % flagM
    else:
        flagM = "*"

    if reduce:
        outName = '%s-reduce%s' % (options.reduce,out)
        return dataName,outName
    else:
        return dataName

# ======================================================================
# ----------------------------------------------------------------------
# THIS SECTION WAS YANKED FROM REDUCE-ONE.PY
# ----------------------------------------------------------------------
def getStats(data,dim=0):
    ''' Get the average and error of all columns in the data matrix. '''

    if ndim(data) > dim:
        numBins  = size(data,dim) 
        dataAve  = average(data,dim) 
        dataAve2 = average(data*data,dim) 
        dataErr   = sqrt( abs(dataAve2-dataAve**2)/(1.0*numBins-1.0) ) 
    else:
        dataAve = data
        dataErr = 0.0*data

    return dataAve,dataErr

# -----------------------------------------------------------------------------
def getScalarEst(type,pimc,outName,reduceFlag):
    ''' Return the arrays containing the reduced averaged scalar
        estimators in question.'''

    fileNames = pimc.getFileList(type)
    headers   = pimchelp.getHeadersFromFile(fileNames[0])

    ave = zeros([len(fileNames),len(headers)],float)
    err = zeros([len(fileNames),len(headers)],float)
    for i,fname in enumerate(fileNames):
        # Compute the averages and error
        data = loadtxt(fname)
        ave[i,:],err[i,:] = getStats(data)
    
    # output the estimator data to disk
    outFile = open('%s-%s' % (type,outName),'w');

    # the headers
    outFile.write('#%15s' % reduceFlag[0])
    for head in headers:
        outFile.write('%16s%16s' % (head,'+/-'))
    outFile.write('\n')

    # the data
    for i,f in enumerate(fileNames):
        outFile.write('%16.8E' % float(pimc.params[pimc.id[i]][reduceFlag[1]]))
        for j,h in enumerate(headers):
            outFile.write('%16.8E%16.8E' % (ave[i,j],err[i,j]))
        outFile.write('\n')
    outFile.close()

    return headers,ave,err;

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# Begin Main Program 
# -----------------------------------------------------------------------------
def main(): 

    # define the mapping between short names and label names 
    shortFlags = ['T','M']
    parMap = {'T':'Temperature', 'M':'Imaginary Time Step'}

    # setup the command line parser options 
    parser = OptionParser() 
    parser.add_option("-T", "--temperature", dest="T", type="float",
                      help="simulation temperature in Kelvin") 
    parser.add_option("-M", "--imag-time-step", dest="M", type="float",
                      help="imaginary time step")
    parser.add_option("-r", "--reduce", dest="reduce",
                      choices=['T','M'], 
                      help="variable name for reduction [T,M]") 
    parser.add_option("-p", "--plot", action="store_true", dest="plot",
                      help="do we want to produce data plots?") 
    parser.set_defaults(plot=False)

    # parse the command line options and get the reduce flag
    (options, args) = parser.parse_args() 
    if len(args) > 0: 
        parser.error("incorrect number of arguments")
    
    if (not options.reduce):
        parser.error("need a correct reduce flag (-r,--reduce): [T,M]")

    dataName,outName = pimchelp.getFileString(options)
    reduceFlag = []
    reduceFlag.append(options.reduce)
    reduceFlag.append(parMap[options.reduce])

    # Create the PIMC analysis helper and fill up the simulation parameters maps
    # THIS LINE IS QUESTIONABLY USABLE!!!!!!!!!!!!
    #pimc = pimchelp.PimcHelp(dataName)

    
    getSimulationParameters()

    # Form the full output file name
    if options.R == None:
        outName += '.dat'
    else:
        outName += '-R-%04.1f.dat' % options.R

    # We first reduce the scalar estimators and output them to disk
    head1,scAve1,scErr1 = getScalarEst('estimator',pimc,outName,reduceFlag)

    # Do we show plots?
    if options.plot:

        # -----------------------------------------------------------------------------
        # Plot the averaged data
        # -----------------------------------------------------------------------------
    
        # Get the changing parameter that we are plotting against
        param = []
        for ID in pimc.id:
            param.append(float(pimc.params[ID][reduceFlag[1]]))

        numParams = len(param)
        markers = loadgmt.getMarkerList()
        headLab = ['E']
        dataCol = []
        for head in headLab:
            n = 0
            for h in head1:
                if head == h:
                    dataCol.append(n)
                    break
                n += 1
        yLabelCol = ['Thermodynamic Energy']

        colors  = loadgmt.getColorList('cw/1','cw1-029',max(numParams,len(headLab))+1)
    
        # ============================================================================
        # Figure -- Various thermodynamic quantities
        # ============================================================================
        figNum = 1
        for n in range(len(dataCol)):
            figure(figNum)
            connect('key_press_event',kevent.press)
    
            errorbar(param, scAve1[:,dataCol[n]], yerr=scErr1[:,dataCol[n]],\
                    color=colors[n],marker=markers[n],markeredgecolor=colors[n],\
                    markersize=8,linestyle='None',capsize=4)
    
            xlabel('%s'%options.reduce)
            ylabel(yLabelCol[n])
            tight_layout()
            #savefig('tba-energy.eps')
            figNum += 1
    
            show()

# ----------------------------------------------------------------------
# ----------------------------------------------------------------------
if __name__ == "__main__": 
    main()

