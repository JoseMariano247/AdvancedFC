#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

std::ofstream outFile("Concentrations_Runge.txt");


struct Params {
    double k1, k2;
};

// Compute derivatives dA/dt and dB/dt at the current state (A, B)
void derivatives(const Params& p, double A, double B,
                 double& dAdt, double& dBdt) 
{
    dAdt = - p.k1 * A + p.k2 * B;
    dBdt = - p.k2 * B + p.k1 * A;
}

// Perform one RK4 integration step
void rk4Step(const Params& p, double& A, double& B, 
             double& t, double dt)
{
    double dA1, dB1, dA2, dB2, dA3, dB3, dA4, dB4;

    // k1
    derivatives(p, A, B, dA1, dB1);

    // k2
    derivatives(p, A + 0.5*dt*dA1, B + 0.5*dt*dB1, dA2, dB2);

    // k3
    derivatives(p, A + 0.5*dt*dA2, B + 0.5*dt*dB2, dA3, dB3);

    // k4
    derivatives(p, A + dt*dA3, B + dt*dB3, dA4, dB4);

    // Update A and B
    A += (dt/6.0) * (dA1 + 2.0*dA2 + 2.0*dA3 + dA4);
    B += (dt/6.0) * (dB1 + 2.0*dB2 + 2.0*dB3 + dB4);

    // Advance time
    t += dt;
}

int main()
{
    // Define parameters
    Params p;
    p.k1 = 0.1;
    p.k2 = 0.01;

    // Initial conditions
    double A = 10000.0;  // [A](t=0)
    double B = 10.0;  // [B](t=0)
    double t = 0.0;  // start time

    // Integration settings
    double dt    = 0.01; // time step
    double tMax  = 50.0;  // end time

    // Main integration loop
    while (t < tMax) {
        // Output current state
        std::cout << t << "\t " << A << " \t" << B << "\n";

        outFile << t << "\t" << A << "\t" << B << "\n";
        
        
        // Take one RK4 step
        rk4Step(p, A, B, t, dt);
    }

    return 0;
}
