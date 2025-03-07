#pragma once

#include <string>

void RungeKutta(double A, double B, double k1, double k2, 
                     double dt, double tMax, const std::string& outputFilename);

