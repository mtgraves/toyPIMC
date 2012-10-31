/**
 * @file constants.h
 * @author Max Graves
 * @date 25-OCT-2012
 *
 * @brief Define Constants of Simulation.
 */

#ifndef CONSTANTS_H 
#define CONSTANTS_H

#include "common.h"

// ========================================================================  
// Constants Class
// ========================================================================  
/** 
 * Constant simulation parameters.
 */
class ConstantParameters
{
    public:
        static ConstantParameters* getInstance();
        void initConstants(double,double,double,double,double,int,int,string);
        
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // Accessors
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        /* Physical constants */
        double T() const {return T_;}       ///< Temperature (K)
        double e() const {return e_;}        ///< Imaginary time step size
        double tau() const {return tau_;}   ///< total imaginary time
        double m() const {return m_;}            ///< Mass of particle
        double w() const {return w_;}       ///< frequency related to kinetic links
        int M() {return M_;}    ///< Half the num. of imaginary time slices
        int s() const {return s_;}      ///< total number of MC sweeps to perform
        
        string V() const {return V_;} ///< Get External Potential

    protected:
        ConstantParameters();
        ConstantParameters(const ConstantParameters&);              ///< Protected constructor
        ConstantParameters& operator= (const ConstantParameters&);  ///< Overload Singleton equals

    private:
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // Mutators
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        double T_;          // Temp. (K)
        double e_;            // Imaginary time step size
        double tau_;
        double m_;              // Mass of Particle
        double w_;
        int M_;       // Number of imaginary time slices
        int s_;

        string V_; // The type of external potential 
};

/**************************************************************************//**
 * Global public access to the constants via Singleton pattern.
******************************************************************************/
inline ConstantParameters* constants() {
    ConstantParameters *temp = ConstantParameters::getInstance();
    return temp;
}

#endif
