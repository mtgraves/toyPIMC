from numpy import *
import numpy as np

#dataIn = loadtxt('qho-1.7-100000-10.dat')
T = 0.2

while (T < 0.9):
    x,y = loadtxt('qho-%s-100000-10.dat' % T, unpack=True, usecols=[0,3])
    T += 0.1

energy = np.array([])

for i in y:
    energy = np.append(energy,i)

for i in energy:
    print i

print 'average energy is: ',np.average(energy)
