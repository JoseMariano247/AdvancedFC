#pragma once

#include <string>

void RungeKuttaCh(double A, double Sv, double As, double A2, double k1, double k2, 
                     double dt, double tMax, const std::string& outputFilename);

