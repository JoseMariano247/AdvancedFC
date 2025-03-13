#include "Recombination_RK.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

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
static void derivatives6(double r1, double r2, double r3, double r4,
                         double r5, double r6, double r7,
                         double A,  double Fv, double Af,
                         double Sv, double As, double A2,
                         double& dAdt,  double& dFvdt, double& dAfdt,
                         double& dSvdt, double& dAsdt, double& dA2dt)
{
    // Reaction rates (mass-action kinetics)
    double R1 = r1 * A * Fv;
    double R2 = r2 * Af;
    double R3 = r3 * A * Sv;
    double R4 = r4 * A * As;
    double R5 = r5 * Af * Sv;
    double R6 = r6 * Af * As;
    double R7 = r7 * Af * Af;  // Reaction 7: 2 Af -> A2 + 2Fv

    // Derivatives
    dAdt  = -R1 + R2 - R3 - R4;
    // For Fv, note that reaction 7 now produces 2 Fv molecules:
    dFvdt = -R1 + R2 + R5 + R6 + 2.0 * R7;
    dAfdt =  R1 - R2 - R5 - R6 - 2.0 * R7;
    dSvdt = -R3 + R4 - R5 + R6;
    dAsdt =  R3 - R4 + R5 - R6;
    // A2 is produced in reactions 4, 6, and 7 (one molecule per occurrence)
    dA2dt =  R4 + R6 + R7;
}

// -----------------------------------------------------------------------------
// One RK4 step for six variables
// -----------------------------------------------------------------------------
static void rk4Step6(double r1, double r2, double r3, double r4,
                     double r5, double r6, double r7,
                     double& A,  double& Fv, double& Af,
                     double& Sv, double& As, double& A2,
                     double& t,  double dt)
{
    // k1
    double dA1, dFv1, dAf1, dSv1, dAs1, dA21;
    derivatives6(r1, r2, r3, r4, r5, r6, r7,
                 A, Fv, Af, Sv, As, A2,
                 dA1, dFv1, dAf1, dSv1, dAs1, dA21);

    // k2
    double dA2_, dFv2_, dAf2_, dSv2_, dAs2_, dA22_;
    derivatives6(r1, r2, r3, r4, r5, r6, r7,
                 A  + 0.5 * dt * dA1,
                 Fv + 0.5 * dt * dFv1,
                 Af + 0.5 * dt * dAf1,
                 Sv + 0.5 * dt * dSv1,
                 As + 0.5 * dt * dAs1,
                 A2 + 0.5 * dt * dA21,
                 dA2_, dFv2_, dAf2_, dSv2_, dAs2_, dA22_);

    // k3
    double dA3, dFv3, dAf3, dSv3, dAs3, dA23;
    derivatives6(r1, r2, r3, r4, r5, r6, r7,
                 A  + 0.5 * dt * dA2_,
                 Fv + 0.5 * dt * dFv2_,
                 Af + 0.5 * dt * dAf2_,
                 Sv + 0.5 * dt * dSv2_,
                 As + 0.5 * dt * dAs2_,
                 A2 + 0.5 * dt * dA22_,
                 dA3, dFv3, dAf3, dSv3, dAs3, dA23);

    // k4
    double dA4, dFv4, dAf4, dSv4, dAs4, dA24;
    derivatives6(r1, r2, r3, r4, r5, r6, r7,
                 A  + dt * dA3,
                 Fv + dt * dFv3,
                 Af + dt * dAf3,
                 Sv + dt * dSv3,
                 As + dt * dAs3,
                 A2 + dt * dA23,
                 dA4, dFv4, dAf4, dSv4, dAs4, dA24);

    // Combine increments
    A  += (dt / 6.0) * (dA1  + 2.0 * dA2_  + 2.0 * dA3  + dA4);
    Fv += (dt / 6.0) * (dFv1 + 2.0 * dFv2_ + 2.0 * dFv3 + dFv4);
    Af += (dt / 6.0) * (dAf1 + 2.0 * dAf2_ + 2.0 * dAf3 + dAf4);
    Sv += (dt / 6.0) * (dSv1 + 2.0 * dSv2_ + 2.0 * dSv3 + dSv4);
    As += (dt / 6.0) * (dAs1 + 2.0 * dAs2_ + 2.0 * dAs3 + dAs4);
    A2 += (dt / 6.0) * (dA21 + 2.0 * dA22_ + 2.0 * dA23 + dA24);

    // Advance time
    t += dt;
}

// -----------------------------------------------------------------------------
// Main driver for the six-species system
// -----------------------------------------------------------------------------
void RungeKuttaRecombination(double A,  double Fv, double Af,
                    double Sv, double As, double A2,
                    double r1, double r2, double r3,
                    double r4, double r5, double r6,
                    double r7,
                    double dt, double tMax,
                    const std::string& outputFilename)
{
    double t = 0.0;

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }

    // Write header
    outFile << "Time\tA\tFv\tAf\tSv\tAs\tA2\n";

    // Time-stepping loop
    while (t < tMax)
    {
        // Optional: Print to console
        std::cout << t << "\t"
                  << A  << "\t" << Fv << "\t" << Af << "\t"
                  << Sv << "\t" << As << "\t" << A2 << "\n";

        // Write to file
        outFile << t << "\t"
                << A  << "\t" << Fv << "\t" << Af << "\t"
                << Sv << "\t" << As << "\t" << A2 << "\n";

        // Advance one RK4 step
        rk4Step6(r1, r2, r3, r4, r5, r6, r7,
                 A, Fv, Af, Sv, As, A2, t, dt);
    }

    outFile.close();
}
