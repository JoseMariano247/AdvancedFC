#pragma once

#include <string>

void RungeKuttaRecombination(double A,  double Fv, double Af,
                    double Sv, double As, double A2,
                    double r1, double r2, double r3,
                    double r4, double r5, double r6,
                    double r7,
                    double dt, double tMax,
                    const std::string& outputFilename);