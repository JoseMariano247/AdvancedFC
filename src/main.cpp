#include "First_Concentration_Evolution.h"
#include "Runge_Kutta_concentration.h"
#include "Chemiosorption.h"
#include "Chemiosorption_MC.h"
#include "Physisorption_simple_RK.h"
#include "Physisorption_simple_MC.h"

int main()
{
    //Simulation Parameters simple case
    int initial_A = 10000;
    int initial_B = 10;
    double k_A = 0.1;
    double k_B = 0.01;
    double t_stop = 50.0;
    double dt = 0.01;

    MonteCarlo(initial_A, initial_B, k_A, k_B, t_stop, "Concentrations.txt"); //Monte Carlo Solver

    RungeKutta(initial_A, initial_B, k_A, k_B, dt, t_stop, "Concentrations_Runge.txt"); //Runge-Kutta 4 Solver

    //Simulation Parameters Chemiosorption
    int initial_A_2 = 10000;
    int initial_Sv = 2000;
    int initial_As = 1000;
    int initial_A2 = 100;
    double k_1 = 0.0001;
    double k_2 = 0.00001;
    double t_stop_2 = 50.0;
    double dt_2 = 0.01;

    RungeKuttaCh(initial_A_2, initial_Sv, initial_As, initial_A2, 
        k_1, k_2, dt_2, t_stop_2, "Concentrations_Runge_Ch.txt"); //Runge-Kutta 4 Solver
    
    MonteCarloCh(initial_A_2, initial_Sv, initial_As, initial_A2, k_1, k_2, t_stop_2, "Concentrations_Monte_Carlo_Ch.txt"); //Monte Carlo Solver


    //Simulation Parameters Physisorption
    int initial_A_3 = 10000;
    int initial_Fv = 2000;
    int initial_Af = 2000;
    double k__1 = 0.0001;
    double k__2 = 0.00001;
    double t_stop_3 = 50.0;
    double dt_3 = 0.01;

    RungeKuttaFS(initial_A_3, initial_Fv, initial_Af,
        k__1, k__2,
        dt_3, t_stop_3,
        "Concentrations_Runge_Ph.txt");

    MonteCarloPh(initial_A_3, initial_Fv, initial_Af, k__1, k__2, t_stop_3, "Concentrations_MC_Ph.txt");

    return 0;
}
