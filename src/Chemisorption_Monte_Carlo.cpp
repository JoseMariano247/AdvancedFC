#include "Chemiosorption_MC.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

void MonteCarloCh(int initial_A, int initial_Sv, int initial_As, int initial_A2,
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

while (t < t_stop && A > 0 && Sv > 0 && As > 0 && A2 > 0) 
{
double a = k1 * A * Sv;
double b = k2 * A * As;

if (a <= 0 || b <= 0) break;
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
