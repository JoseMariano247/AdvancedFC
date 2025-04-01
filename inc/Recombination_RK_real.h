#pragma once

#include <string>

void RungeKuttaRecombinationreal(double A,  double Fv,
    double Sv, double A2, double M, double Tg, double Tw,
    double k1, double k3, double k4, double vd,
    double vD, double Ed, double ED, double Er, double ELHF,
    double tMax,
    const std::string& outputFilename);