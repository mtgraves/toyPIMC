// implementation file for constants class
#include "constants.h"
#include<iostream>
#include<cstdlib>
#include<vector>
#include "../../PIMC/MersenneTwister.h"
using namespace std;

// assign value to M
void constants::setM(int steps){
    M = steps;
}
// return value to M
int constants::sendM() const{
    return M;
}
vector<double> constants::initPos(int M) {
    vector<double> aa;
    /* define seed for RNG */
    unsigned long seed = 189853;       //139853 originally
    /* instantiate MTRand class */
    MTRand random(seed);
    /* populate array with M randomly placed beads */
    for (int i=0; i<M; i++){
        double r = 2*random.rand()-1;
        aa.push_back(r);
    }
    return aa;
}
vector<double> constants::steps(int M) {
    vector<double> steps;
    for (int i=0; i<M; i++){
        steps.push_back(i);
    }
    return steps;
}
