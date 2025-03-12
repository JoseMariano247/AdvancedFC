#pragma once

#include <string>

void MonteCarloPh(int initial_A, int initial_Fv, int initial_Af, double k1, double k2, 
                     double t_stop, const std::string& outputFilename);

