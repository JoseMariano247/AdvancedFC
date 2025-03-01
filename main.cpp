#include <iostream>
#include "montecarlo.h"

int main() {
    int samples = 1000000; // Number of random points to generate
    double pi = MonteCarloPi(samples); // Estimate PI using Monte Carlo simulation

    std::cout << "Approximated value of PI using " << samples 
              << " samples is: " << pi << std::endl;

    return 0;
}
