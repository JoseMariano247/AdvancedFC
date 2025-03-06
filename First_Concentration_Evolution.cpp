#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

/*
This code simulates the reaction between two species that change from one
to the other.
*/



int main() {
    // Simulation parameters
    int initial_A = 10000;    // Initial number of A molecules
    int initial_B = 10;        // Initial number of B molecules
    double k_A = 0.1;           // Reaction rate constant for A -> B
    double k_B = 0.01;          // Reaction rate constant for B -> A
    double t_stop = 50.0;     // End time for simulation
    double stop; 

    // Initialise simulation state
    double t = 0.0;
    int A = initial_A;
    int B = initial_B;
    std::vector<double> times;
    std::vector<int> populations_A;
    std::vector<int> populations_B;
    times.push_back(t);
    populations_A.push_back(A);
    populations_B.push_back(B);

    // Set up random number generator (C++11)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile("Concentrations.txt");

    // Write headers (optional)
    outFile << "Time\tPopulationA\tPopulationB\n";

    // Main simulation loop
    while (t < t_stop && A > 0 && B > 0) {
        // Calculate the total propensity for the unimolecular decay
        double a = k_A * A;
        double b = k_B * B;
        if (a <= 0) break;
        if (b <= 0) break;
        stop = A * k_A/(A * k_A + B * k_B);

        // Draw a uniform random number r in (0,1) to determine the time increment
        double r = dis(gen);
        double check = dis(gen);
        double T = -std::log(r) / (a + b);  // T = (1/a)*ln(1/r)
        t += T;
        if (t > t_stop) break;

        if (check < stop) {
            A--;
            B++;
        } else {
            B--;
            A++;
        }
        

        // Record time and molecule count
        times.push_back(t);
        populations_A.push_back(A);
        populations_B.push_back(B);
        std::cout << t << "\t" << A << "\t" << B << "\n";

    }

    for (size_t i = 0; i < times.size(); ++i) {
        outFile << times[i] << "\t" << populations_A[i] << "\t" << populations_B[i] << "\n";
    }

    // Close the file
    outFile.close();
    

    return 0;
}