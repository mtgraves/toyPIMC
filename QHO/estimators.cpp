// implementation file for estimators class
#include "estimators.h"
#include<iostream>
#include<cstdlib>
#include<vector>
#include "../../PIMC/MersenneTwister.h"
using namespace std;

// =======================================================================
// POSITION
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array of the instantaneous x average per bin
vector<double> estimators::instX(vector<double> instX, vector<double> aa,
        int M){
    
    double xbar_i=0;
    for (int j=0; j<M; j++){
        xbar_i += aa[j]/M;
    }
    instX.push_back(xbar_i);
    
    return instX;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array of running x averages
vector<double> estimators::aveX(vector<double> instX, vector<double> aveX,
        int M, double bin){
    
    double xbar = 0;
    for(int j=0; j<bin; j++){
        xbar += instX[j]/bin;
    }
    aveX.push_back(xbar);
    
    return aveX;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array for the standard error for x
vector<double> estimators::stDevX(vector<double> stDevX,
        vector<double> aveX, int M, double bin){
    
    double sumX=0;
    for (int j=0; j<bin; j++){
        sumX += aveX[j];
    }
    stDevX.push_back(sqrt((pow(sumX,2)/bin) - pow((sumX/bin),2)));
    
    return stDevX;
}

// =======================================================================
// ENERGY
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array of the instantaneous E average per bin
vector<double> estimators::instE(vector<double> instE, vector<double> aa,
        int M, double tau, double hbar, double w, double m){

    double Ebar_i = 0.5/tau;
    for (int j=0; j<M; j++){
        if (j==0){
            Ebar_i += ((-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[M-1]),2)))
                    + (((m*w*w)/(2*M))*pow((aa[j]),2)));
        }
        else{
            Ebar_i += (-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[j-1]),2))
                    + (((m*w*w)/(2*M))*pow((aa[j]),2)));
        }
    }
    instE.push_back(Ebar_i);
    return instE;
}   

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array of running E averages
vector<double> estimators::aveE(vector<double> instE, vector<double> aveE, 
        int M, double bin){
    
    double Ebar=0; 
    for(int j=0; j<bin; j++){
        Ebar += instE[j]/bin;
    }
    aveE.push_back(Ebar);
    return aveE;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// return an array of the standard error for E
vector<double> estimators::stDevE(vector<double> stDevE,
        vector<double> aveE, int M, double bin){
    
    double sumE = 0;
    for (int j=0; j<bin; j++){
        sumE += aveE[j];
    }
    stDevE.push_back(sqrt((pow(sumE,2)/bin) - pow((sumE/bin),2)));
    
    return stDevE;
}
