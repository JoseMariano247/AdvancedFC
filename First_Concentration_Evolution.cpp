#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

int main() {
    // Simulation parameters
    int initial_A = 10000;      // Initial number of A molecules
    double k = 0.1;           // Reaction rate constant
    double t_stop = 50.0;     // End time for simulation

    // Initialise simulation state
    double t = 0.0;
    int A = initial_A;
    std::vector<double> times;
    std::vector<int> populations;
    times.push_back(t);
    populations.push_back(A);

    // Set up random number generator (C++11)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile("output.txt");

    // Write headers (optional)
    outFile << "Time\tPopulation\n";

    // Main simulation loop
    while (t < t_stop && A > 0) {
        // Calculate the total propensity for the unimolecular decay
        double a = k * A;
        if (a <= 0) break;

        // Draw a uniform random number r in (0,1) to determine the time increment
        double r = dis(gen);
        double T = -std::log(r) / a;  // T = (1/a)*ln(1/r)
        t += T;
        if (t > t_stop) break;

        // Update state: decay reaction reduces molecule count by 1 (A -> ∅)
        A--;

        // Record time and molecule count
        times.push_back(t);
        populations.push_back(A);
        std::cout << t << "\t" << A << "\n";
        // Write data to the file
    }

    for (size_t i = 0; i < times.size(); ++i) {
        std::cout <<times[i] << "\t" << populations[i] << "\n";
        outFile << times[i] << "\t" << populations[i] << "\n";
    }

    // Close the file
    outFile.close();
    

    return 0;
}