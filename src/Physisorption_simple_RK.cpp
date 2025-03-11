#include "Physisorption_simple_RK.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

// Calculate derivatives for the system:
// A + Fv ->(k1) Af
// Af ->(k2) A + Fv
static void derivatives3(double k1, double k2,
                        double A, double Fv, double Af,
                        double& dAdt, double& dFvdt, double& dAfdt)
{
    dAdt  = -k1 * A * Fv + k2 * Af;
    dFvdt = -k1 * A * Fv + k2 * Af;
    dAfdt =  k1 * A * Fv - k2 * Af;
}

// Perform one RK4 step
static void rk4Step3(double k1, double k2,
                    double& A, double& Fv, double& Af,
                    double& t, double dt)
{
    double dA1, dFv1, dAf1;
    double dA2, dFv2, dAf2;
    double dA3, dFv3, dAf3;
    double dA4, dFv4, dAf4;

    // k1
    derivatives3(k1, k2, A, Fv, Af, dA1, dFv1, dAf1);

    // k2
    derivatives3(k1, k2,
                A  + 0.5 * dt * dA1,
                Fv + 0.5 * dt * dFv1,
                Af + 0.5 * dt * dAf1,
                dA2, dFv2, dAf2);

    // k3
    derivatives3(k1, k2,
                A  + 0.5 * dt * dA2,
                Fv + 0.5 * dt * dFv2,
                Af + 0.5 * dt * dAf2,
                dA3, dFv3, dAf3);

    // k4
    derivatives3(k1, k2,
                A  + dt * dA3,
                Fv + dt * dFv3,
                Af + dt * dAf3,
                dA4, dFv4, dAf4);

    // Combine
    A  += (dt / 6.0) * (dA1  + 2.0 * dA2  + 2.0 * dA3  + dA4);
    Fv += (dt / 6.0) * (dFv1 + 2.0 * dFv2 + 2.0 * dFv3 + dFv4);
    Af += (dt / 6.0) * (dAf1 + 2.0 * dAf2 + 2.0 * dAf3 + dAf4);

    t  += dt;
}

// Main driver function
void RungeKuttaFS(double A, double Fv, double Af,
                  double k1, double k2,
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
    outFile << "Time\tA\tFv\tAf\n";

    // Time-stepping loop
    while (t < tMax)
    {
        // Print to console (optional)
        std::cout << t << "\t" << A << "\t" << Fv << "\t" << Af << "\n";
        // Write to file
        outFile << t << "\t" << A << "\t" << Fv << "\t" << Af << "\n";

        // Advance one RK4 step
        rk4Step3(k1, k2, A, Fv, Af, t, dt);
    }

    outFile.close();
}
