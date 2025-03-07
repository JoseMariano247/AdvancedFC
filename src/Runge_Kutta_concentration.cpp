#include "Runge_Kutta_concentration.h"
#include <iostream>
#include <cmath>
#include <fstream>

struct Params {
    double k1, k2;
};

static void derivatives(const Params& p, double A, double B, 
                        double& dAdt, double& dBdt)
{
    dAdt = -p.k1 * A + p.k2 * B;
    dBdt = -p.k2 * B + p.k1 * A;
}

static void rk4Step(const Params& p, double& A, double& B, double& t, double dt)
{
    double dA1, dB1, dA2, dB2, dA3, dB3, dA4, dB4;

    // k1
    derivatives(p, A, B, dA1, dB1);
    // k2
    derivatives(p, A + 0.5 * dt * dA1, B + 0.5 * dt * dB1, dA2, dB2);
    // k3
    derivatives(p, A + 0.5 * dt * dA2, B + 0.5 * dt * dB2, dA3, dB3);
    // k4
    derivatives(p, A + dt * dA3, B + dt * dB3, dA4, dB4);

    A += (dt / 6.0) * (dA1 + 2.0 * dA2 + 2.0 * dA3 + dA4);
    B += (dt / 6.0) * (dB1 + 2.0 * dB2 + 2.0 * dB3 + dB4);
    t += dt;
}

void solveRungeKutta(double A, double B, double k1, double k2, 
                     double dt, double tMax, const std::string& outputFilename)
{
    Params p {k1, k2};
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
        rk4Step(p, A, B, t, dt);
    }

    outFile.close();
}
