#include "montecarlo.h"
#include <cstdlib>  // For rand() and RAND_MAX
#include <cmath>    // For sqrt()
#include <ctime>    // For time()

// Function implementation of the Monte Carlo simulation for PI
double MonteCarloPi(int numSamples) {
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int countInsideCircle = 0;

    // Generate numSamples random points in the unit square
    for (int i = 0; i < numSamples; ++i) {
        // Random x and y between 0 and 1
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;

        // Calculate the distance from the origin (0, 0)
        double distance = std::sqrt(x * x + y * y);

        // Check if the point is inside the quarter circle (radius 1)
        if (distance <= 1.0) {
            ++countInsideCircle;
        }
    }

    return 4.0 * countInsideCircle / numSamples;
}