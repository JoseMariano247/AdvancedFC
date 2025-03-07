#include "First_Concentration_Evolution.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

void MonteCarlo(int initial_A, int initial_B, double k_A, double k_B, 
                     double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int A = initial_A;
    int B = initial_B;
    std::vector<double> times{t};
    std::vector<int> populations_A{A};
    std::vector<int> populations_B{B};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }

    outFile << "Time\tPopulationA\tPopulationB\n";

    while (t < t_stop && A > 0 && B > 0)
    {
        double a = k_A * A;
        double b = k_B * B;
        if (a <= 0 || b <= 0) break;
        double probAtoB = (A * k_A) / (A * k_A + B * k_B);
        double r = dis(gen);
        double check = dis(gen);
        double T = -std::log(r) / (a + b);
        t += T;
        if (t > t_stop) break;

        if (check < probAtoB)
        {
            A--;
            B++;
        }
        else
        {
            B--;
            A++;
        }

        times.push_back(t);
        populations_A.push_back(A);
        populations_B.push_back(B);

        std::cout << t << "\t" << A << "\t" << B << "\n";
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t" << populations_A[i] << "\t" << populations_B[i] << "\n";
    }

    outFile.close();
}
