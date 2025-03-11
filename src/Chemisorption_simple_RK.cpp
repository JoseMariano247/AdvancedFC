#include "Chemiosorption.h"
#include <iostream>
#include <cmath>
#include <fstream>

static void derivatives2(double k1, double k2, double A, double Sv, double As, 
    double A2, double& dAdt, double& dSvdt, double& dAsdt, double& dA2dt)
{
    dAdt = -k1 * A * Sv - k2 * A * As;
    dSvdt = -k1 * A * Sv + k2 * A * As;
    dAsdt = k1 * A * Sv - k2 * A * As;
    dA2dt = k2 * A * As;
}

static void rk4Step2(double& k1, double& k2, double& A, double& Sv, double& As,
     double& A2, double& t, double dt)
{
    double dA1, dSv1, dAs1, dA21, dA2, dSv2, dAs2, dA22, dA3, dSv3, dAs3, dA23, dA4, dSv4, dAs4, dA24;

    // k1
    derivatives2(k1, k2, A, Sv, As, A2, dA1, dSv1, dAs1, dA21);
    // k2
    derivatives2(k1, k2, A + 0.5 * dt * dA1, Sv + 0.5 * dt * dSv1, As + 0.5 * dt * dAs1, 
        A2 + 0.5 * dt * dA21, dA2, dSv2, dAs2, dA21);
    // k3
    derivatives2(k1, k2, A + 0.5 * dt * dA2, Sv + 0.5 * dt * dSv2, As + 0.5 * dt * dAs2, 
        A2 + 0.5 * dt * dA22, dA3, dSv3, dAs3, dA23);
    // k4
    derivatives2(k1, k2, A + dt * dA3, Sv + dt * dSv3, As + dt * dAs3, 
        A2 + dt * dA23, dA4, dSv4, dAs4, dA24);

    A += (dt / 6.0) * (dA1 + 2.0 * dA2 + 2.0 * dA3 + dA4);
    Sv += (dt / 6.0) * (dSv1 + 2.0 * dSv2 + 2.0 * dSv3 + dSv4);
    As += (dt / 6.0) * (dAs1 + 2.0 * dAs2 + 2.0 * dAs3 + dAs4);
    A2 += (dt / 6.0) * (dA21 + 2.0 * dA22 + 2.0 * dA23 + dA24);
    t += dt;
}

void RungeKuttaCh(double A, double Sv, double As, double A2, double k1, double k2, 
    double dt, double tMax, const std::string& outputFilename)
{
double t = 0.0;

std::ofstream outFile(outputFilename);
if (!outFile)
{
std::cerr << "Error opening file: " << outputFilename << std::endl;
return;
}

outFile << "Time\tA\tSv\tAs\tA2\n";

while (t < tMax)
{
std::cout << t << "\t" << A << "\t" << Sv << "\t" << As << "\t" << A2 << "\n";
outFile << t << "\t" << A << "\t" << Sv << "\t" << As << "\t" << A2 << "\n";
rk4Step2(k1, k2, A, Sv, As, A2, t, dt);
}

outFile.close();
}
