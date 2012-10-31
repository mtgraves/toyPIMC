/** 
 * @file constants.cpp
 * @author Max Graves
 * @date 25-OCT-2012
 *
 * @brief Implementation of Constants Class.
 */

#include "constants.h"

// ==================================================================
// CONSTANTS CLASS
// ==================================================================

ConstantParameters::ConstantParameters() : T_(), m_(), tau_(), e_(),
    w_(), M_(), s_(), V_(){
        //empty constructor
}

// Initialize constants.
void ConstantParameters::initConstants(double _T, double _m, 
        double _tau, double _e, double _w, int _M, int _s, 
        string _V) {

	// assign values
	T_ = _T;
    m_ = _m;
	tau_ = 1.0;
    w_ = 1.0;
    e_ = _tau/(2.0*_M);
    M_ = _M;
	V_ = _V;
    s_ = _s;
    
}

// ==================================================================
// Return an instance of the Constant object
// ==================================================================
ConstantParameters* ConstantParameters::getInstance ()
{   
	static ConstantParameters inst;
	return &inst;
}
