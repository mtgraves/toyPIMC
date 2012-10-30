#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
#include "../PIMC/MersenneTwister.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;


// ==================================================================
// Begin Function Main
// ==================================================================
int main(int argc, char** argv){
 
    /* initialize variables to be parsed from cmd line */
    int M;
    double T;
    double m;
    int s;

    /* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
    /* PARSE the cmd line options */
    po::options_description desc("\n~~~~~~~~~Supported Options for QHO PIMC~~~~~~~~~\n");
    desc.add_options()
        ("help,h","..you're always asking for help")
        ("slices,M", po::value<int>(&M),"number of imaginary time slices")
        ("temp,T", po::value<double>(&T),"Temperature in Kelvin")
        ("mass,m", po::value<double>(&m),"mass in AMU")
        ("sweeps,s", po::value<int>(&s),"number of Monte Carlo sweeps")
        ;
    po::variables_map vm;
    po::store(
            po::parse_command_line(
                argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")){
        cout << desc << endl;
        return false;
    }

    /* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
    /* define physical constants */
    double hbar = 1;
    double k = 1;       // Boltzmann constant
    double beta = 1/(k*T);
    double w = 1;
    double tau = beta/M;

    typedef unsigned long uint32;
    /* define seed for RNG */
    uint32 seed = 139853;       //139853 originally
    /* instantiate MTRand class */
    MTRand randy(seed);

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
    double bin=0;
    int observableSkip = 50;
    
    /* populate array with M randyly placed beads */
    for (int i=0; i<M; i++){
        double r = 2*randy.rand()-1;
        aa.push_back(r);
        steps.push_back(i);
    }

    /************ start MC sweeps ********************************************/
    double accept = 0.0;
    double reject = 0.0;
    for (int z=0; z<s; z++){
        //cout<< "============MC SWEEP NUMBER "<<z+1<<"============"<<endl;
        bb = aa;
        double r = randy.rand();
        if (r <= 0.25){
            double R1 = (2*randy.rand()-1);
            for (int i=0; i<M; i++){
                aa[i] = aa[i] + R1;
            }
        }
        else{
            int R = randy.randInt(M-1);
            aa[R] = aa[R] + (2*randy.rand()-1);
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
        /*cout<< "S_k_diff: "<<S_k_diff<<endl;
        cout<< "present kinetic action: "<<S_kpr<<endl;
        cout<< "S_V_diff: "<<S_V_diff<<endl;
        if (z == s-1){
            double finalAction = S_kpr + S_prV;
            cout<<"final action: "<<finalAction<<endl;
        }*/
        
        /************* Metropolis Algorithm **********************************/
        if (rho >= 1){
            accept += 1;
            aa = aa;
        }
        else{
            r = randy.rand();
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
        /* ONE WAY TO DO IT */
        if ((z>1000) && (z%50 == 0)){
            bin += 1;
            double xbar_i = 0;
            double xbar = 0;
            double Ebar_i = 0.5/tau;
            double Ebar = 0;
            double sumE = 0;
            double sumX = 0;
            for (int j=0; j<M; j++){
                xbar_i += aa[j]/M;
                if (j==0){
                    Ebar_i += ((-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[M-1]),2)))
                            + (((m*w*w)/(2*M))*pow((aa[j]),2)));
                }
                else{
                    Ebar_i += (-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[j-1]),2))
                            + ((m*w*w)/(2*M))*(pow((aa[j]),2)));
                }
            }
            instX.push_back(xbar_i);
            instE.push_back(Ebar_i);
            for(int j=0; j<bin; j++){
                xbar += instX[j]/bin;
                Ebar += instE[j]/bin;
            }
            aveX.push_back(xbar);
            aveE.push_back(Ebar);
            // standard deviations 
            for (int j=0; j<bin; j++){
                sumX += aveX[j];
                sumE += aveE[j];
            }
            stDevX.push_back(sqrt((pow(sumX,2)/bin) - pow((sumX/bin),2)));
            stDevE.push_back(sqrt((pow(sumE,2)/bin) - pow((sumE/bin),2)));
        }

        /*************************************************************/
        /* THE OTHER WAY TO DO IT 
        double xbar_i = 0;
        double xbar = 0;
        double Ebar_i = 0.5/tau;
        double Ebar = 0;
        double sumE = 0;
        double sumX = 0;
        // calculate instantaneous estimator values 
        for (int j=0; j<M; j++){
            xbar_i += aa[j]/M;
            if (j==0){
                Ebar_i += ((-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[M-1]),2)))
                        + (((m*w*w)/(2*M))*pow((aa[j]),2)));
            }
            else{
                Ebar_i += (-(m/(2*M*tau*tau*hbar*hbar))*(pow((aa[j]-aa[j-1]),2))
                        + ((m*w*w)/(2*M))*(pow((aa[j]),2)));
            }
        }
        // add these values to a temporary vector 
        instX.push_back(xbar_i);
        instE.push_back(Ebar_i);
        // after *observableSkip* loops, calculate the average 
        // of the vector we have built up 
        if ((z!=0) && (z%observableSkip == 0)){
            for(int j=0; j<observableSkip; j++){
                xbar += instX[j]/(observableSkip);
                Ebar += instE[j]/(observableSkip);
                cout << "binned data on MC step: "<<z<<endl;
            }
            // clear the temporary vectors 
            instX.clear();
            instE.clear();
            cout<< "and the Ebar value was: "<<Ebar<<endl;
            bin += 1;
            // add values to new vector, actual data to be plotted 
            aveX.push_back(xbar);
            aveE.push_back(Ebar);
        
        }
        // standard deviations 
        for (int j=0; j<bin; j++){
            sumX += aveX[j];
            sumE += aveE[j];
        }
        stDevX.push_back(sqrt((pow(sumX,2)/bin) - pow((sumX/bin),2)));
        stDevE.push_back(sqrt((pow(sumE,2)/bin) - pow((sumE/bin),2)));

        */
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
