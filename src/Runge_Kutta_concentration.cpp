#include "Runge_Kutta_concentration.h"
#include <iostream>
#include <cmath>
#include <fstream>

static void derivatives(double k1, double k2, double A, double B, 
                        double& dAdt, double& dBdt)
{
    dAdt = -k1 * A + k2 * B;
    dBdt = -k2 * B + k1 * A;
}

static void rk4Step(double& k1, double& k2, double& A, double& B, double& t, double dt)
{
    double dA1, dB1, dA2, dB2, dA3, dB3, dA4, dB4;

    // k1
    derivatives(k1, k2, A, B, dA1, dB1);
    // k2
    derivatives(k1, k2, A + 0.5 * dt * dA1, B + 0.5 * dt * dB1, dA2, dB2);
    // k3
    derivatives(k1, k2, A + 0.5 * dt * dA2, B + 0.5 * dt * dB2, dA3, dB3);
    // k4
    derivatives(k1, k2, A + dt * dA3, B + dt * dB3, dA4, dB4);

    A += (dt / 6.0) * (dA1 + 2.0 * dA2 + 2.0 * dA3 + dA4);
    B += (dt / 6.0) * (dB1 + 2.0 * dB2 + 2.0 * dB3 + dB4);
    t += dt;
}

void RungeKutta(double A, double B, double k1, double k2, 
                     double dt, double tMax, const std::string& outputFilename)
{
    double t = 0.0;

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }

    outFile << "Time\tA\tB\n";

    while (t < tMax)
    {
        std::cout << t << "\t" << A << "\t" << B << "\n";
        outFile << t << "\t" << A << "\t" << B << "\n";
        rk4Step(k1, k2, A, B, t, dt);
    }

    outFile.close();
}
