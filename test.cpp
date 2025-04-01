#include "test.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

void BasicMC(int initial_A, int initial_B, double k_A, double k_B, 
            double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int A = initial_A;
    int B = initial_B;
    vector<double> times{t};
    vector<int> populations_A{A};
    vector<int> populations_B{B};

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    ofstream outFile(outputFilename);
    if (!outFile)
    {
        cerr << "Error opening file: " << outputFilename << endl;
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

        cout << t << "\t" << A << "\t" << B << "\n";
    }

    // Write final results to the file
    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t"
                << populations_A[i] << "\t"
                << populations_B[i] << "\n";
    }

    outFile.close();
}

void PhysisorptionMC(int initial_A, int initial_Fv, int initial_Af,
    double k1, double k2, double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int A = initial_A;
    int Fv = initial_Fv;
    int Af = initial_Af;

    std::vector<double> times{t};
    std::vector<int> populations_A{A};
    std::vector<int> populations_Fv{Fv};
    std::vector<int> populations_Af{Af};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
    std::cerr << "Error opening file: " << outputFilename << std::endl;
    return;
    }

    outFile << "Time\tPopulationA\tPopulationFv\tPopulationAf\n";
    

    while (t < t_stop) 
        {
        
        double a = k1 * A * Fv;
        double b = k2 * Af;

        if (a <= 0 & b <= 0) break;
        double probAtoB = a / (a + b);
        double r = dis(gen);
        double check = dis(gen);
        double T = -std::log(r) / (a + b);

        t += T;


        if (t > t_stop) break;

        if (check < probAtoB)
        {
        A--;
        Fv--;
        Af++;
        }
        else
        {
        Af--;
        A++;
        Fv++;
        }

        times.push_back(t);
        populations_A.push_back(A);
        populations_Fv.push_back(Fv);
        populations_Af.push_back(Af);

        cout << t << "\t" << A << "\t" << Fv << "\t" << Af << "\n";
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
    outFile << times[i] << "\t" << populations_A[i] << "\t" << populations_Fv[i] << "\t" << populations_Af[i] << "\n";
    }

    outFile.close();
}

void ChemisorptionMC(int initial_A, int initial_Sv, int initial_As, int initial_A2,
    double k1, double k2, double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int A = initial_A;
    int Sv = initial_Sv;
    int As = initial_As;
    int A2 = initial_A2;

    std::vector<double> times{t};
    std::vector<int> populations_A{A};
    std::vector<int> populations_Sv{Sv};
    std::vector<int> populations_As{As};
    std::vector<int> populations_A2{A2};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
    std::cerr << "Error opening file: " << outputFilename << std::endl;
    return;
    }

    outFile << "Time\tPopulationA\tPopulationSv\tPopulationAs\tPopulationA2\n";

    while (t < t_stop) 
    {
        double a = k1 * A * Sv;
        double b = k2 * A * As;

        if (a <= 0 & b <= 0) break;
        double probAtoB = a / (a + b);
        double r = dis(gen);
        double check = dis(gen);
        double T = -std::log(r) / (a + b);
        t += T;
        if (t > t_stop) break;

        if (check < probAtoB)
        {
        A--;
        Sv--;
        As++;
        }
        else
        {
        A--;
        As--;
        A2++;
        Sv++;
        }

        times.push_back(t);
        populations_A.push_back(A);
        populations_Sv.push_back(Sv);
        populations_As.push_back(As);
        populations_A2.push_back(A2);

        std::cout << t << "\t" << A << "\t" << Sv << "\t" << As << "\t" << A2 << "\n";
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t" << populations_A[i] << "\t" << populations_Sv[i] << "\t" << populations_As[i] << "\t" << populations_A2[i] << "\n";
    }

    outFile.close();
}

void SurfaceDiffusionMC(int initial_Af, int initial_As, int initial_Fv, int initial_Sv,
    double k1, double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int Af = initial_Af;
    int As = initial_As;
    int Fv = initial_Fv;
    int Sv = initial_Sv;

    std::vector<double> times{t};
    std::vector<int> populations_Af{Af};
    std::vector<int> populations_As{As};
    std::vector<int> populations_Fv{Fv};
    std::vector<int> populations_Sv{Sv};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
    std::cerr << "Error opening file: " << outputFilename << std::endl;
    return;
    }

    outFile << "Time\tPopulationAf\tPopulationSv\tPopulationFv\tPopulationAs\n";

    while (t < t_stop) 
    {
        double a = k1 * Af * Sv;

        double r = dis(gen);

        double T = -std::log(r) / a;
        t += T;
        if (t > t_stop) break;

        Af--;
        Sv--;
        Fv++;
        As++;
        
        times.push_back(t);
        populations_Af.push_back(Af);
        populations_Sv.push_back(Sv);
        populations_Fv.push_back(Fv);
        populations_As.push_back(As);

        std::cout << t << "\t" << Af << "\t" << Sv << "\t" << Fv << "\t" << As << "\n";
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t" << populations_Af[i] << "\t" << populations_Sv[i] << "\t" << populations_Fv[i] << "\t" << populations_As[i] << "\n";
    }

    outFile.close();
}

void LHMC(int initial_Af, int initial_As, int initial_Fv, int initial_Sv, int initial_A2,
    double k1, double k2, double t_stop, const std::string& outputFilename)
{
    double t = 0.0;
    int Af = initial_Af;
    int As = initial_As;
    int Fv = initial_Fv;
    int Sv = initial_Sv;
    int A2 = initial_A2;

    std::vector<double> times{t};
    std::vector<int> populations_Af{Af};
    std::vector<int> populations_As{As};
    std::vector<int> populations_Fv{Fv};
    std::vector<int> populations_Sv{Sv};
    std::vector<int> populations_A2{A2};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream outFile(outputFilename);
    if (!outFile)
    {
    std::cerr << "Error opening file: " << outputFilename << std::endl;
    return;
    }

    outFile << "Time\tPopulationAf\tPopulationSv\tPopulationFv\tPopulationAs\tPopulationA2\n";

    while (t < t_stop) 
    {
        double a = k1 * Af * As;
        double b = k2 * Af * Af;

        if (a <= 0 & b <= 0) break;
        double probAtoB = a / (a + b);
        double r = dis(gen);
        double check = dis(gen);
        double T = -std::log(r) / (a + b);
        t += T;
        if (t > t_stop) break;

        if (check < probAtoB)
        {
        Af--;
        As--;
        A2++;
        Sv++;
        Fv++;
        }
        else
        {
        Af--;
        Af--;
        A2++;
        Fv++;
        Fv++;
        }
        
        times.push_back(t);
        populations_Af.push_back(Af);
        populations_Sv.push_back(Sv);
        populations_Fv.push_back(Fv);
        populations_As.push_back(As);
        populations_A2.push_back(A2);

        std::cout << t << "\t" << Af << "\t" << Sv << "\t" << Fv << "\t" << As << "\t" << A2 << "\n";
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        outFile << times[i] << "\t" << populations_Af[i] << "\t" << populations_Sv[i] << "\t" << populations_Fv[i] << "\t" << populations_As[i] << "\t" << populations_A2[i] << "\n";
    }

    outFile.close();
}

int main(int argc, char* argv[]) {
   
    // Debug print of all arguments
    cout << "argc = " << argc << "\n";
    for (int i = 0; i < argc; ++i) {
        cout << "argv[" << i << "] = " << (argv[i] ? argv[i] : "null") << "\n";
    }

    if (argc < 2) {
        cerr << "No reaction type provided.\n";
        return 1;
    }
    
    // Determine the reaction type from the first argument
    string reactionType = argv[1];

    if (reactionType == "Basic") {

        // For BasicMC, we expect 5 simulation parameters
        int totalArgs = argc - 1; // excluding argv[0]
        int simParamStart = totalArgs - 5 + 1; // index where numeric params begin

        // Optionally gather reaction names from argv[1..simParamStart-1] into a vector (if needed)
        vector<string> reactions;
        for (int i = 1; i < simParamStart; ++i) {
            reactions.push_back(argv[i]);
        }
        // Expected order for BasicMC: k_A, k_B, initial_A, initial_B, t_stop
        double k_A    = std::stod(argv[simParamStart]);
        double k_B    = std::stod(argv[simParamStart + 1]);
        int initial_A = std::stoi(argv[simParamStart + 2]);
        int initial_B = std::stoi(argv[simParamStart + 3]);
        double t_stop = std::stod(argv[simParamStart + 4]);
        string outputFilename = "output.txt";

        BasicMC(initial_A, initial_B, k_A, k_B, t_stop, outputFilename);

    } else if (reactionType == "Physisorption") {

        int totalArgs = argc - 1;
        int simParamStart = totalArgs - 6 + 1; 

        // Gather reaction names (all arguments before simParamStart)
        vector<string> reactions;
        for (int i = 1; i < simParamStart; ++i) {
            reactions.push_back(argv[i]);
        }
  
        double k1        = std::stod(argv[simParamStart]);
        double k2        = std::stod(argv[simParamStart + 1]);
        int initial_A    = std::stoi(argv[simParamStart + 2]);
        int initial_Fv   = std::stoi(argv[simParamStart + 3]);
        int initial_Af   = std::stoi(argv[simParamStart + 4]);
        double t_stop    = std::stod(argv[simParamStart + 5]);


        string outputFilename = "output.txt";

        PhysisorptionMC(initial_A, initial_Fv, initial_Af, k1, k2, t_stop, outputFilename);

    } else if (reactionType == "Chemisorption") {

        int totalArgs = argc - 1;
        int simParamStart = totalArgs - 7 + 1; 

        // Gather reaction names (all arguments before simParamStart)
        vector<string> reactions;
        for (int i = 1; i < simParamStart; ++i) {
            reactions.push_back(argv[i]);
        }
  
        double k1        = std::stod(argv[simParamStart]);
        double k2        = std::stod(argv[simParamStart + 1]);
        int initial_A    = std::stoi(argv[simParamStart + 2]);
        int initial_Sv   = std::stoi(argv[simParamStart + 3]);
        int initial_As   = std::stoi(argv[simParamStart + 4]);
        int initial_A2   = std::stoi(argv[simParamStart + 5]);
        double t_stop    = std::stod(argv[simParamStart + 6]);


        string outputFilename = "output.txt";

        ChemisorptionMC(initial_A, initial_Sv, initial_As, initial_A2, k1, k2, t_stop, outputFilename);

    } else if (reactionType == "Surface Diffusion") {

        int totalArgs = argc - 1;
        int simParamStart = totalArgs - 6 + 1; 

        // Gather reaction names (all arguments before simParamStart)
        vector<string> reactions;
        for (int i = 1; i < simParamStart; ++i) {
            reactions.push_back(argv[i]);
        }
  
        double k1        = std::stod(argv[simParamStart]);
        int initial_Af   = std::stoi(argv[simParamStart + 1]);
        int initial_Sv   = std::stoi(argv[simParamStart + 2]);
        int initial_Fv   = std::stoi(argv[simParamStart + 3]);
        int initial_As   = std::stoi(argv[simParamStart + 4]);
        double t_stop    = std::stod(argv[simParamStart + 5]);


        string outputFilename = "output.txt";

        SurfaceDiffusionMC(initial_Af, initial_As, initial_Fv, initial_Sv,
                           k1, t_stop, outputFilename);

    } else if (reactionType == "Langmuir-Hinshelwood recombination") {

        int totalArgs = argc - 1;
        int simParamStart = totalArgs - 8 + 1; 

        // Gather reaction names (all arguments before simParamStart)
        vector<string> reactions;
        for (int i = 1; i < simParamStart; ++i) {
            reactions.push_back(argv[i]);
        }
  
        double k1        = std::stod(argv[simParamStart]);
        double k2        = std::stod(argv[simParamStart + 1]);
        int initial_Af   = std::stoi(argv[simParamStart + 2]);
        int initial_Sv   = std::stoi(argv[simParamStart + 3]);
        int initial_Fv   = std::stoi(argv[simParamStart + 4]);
        int initial_As   = std::stoi(argv[simParamStart + 5]);
        int initial_A2   = std::stoi(argv[simParamStart + 6]);
        double t_stop    = std::stod(argv[simParamStart + 7]);


        string outputFilename = "output.txt";

        LHMC(initial_Af, initial_As, initial_Fv, initial_Sv, initial_A2,
            k1, k2, t_stop, outputFilename);

    } else {
        cerr << "Unknown reaction type: " << reactionType << "\n";
        return 1;
    }

    return 0;
}
