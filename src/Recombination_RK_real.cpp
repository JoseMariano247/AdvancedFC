#include "Recombination_RK_real.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#ifndef pi
#define pi 3.14159265358979323846
#endif

// -----------------------------------------------------------------------------
// Derivatives for the six-species system with seven reactions:
//
//  1) A + Fv   ->(r1)  Af
//  2) Af       ->(r2)  A + Fv
//  3) A + Sv   ->(r3)  As
//  4) A + As   ->(r4)  A2 + Sv
//  5) Af + Sv  ->(r5)  Fv + As
//  6) Af + As  ->(r6)  A2 + Sv + Fv
//  7) Af + Af  ->(r7)  A2 + 2Fv
//
// Species: A, Fv, Af, Sv, As, A2
// -----------------------------------------------------------------------------

static void derivatives6real(double r1, double r2, double r3, double r4,
    double r5, double r6, double r7,
    double thetaF, double thetaS,
    double Sv, double Fv, double& dFdt, double& dSdt)
{
    // Separate reactions affecting Fv and Sv to ensure different growth for thetaF and thetaS
    double term1_Fv = (1 - thetaF) * r1 - thetaF * r2;  // Reaction for Fv
    double term2_Fv = thetaF * (1 - thetaS) * Sv / Fv * r5; // Interaction of Fv and Sv for Fv
    double term3_Fv = thetaF * thetaS * Sv / Fv * r6;     // More interaction for Fv

    double term1_Sv = (1 - thetaS) * r3 - thetaS * r4;    // Reaction for Sv
    double term2_Sv = thetaF * (1 - thetaS) * r5;         // Interaction for Sv
    double term3_Sv = thetaF * thetaS * r6;               // More interaction for Sv

    // Define the change rates based on separate terms for Fv and Sv
    dFdt = term1_Fv - term2_Fv - term3_Fv - 2 * thetaF * thetaF * r7;  // Modify the interaction terms
    dSdt = term1_Sv + term2_Sv - term3_Sv; // Modify the interaction terms for Sv
}

// -----------------------------------------------------------------------------
// One RK4 step for six variables
// -----------------------------------------------------------------------------
static void rk4Step6real(double r1, double r2, double r3, double r4,
                         double r5, double r6, double r7,
                         double& thetaF, double& thetaS,
                         double Sv, double Fv, double& t,  double dt)
{
    // k1
    double dF1, dS1;
    derivatives6real(r1, r2, r3, r4, r5, r6, r7, 
                     thetaF, thetaS, Fv, Sv, dF1, dS1);

    // k2
    double dF2, dS2;
    derivatives6real(r1, r2, r3, r4, r5, r6, r7,
                    thetaF  + 0.5 * dt * dF1,
                    thetaS + 0.5 * dt * dS1,
                    Fv, Sv, dF2, dS2);

    // k3
    double dF3, dS3;
    derivatives6real(r1, r2, r3, r4, r5, r6, r7,
                     thetaF  + 0.5 * dt * dF2,
                     thetaS + 0.5 * dt * dS2,
                     Fv, Sv, dF3, dS3);

    // k4
    double dF4, dS4;
    derivatives6real(r1, r2, r3, r4, r5, r6, r7,
                     thetaF  + dt * dF3,
                     thetaS +  dt * dS3,
                     Fv, Sv, dF4, dS4);

    // Combine increments
    thetaF  += (dt / 6.0) * (dF1  + 2.0 * dF2  + 2.0 * dF3  + dF4);
    thetaS += (dt / 6.0) * (dS1 + 2.0 * dS2 + 2.0 * dS3 + dS4);
    
    // Advance time
    t += dt;
}

// -----------------------------------------------------------------------------
// Main driver for the six-species system
// -----------------------------------------------------------------------------
void RungeKuttaRecombinationreal(double A,  double Fv,
                    double Sv, double A2,
                    double M, double Tg, double Tw,
                    double k1, double k3, double k4, double vd,
                    double vD, double Ed, double ED, double Er, double ELHF,
                    double tMax,
                    const std::string& outputFilename)
{
    double t = 0.0;

    double kb = 1.380649e-23;
    double Na = 6.023e23;

    double v_med = std::sqrt((8*kb*Tg*Na)/(pi * M));

    double phi_O = 0.25 * v_med * A;
    double Pr = k4 * std::exp(-Er/(Na*kb*Tw));
    double Prlh = k4 * std::exp(-ELHF/(Na*kb*Tw));
    double tau_d_1 = vD * std::exp(-ED/(Na*kb*Tw));
    double dt = 1e-7;
    std::cout << tau_d_1 << "\n";

    //calculate reaction coefficents

    double r1 = k1 * phi_O/(Fv + Sv);
    double r2 = vd * std::exp(-Ed/(Na*kb*Tw));
    double r3 = k3 * phi_O/(Fv + Sv);
    double r4 = Pr * r3;
    double r5 = 0.75*tau_d_1;
    double r6 = tau_d_1 * Pr;
    double r7 = tau_d_1 * Prlh;

    double Af = 0.0;
    double As = 0.0;
    double thetaF = 0.0;
    double thetaS = 0.0;

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }

    // Write header
    outFile << "Time\tthetaF\tthetaS\n";

    // Time-stepping loop
    while (t < tMax)
    {
        // Optional: Print to console
        std::cout << t << "\t" << thetaF  << "\t" << thetaS << "\n";

        // Write to file
        outFile << t << "\t" << thetaF  << "\t" << thetaS << "\n";

        // Advance one RK4 step
        rk4Step6real(r1, r2, r3, r4, r5, r6, r7,
                     thetaF, thetaS, Sv, Fv, t, dt);
    }

    outFile.close();
}