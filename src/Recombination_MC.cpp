#include "Recombination_MC.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

void MonteCarloRecombination(int initial_A, int initial_Fv, int initial_Af,
                                       int initial_Sv, int initial_As, int initial_A2,
                                       double r1, double r2, double r3, double r4,
                                       double r5, double r6, double r7,
                                       double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int A = initial_A;
    int Fv = initial_Fv;
    int Af = initial_Af;
    int Sv = initial_Sv;
    int As = initial_As;
    int A2 = initial_A2;

    std::vector<double> times{ t };
    std::vector<int> populations_A{ A };
    std::vector<int> populations_Fv{ Fv };
    std::vector<int> populations_Af{ Af };
    std::vector<int> populations_Sv{ Sv };
    std::vector<int> populations_As{ As };
    std::vector<int> populations_A2{ A2 };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }
    
    outFile << "Time\tA\tFv\tAf\tSv\tAs\tA2\n";

    while (t < t_stop)
    {
        double R1 = r1 * A * Fv;      // A + Fv -> Af
        double R2 = r2 * Af;          // Af -> A + Fv
        double R3 = r3 * A * Sv;      // A + Sv -> As
        double R4 = r4 * A * As;      // A + As -> A2 + Sv
        double R5 = r5 * Af * Sv;     // Af + Sv -> Fv + As
        double R6 = r6 * Af * As;     // Af + As -> A2 + Sv + Fv
        double R7 = (Af >= 2) ? r7 * Af * Af : 0;  // Af + Af -> A2 + 2 Fv

        double totalRate = R1 + R2 + R3 + R4 + R5 + R6 + R7;
        if (totalRate <= 0) break;

        double r_time = dis(gen);
        double dt = -std::log(r_time) / totalRate;
        t += dt;
        if (t > t_stop) break;

        double r_choice = dis(gen) * totalRate;
        double cumulative = 0.0;
        int reaction = -1;

        if ((cumulative += R1) >= r_choice)
            reaction = 1;
        else if ((cumulative += R2) >= r_choice)
            reaction = 2;
        else if ((cumulative += R3) >= r_choice)
            reaction = 3;
        else if ((cumulative += R4) >= r_choice)
            reaction = 4;
        else if ((cumulative += R5) >= r_choice)
            reaction = 5;
        else if ((cumulative += R6) >= r_choice)
            reaction = 6;
        else if ((cumulative += R7) >= r_choice)
            reaction = 7;

        // Update species counts based on the chosen reaction:
        switch (reaction)
        {
            case 1: // A + Fv -> Af
                if (A > 0 && Fv > 0) { A--; Fv--; Af++; }
                break;
            case 2: // Af -> A + Fv
                if (Af > 0) { Af--; A++; Fv++; }
                break;
            case 3: // A + Sv -> As
                if (A > 0 && Sv > 0) { A--; Sv--; As++; }
                break;
            case 4: // A + As -> A2 + Sv
                if (A > 0 && As > 0) { A--; As--; A2++; Sv++; }
                break;
            case 5: // Af + Sv -> Fv + As
                if (Af > 0 && Sv > 0) { Af--; Sv--; Fv++; As++; }
                break;
            case 6: // Af + As -> A2 + Sv + Fv
                if (Af > 0 && As > 0) { Af--; As--; A2++; Sv++; Fv++; }
                break;
            case 7: // Af + Af -> A2 + 2 Fv
                if (Af >= 2) { Af -= 2; A2++; Fv += 2; }
                break;
            default:
                break;
        }

        // Record the updated state:
        times.push_back(t);
        populations_A.push_back(A);
        populations_Fv.push_back(Fv);
        populations_Af.push_back(Af);
        populations_Sv.push_back(Sv);
        populations_As.push_back(As);
        populations_A2.push_back(A2);

        // Optional: Print current state to console
        std::cout << t << "\t" << A << "\t" << Fv << "\t" << Af << "\t"
                  << Sv << "\t" << As << "\t" << A2 << "\n";
    }

    // Write simulation results to file:
    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t" << populations_A[i] << "\t" 
                << populations_Fv[i] << "\t" << populations_Af[i] << "\t" 
                << populations_Sv[i] << "\t" << populations_As[i] << "\t" 
                << populations_A2[i] << "\n";
    }

    outFile.close();
}
