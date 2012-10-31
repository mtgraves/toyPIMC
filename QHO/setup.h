/**
 * @file setup.h
 * @author Max Graves
 * @date 25-OCT-2012
 *
 * @brief Define Class to set up the simulation
 * @details ...in future, use details description!
 */

#ifndef SETUP_H 
#define SETUP_H

#include<boost/program_options.hpp>
namespace po=boost::program_options;

class Setup {
    public:
        Setup();

        // set up cmd line options
        void cmdLineOptions(int,char*[]);
        bool parseOptions();

        po::variables_map vm;           ///< variable map from cmd line

    private:
        po::options_description physOptions;    ///< Physical Parameters
        po::options_description VOptions;       ///< Potential Options
        po::options_description allOptions;     ///< All Command line Options

        vector<string> externalPotentialChoices;    ///< array of external potentials
        string externalPotentials;          ///< string of external potentials
};
#endif
