#include "common.h"
using namespace std;

#ifndef ESTIMATORS_H
#define ESTIMATORS_H

// estimators class declaration
class estimators{
    
    public:
        vector<double> instX(vector<double>, vector<double>, int);
        vector<double> instE(vector<double>, vector<double>, int, double,
                double, double, double);
        vector<double> aveX(vector<double>, vector<double>, int, double);
        vector<double> aveE(vector<double>, vector<double>, int, double);
        vector<double> stDevX(vector<double>, vector<double>,
                int, double);
        vector<double> stDevE(vector<double>, vector<double>,
                int, double);

};
#endif
