#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
//#include "constants.h"
#include "estimators.h"
#include "../../PIMC/MersenneTwister.h"
using namespace std;

// ==================================================================
// Define important values
// ==================================================================
int M = 10;
// temp
double T = 0.2;
// mass in amu
double m = 1.0;
// num of MC steps to take
int s = 100000;

double hbar = 1;
double k = 1;       // Boltzmann constant
double beta = 1/(k*T);
double w = 1;
double tau = beta/M;
typedef unsigned long uint32;

/* define seed for RNG */
uint32 seed = 139853;       //139853 originally
/* instantiate MTRand class */
MTRand random(seed);

// ==================================================================
// Begin Function Main
// ==================================================================
int main(){
    // instantiate classes
    //constants con;
    estimators estim;
    // number im.Time slices
    /*int dirka = 10;
    con.setM(dirka);
    int M = con.sendM();
    double tau = beta/M;
    */
    /* positions of beads */
    vector<double> aa;
    /* temporary positions */
    vector<double> bb;
    /* imaginary time steps */
    vector<int> steps;
    /* average instantaneous position <x_i> */
    vector<double> instX;
    /* cumulative average position <x> */
    vector<double> aveX;
    /* average inst. Thermo energy <E_i> */
    vector<double> instE;
    /* cumulative average thermo energy <E> */
    vector<double> aveE;
    /* std deviations */
    vector<double> stDevX;
    vector<double> stDevE;
    
    /* keep track of measurements binned */
    int obsSkip = 50;
    double bin = 0;

    /* populate array with M randomly placed beads */
    for (int i=0; i<M; i++){
        double r = 2*random.rand()-1;
        aa.push_back(r);
        steps.push_back(i);
    }

    /* initialize vector of initial positions */
    //vector<double> aa = con.initPos(M);
    //vector<double> steps = con.steps(M);

    /************ start MC sweeps ********************************************/
    double accept = 0.0;
    double reject = 0.0;
    for (int z=0; z<s; z++){
        //cout<< "============MC SWEEP NUMBER "<<z+1<<"============"<<endl;
        bb = aa;
        double r = random.rand();
        if (r <= 0.25){
            double R1 = (2*random.rand()-1);
            double R = 0;
            for (int i=0; i<M; i++){
                aa[i] = aa[i] + R1;
            }
        }
        else{
            int R = random.randInt(M-1);
            aa[R] = aa[R] + (2*random.rand()-1);
        }

        /****** Calculate Primitive Action ***********************************/
        double S_prV = 0;
        double S_paV = 0;
        double S_kpr = 0;
        double S_kpa = 0;
        for (int i=0; i<M; i++){
            /** present potential */
            S_prV += 0.5*tau*m*w*w*pow(aa[i],2);
            /** past potential */
            S_paV += 0.5*tau*m*w*w*pow(bb[i],2);
            /** past and present kinetic */
            if (i == 0){
                S_kpr += m/(2*tau*hbar*hbar)*pow((aa[i]-aa[M-1]), 2);
                S_kpa += m/(2*tau*hbar*hbar)*pow((bb[i]-bb[M-1]), 2);
            }
            else{
                S_kpr += m/(2*tau*hbar*hbar)*pow((aa[i]-aa[i-1]), 2);
                S_kpa += m/(2*tau*hbar*hbar)*pow((bb[i]-bb[i-1]), 2);
            }
        }
        double S_present = S_kpr + S_prV;
        double S_past = S_kpa + S_paV;
        double rho = exp(-(S_present - S_past));
        
        /************* Metropolis Algorithm **********************************/
        if (rho >= 1){
            accept += 1;
            aa = aa;
        }
        else{
            r = random.rand();
            if (r <= rho){
                aa = aa;
                accept += 1;
            }
            else{
                aa = bb;
                reject += 1;
            }
        }
        /********* Estimators ****************************************/
        /* <x>  and <E> */
        /*************************************************************/
        if ((z>1000) && (z%obsSkip == 0)){
            bin += 1;
            
            instX = estim.instX(instX, aa, M);
            aveX = estim.aveX(instX, aveX, M, bin);
            stDevX = estim.stDevX(stDevX, aveX, M, bin);
            
            instE = estim.instE(instE, aa, M, tau, hbar, w, m);
            aveE = estim.aveE(instE, aveE, M, bin);
            stDevE = estim.stDevE(stDevE, aveE, M, bin);
        }
    }

    double acceptanceRatio = accept/(accept+reject);   
    cout<< "==================================="<<endl;
    cout<< "acceptance ratio: " <<acceptanceRatio <<endl;
    cout<< "==================================="<<endl;
    
    
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // DATA HANDLING
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    ofstream outputFile;

    outputFile.open("./output/output.dat");
    outputFile << "#" << "\t" << "<x>"  << "\t"<< "stDevX" << "\t" << "<E>" << "\t" << "stDevE" << endl;
    for (int i=0; i<bin; i++){
        outputFile << i << "\t" << aveX[i]  << "\t"<< stDevX[i] << "\t" << aveE[i]  << "\t"<< stDevE[i] << endl;
    }
    outputFile.close();

    return 0;
}
