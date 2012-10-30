#include<vector>
using namespace std;
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Constants class declaration
class constants{
    private:
        int M;
    public:
        void setM(int);
        int sendM() const;
        vector<double> initPos(int);
        vector<double> steps(int);
        /*
        // number im.Time slices
        static const int M = 8 ;
        // temp
        static const double T = 5;
        // mass in amu
        static const double m = 4;
        // spring constant
        static const double k_s = 1;
        // num of MC steps to take
        static const int s = 1000;
        // scale physical step
        static const double rat = 1;
        // scale worldline shift
        static const double rat2 = 1;
        static const double hbar = 1;
        static const double k = 1;
        //r = MTRand(1);
        static double beta = 1/(k*T);
        static const double w = sqrt(k_s/m);
        static const double tau = beta/M;*/
};
#endif
