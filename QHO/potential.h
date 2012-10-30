#ifndef POTENTIAL_H
#define POTENTIAL_H

class Potential{
    private:
        double V;
    public:
        // constructor
        Potential(){
            V = 0;
        }
        // Mutator
        void setPotential(double w, double m, int M, double<vector> aa){
            for (int i=0; i<M; i++){
                V_QHO += 0.5*m*w*w*pow(aa[i],2)/M;
            }
            V = V_QHO;
        }
        // Accessor
        double getPotential() const{
            return V;
        }
};
#endif
