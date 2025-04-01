#pragma once

#include <string>

void BasicMC(int initial_A, int initial_B, double k_A, double k_B, 
                        double t_stop, const std::string& outputFilename);

void PhysisorptionMC(int initial_A, int initial_Fv, int initial_Af,
                        double k1, double k2, double t_stop, const std::string& outputFilename);

void ChemisorptionMC(int initial_A, int initial_Sv, int initial_As, int initial_A2,
                        double k1, double k2, double t_stop, const std::string& outputFilename);

void SurfaceDiffusionMC(int initial_Af, int initial_As, int initial_Fv, int initial_Sv,
                        double k1, double t_stop, const std::string& outputFilename);

void LHMC(int initial_Af, int initial_As, int initial_Fv, int initial_Sv, int initial_A2,
                        double k1, double k2, double t_stop, const std::string& outputFilename);