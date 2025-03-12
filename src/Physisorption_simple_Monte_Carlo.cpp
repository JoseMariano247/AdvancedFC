#include "Physisorption_simple_MC.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

void MonteCarloPh(int initial_A, int initial_Fv, int initial_Af,
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

while (t < t_stop && A > 0 && Fv > 0 && Af > 0) 
{
double a = k1 * A * Fv;
double b = k2 * Af;

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

std::cout << t << "\t" << A << "\t" << Fv << "\t" << Af << "\n";
}

for (size_t i = 0; i < times.size(); ++i)
{
outFile << times[i] << "\t" << populations_A[i] << "\t" << populations_Fv[i] << "\t" << populations_Af[i] << "\n";
}

outFile.close();
}
