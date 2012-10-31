/**
 * @file setup.cpp
 * @author Max Graves
 * @date 25-OCT-2012
 *
 * @brief Define Class to set up the simulation
 * @details ...in future, use details description!
 */

#include "setup.h"

Setup::Setup():
    vm(),
    physOptions("Physical Parameters"),
    VOptions("Potential Options")
{
    // Interator to move through external potentials (Credit: Adrian Del Maestro)
    vector<string>::iterator it;

    externalPotentialChoices.push_back("harm_osc");
    externalPotentialChoices.push_back("double_well");

    externalPotentials="";
    for (it=externalPotentialChoices.begin(); it!=externalPotentialChoices.end(); ++it)
        externalPotentials += *it +",";
    externalPotentials.erase(externalPotentials.end()-1);
}


void Setup::cmdLineOptions(int argc, char *argv[]){
    physOptions.add_options()
        ("help,h","...you're always asking for help")
        ("slices,M", po::value<int>(&M),"HALF the number of imaginary time slices")
        ("temp,T", po::value<double>(&T),"Temperature in Kelvin")
        ("mass,m", po::value<double>(&m)->default_value(1) ,"mass in AMU")
        ("sweeps,s", po::value<int>(&s),"number of Monte Carlo sweeps")
        ;
    VOptions.add_options()
        ("ext_potential,V", po::value<string>(&V)->default_value("harm_osc"),
         "Choice of External Potential")
        ;

    allOptions.add(physOptions).add(VOptions);

    po::store(
            po::parse_command_line(
                argc, argv, allOptions), vm);
    po::notify(vm); 
}

bool Setup::parseOptions(){
    
    if (vm.count("help")){
        cout << allOptions << endl;
        return false;
    }

    bool acceptableInput = false;
    vector<string>::iterator it;
    for (it=externalPotentialChoices.begin(); it != externalPotentialChoices.end(); ++it) {
        if (vm["ext_potential"].as<string>() == *it) {
            acceptableInput = true;
            break;
        }
    }
    if (!acceptableInput) {
        cerr << "\nThe potential you input is not available." << endl;
        cerr << "\nYou must choose one of the following:" << endl
             << "\t[" << externalPotentials << "]" << endl;
        return true;
    }
    // catch at the end
    return false;
}
