#include "First_Concentration_Evolution.h"
#include "Runge_Kutta_concentration.h"

int main()
{
    // Monte Carlo simulation parameters
    int initial_A = 10000;
    int initial_B = 10;
    double k_A = 0.1;
    double k_B = 0.01;
    double t_stop = 50.0;
    solveMonteCarlo(initial_A, initial_B, k_A, k_B, t_stop, "Concentrations.txt");

    // Runge–Kutta simulation parameters
    double A0 = 10000.0;
    double B0 = 10.0;
    double k1 = 0.1;
    double k2 = 0.01;
    double dt = 0.01;
    double tMax = 50.0;
    solveRungeKutta(A0, B0, k1, k2, dt, tMax, "Concentrations_Runge.txt");

    return 0;
}
