#pragma once

#include <string>

void solveRungeKutta(double A, double B, double k1, double k2, 
                     double dt, double tMax, const std::string& outputFilename);

