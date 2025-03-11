#pragma once

#include <string>

void RungeKuttaFS(double A, double Fv, double Af, 
    double k1, double k2, double dt, 
    double tMax, const std::string& outputFilename);


