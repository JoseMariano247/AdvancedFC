#include "First_Concentration_Evolution.h"
#include "Runge_Kutta_concentration.h"

int main()
{
    //Simulation Parameters
    int initial_A = 10000;
    int initial_B = 10;
    double k_A = 0.1;
    double k_B = 0.01;
    double t_stop = 50.0;
    double dt = 0.01;

    MonteCarlo(initial_A, initial_B, k_A, k_B, t_stop, "Concentrations.txt"); //Monte Carlo Solver

    RungeKutta(initial_A, initial_B, k_A, k_B, dt, t_stop, "Concentrations_Runge.txt"); //Runge-Kutta 4 Solver

    return 0;
}
