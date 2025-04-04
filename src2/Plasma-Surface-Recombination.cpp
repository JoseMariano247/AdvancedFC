#include "Plasma-Surface-Recombination.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include <functional>
#include <map>
#include <algorithm>
#include <set>
#include <stdio.h>

#ifndef pi
#define pi 3.14159265358979323846
#endif

using namespace std;

// Global variable for initial concentration of A.
double initial_A = 0.0;

// Global general parameters (for non‑Basic reactions). They will be extracted once.
double global_Tw = 0.0, global_Tg = 0.0, global_M = 0.0;
bool general_params_extracted = false;

double vD;
double ED;
double Er;
double k_4;

/* 
 * Dictionary defining which species each reaction uses.
 */
static const map<string, vector<string>> reactionSpecies = {
    {"Basic", {"A", "B"}},
    {"Physisorption", {"A", "Fv", "Af"}},
    {"Chemisorption", {"A", "Sv", "As", "A2"}},
    {"Surface Diffusion", {"Af", "Sv", "Fv", "As"}},
    {"Langmuir-Hinshelwood recombination", {"Af", "As", "Fv", "Sv", "A2"}}
};

/* 
 * Dictionary for number of rate constants required for each reaction.
 * (These are the nominal numbers when each reaction is alone.)
 * For non‑Basic reactions the GUI supplies 3 extra general parameters (Tw, Tg, M) once.
 */
static const map<string, int> reactionRateCount = {
    {"Basic", 2},
    {"Physisorption", 3},
    {"Chemisorption", 3},
    {"Surface Diffusion", 2},
    {"Langmuir-Hinshelwood recombination", 5}
};

/*
 * Structure for a reaction event.
 */
struct ReactionEvent {
    function<double(const vector<double>&, double)> propensity;
    vector<double> delta;
    double k;
};

/*
 * Helper lambdas for propensity functions.
 */
auto prop_single = [](int idx) {
    return [=](const vector<double>& state, double k) -> double {
        return k * state[idx];
    };
};

auto prop_bimolecular = [](int idx1, int idx2) {
    return [=](const vector<double>& state, double k) -> double {
        return k * state[idx1] * state[idx2];
    };
};

auto prop_square = [](int idx) {
    return [=](const vector<double>& state, double k) -> double {
        return k * state[idx] * state[idx];
    };
};

/*
 * Build ReactionEvent objects for a given reaction.
 * Accepts additional Boolean flags:
 *   chemPresent: whether "Chemisorption" is selected
 *   surfPresent: whether "Surface Diffusion" is selected
 */
vector<ReactionEvent> buildEventsForReaction(const string& reaction, 
                                             const vector<double>& rates, 
                                             int& rateIndex, 
                                             const map<string,int>& speciesIndex,
                                             bool chemPresent,
                                             bool surfPresent)
{
    double kb = 1.380649e-23;  // J/K
    double Na = 6.023e23;      // mol^-1

    vector<ReactionEvent> result;
    auto idx = [&](const string& s){ return speciesIndex.at(s); };

    if (reaction == "Basic") {
        double kA = rates[rateIndex++];
        double kB = rates[rateIndex++];
        {
            ReactionEvent e;
            e.k = kA;
            e.propensity = prop_single(idx("A"));
            e.delta.resize(speciesIndex.size(), 0.0);
            e.delta[idx("A")] = -1.0;
            e.delta[idx("B")] = +1.0;
            result.push_back(e);
        }
        {
            ReactionEvent e;
            e.k = kB;
            e.propensity = prop_single(idx("B"));
            e.delta.resize(speciesIndex.size(), 0.0);
            e.delta[idx("B")] = -1.0;
            e.delta[idx("A")] = +1.0;
            result.push_back(e);
        }
    }
    else {
        // For non‑Basic reactions, extract general parameters only once.
        if (!general_params_extracted) {
            global_Tw = rates[rateIndex++];
            global_Tg = rates[rateIndex++];
            global_M  = rates[rateIndex++];
            general_params_extracted = true;
        }
        double v_med = std::sqrt((8 * kb * global_Tg * Na) / (pi * global_M));
        double phi_O = 0.25 * v_med * initial_A;

        if (reaction == "Physisorption") {
            // Extract 3 parameters: k_1, vd, Ed.
            double k_1 = rates[rateIndex++];
            double vd  = rates[rateIndex++];
            double Ed  = rates[rateIndex++];
            {
                ReactionEvent e;
                e.k = k_1 * phi_O;
                e.propensity = prop_bimolecular(idx("A"), idx("Fv"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("A")]  = -1.0;
                e.delta[idx("Fv")] = -1.0;
                e.delta[idx("Af")] = +1.0;
                result.push_back(e);
            }
            {
                ReactionEvent e;
                e.k = vd * std::exp(-Ed / (Na * kb * global_Tw));
                e.propensity = prop_single(idx("Af"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("Af")] = -1.0;
                e.delta[idx("A")]  = +1.0;
                e.delta[idx("Fv")] = +1.0;
                result.push_back(e);
            }
        }
        else if (reaction == "Chemisorption") {
            // Extract 3 parameters: k_3, k_4, Er.
            double k_3 = rates[rateIndex++];
            k_4 = rates[rateIndex++];
            Er  = rates[rateIndex++];
            double Pr  = k_4 * std::exp(-Er / (Na * kb * global_Tw));
            {
                ReactionEvent e;
                e.k = k_3 * phi_O;
                e.propensity = prop_bimolecular(idx("A"), idx("Sv"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("A")]  = -1.0;
                e.delta[idx("Sv")] = -1.0;
                e.delta[idx("As")] = +1.0;
                result.push_back(e);
            }
            {
                ReactionEvent e;
                e.k = Pr * k_3 * phi_O;
                e.propensity = prop_bimolecular(idx("A"), idx("As"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("A")]  = -1.0;
                e.delta[idx("As")] = -1.0;
                e.delta[idx("A2")] = +1.0;
                e.delta[idx("Sv")] = +1.0;
                result.push_back(e);
            }
        }
        else if (reaction == "Surface Diffusion") {
            // Extract 2 parameters: vD and ED.
            vD = rates[rateIndex++];
            ED = rates[rateIndex++];
            double tau_d_1 = vD * std::exp(-ED / (Na * kb * global_Tw));
            {
                ReactionEvent e;
                e.k = 0.75 * tau_d_1;
                e.propensity = prop_bimolecular(idx("Af"), idx("Sv"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("Af")] = -1.0;
                e.delta[idx("Sv")] = -1.0;
                e.delta[idx("Fv")] = +1.0;
                e.delta[idx("As")] = +1.0;
                result.push_back(e);
            }
        }
        else if (reaction == "Langmuir-Hinshelwood recombination") {
            // LH normally requires 5 parameters: vD, ED, k4, Er, ELHF.
            // Adjust extraction based on presence of Chemisorption and Surface Diffusion.
            double vD_local = 0.0, ED_local = 0.0, k4_local = 1.0, Er_local = 0.0, ELHF_local = 0.0;
            double tau_d_1 = 1.0;
            if (!chemPresent && !surfPresent) {
                // LH alone: extract all 5.
                vD_local = rates[rateIndex++];
                ED_local = rates[rateIndex++];
                k4_local = rates[rateIndex++];
                Er_local = rates[rateIndex++];
                ELHF_local = rates[rateIndex++];
                tau_d_1 = vD_local * std::exp(-ED_local / (Na * kb * global_Tw));
            } else if (chemPresent && !surfPresent) {
                // Chemisorption present: LH extracts vD, ED, and ELHF.
                vD_local = rates[rateIndex++];
                ED_local = rates[rateIndex++];
                ELHF_local = rates[rateIndex++];
                tau_d_1 = vD_local * std::exp(-ED_local / (Na * kb * global_Tw));
                // k4_local and Er_local assumed provided by Chemisorption.
            } else if (!chemPresent && surfPresent) {
                // Surface Diffusion present: LH extracts k4, Er, and ELHF.
                k4_local = rates[rateIndex++];
                Er_local = rates[rateIndex++];
                ELHF_local = rates[rateIndex++];
                // tau_d_1 assumed provided by Surface Diffusion.
            } else if (chemPresent && surfPresent) {
                // Both present: LH extracts only ELHF.
                ELHF_local = rates[rateIndex++];
                tau_d_1 = vD * std::exp(-ED / (Na * kb * global_Tw));
                k4_local = k_4;
                Er_local = Er;
            }
            double Pr   = k4_local * std::exp(-Er_local / (Na * kb * global_Tw));
            double Prlh = k4_local * std::exp(-ELHF_local / (Na * kb * global_Tw));
            {
                ReactionEvent e;
                e.k = tau_d_1 * Pr;
                e.propensity = prop_bimolecular(idx("Af"), idx("As"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("Af")] = -1.0;
                e.delta[idx("As")] = -1.0;
                e.delta[idx("A2")] = +1.0;
                e.delta[idx("Sv")] = +1.0;
                e.delta[idx("Fv")] = +1.0;
                result.push_back(e);
            }
            {
                ReactionEvent e;
                e.k = tau_d_1 * Prlh;
                e.propensity = prop_square(idx("Af"));
                e.delta.resize(speciesIndex.size(), 0.0);
                e.delta[idx("Af")] = -2.0;
                e.delta[idx("A2")] = +1.0;
                e.delta[idx("Fv")] = +2.0;
                result.push_back(e);
            }
        }
    }
    return result;
}

/*
 * Gillespie simulation that prints the state at each time step.
 */
void simulateMultiReaction(double t_stop, 
                           const vector<ReactionEvent>& events, 
                           vector<double>& state,
                           const vector<string>& speciesList,
                           const string& outputFilename)
{
    double t = 0.0;
    vector<double> times{t};
    vector<vector<double>> states{state};

    cout << "Time";
    for (auto &s : speciesList)
        cout << "\t" << s;
    cout << "\n";
    
    cout << t;
    for (double x : state)
        cout << "\t" << x;
    cout << "\n" << flush;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    while (t < t_stop) {
        double total_rate = 0.0;
        vector<double> rvec;
        rvec.reserve(events.size());
        for (auto &evt : events) {
            double r = evt.propensity(state, evt.k);
            rvec.push_back(r);
            total_rate += r;
        }
        if (total_rate <= 1e-15)
            break;

        double r1 = dis(gen);
        double dt = -log(r1) / total_rate;
        t += dt;
        if (t > t_stop)
            break;

        double r2 = dis(gen) * total_rate;
        double cum = 0.0;
        int chosen = -1;
        for (int i = 0; i < (int)events.size(); i++) {
            cum += rvec[i];
            if (cum >= r2) {
                chosen = i;
                break;
            }
        }
        if (chosen < 0)
            break;
        
        for (int i = 0; i < (int)state.size(); i++) {
            state[i] += events[chosen].delta[i];
            if (state[i] < 0)
                state[i] = 0;
        }
        times.push_back(t);
        states.push_back(state);
        
        cout << t;
        for (double x : state)
            cout << "\t" << x;
        cout << "\n" << flush;
    }

    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Error opening file: " << outputFilename << "\n";
        return;
    }
    outFile << "Time";
    for (auto &s : speciesList) {
        outFile << "\t" << "Population" << s;
    }
    outFile << "\n";
    for (size_t i = 0; i < times.size(); i++) {
        outFile << times[i];
        for (int j = 0; j < (int)speciesList.size(); j++) {
            outFile << "\t" << states[i][j];
        }
        outFile << "\n";
    }
    outFile.close();
    cout << "Simulation complete. Output written to " << outputFilename << "\n";
}

/*
 * Main function.
 * Expects:
 *   - Reaction names (until a numeric token is encountered)
 *   - Then: [needed rate constants] + [initial populations for each species in union] + [t_stop]
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "No arguments provided.\n";
        return 1;
    }
    
    // Print all command-line arguments.
    cout << "Command-line arguments:" << "\n";
    for (int i = 0; i < argc; i++) {
        cout << "argv[" << i << "]: " << argv[i] << "\n";
    }
    
    vector<string> reactions;
    int argIndex = 1;
    while (argIndex < argc) {
        string token = argv[argIndex];
        try {
            stod(token); // token convertible to number, stop.
            break;
        } catch (...) {
            reactions.push_back(token);
            argIndex++;
        }
    }
    if (reactions.empty()) {
        cerr << "No reactions specified.\n";
        return 1;
    }
    cout << "Selected reactions: ";
    for (auto &r : reactions)
        cout << r << " ";
    cout << "\n";

    int neededRateConstants = 0;
    for (auto &r : reactions) {
        auto it = reactionRateCount.find(r);
        if (it == reactionRateCount.end()) {
            cerr << "Unknown reaction: " << r << "\n";
            return 1;
        }
        neededRateConstants += it->second;
    }
    cout << "Needed rate constants (nominal): " << neededRateConstants << "\n";

    // Adjust needed rate constants for LH recombination if shared parameters are provided.
    if (find(reactions.begin(), reactions.end(), "Langmuir-Hinshelwood recombination") != reactions.end()) {
        // If LH is present:
        if ( (find(reactions.begin(), reactions.end(), "Chemisorption") != reactions.end()) &&
             (find(reactions.begin(), reactions.end(), "Surface Diffusion") != reactions.end()) ) {
            // Both present: LH effective parameters = 1.
            neededRateConstants = neededRateConstants - reactionRateCount.at("Langmuir-Hinshelwood recombination") + 1;
        }
        else if (find(reactions.begin(), reactions.end(), "Chemisorption") != reactions.end()) {
            // Chemisorption present: LH effective parameters = 3.
            neededRateConstants = neededRateConstants - reactionRateCount.at("Langmuir-Hinshelwood recombination") + 3;
        }
        else if (find(reactions.begin(), reactions.end(), "Surface Diffusion") != reactions.end()) {
            // Surface Diffusion present: LH effective parameters = 3.
            neededRateConstants = neededRateConstants - reactionRateCount.at("Langmuir-Hinshelwood recombination") + 3;
        }
    }
    cout << "Adjusted needed rate constants: " << neededRateConstants << "\n";

    set<string> usedSpecies;
    for (auto &r : reactions) {
        auto it = reactionSpecies.find(r);
        if (it == reactionSpecies.end()) {
            cerr << "No species info for reaction: " << r << "\n";
            return 1;
        }
        for (auto &s : it->second)
            usedSpecies.insert(s);
    }
    vector<string> allSpecies;
    vector<string> fixedOrder = {"A", "B", "Af", "As", "Fv", "Sv", "A2"};
    for (const auto &s : fixedOrder) {
        if (usedSpecies.find(s) != usedSpecies.end())
            allSpecies.push_back(s);
    }
    cout << "Union of species: ";
    for (auto &s : allSpecies)
        cout << s << " ";
    cout << "\n";

    int totalNumericNeeded = 0;
    if (reactions.size() == 1 && reactions[0] == "Basic") {
        totalNumericNeeded = 2 + (int)allSpecies.size() + 1;
    } else {
        totalNumericNeeded = 3 + neededRateConstants + (int)allSpecies.size() + 1;
    }
    int numericAvailable = argc - argIndex;
    cout << "Total numeric parameters expected: " << totalNumericNeeded 
         << ", numeric available: " << numericAvailable << "\n";
    if (numericAvailable < totalNumericNeeded) {
        cerr << "Not enough numeric parameters provided.\n";
        cerr << "Expected " << totalNumericNeeded << ", got " << numericAvailable << ".\n";
        return 1;
    }

    vector<double> rates;
    rates.reserve(neededRateConstants);
    if (reactions.size() == 1 && reactions[0] == "Basic") {
        for (int i = 0; i < 2; i++) {
            double val = stod(argv[argIndex++]);
            rates.push_back(val);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            double val = stod(argv[argIndex++]);
            rates.push_back(val);
        }
        for (int i = 0; i < neededRateConstants; i++) {
            double val = stod(argv[argIndex++]);
            rates.push_back(val);
        }
    }
    cout << "Parsed rate constants: ";
    for (auto &v : rates)
        cout << v << " ";
    cout << "\n";

    vector<double> initState(allSpecies.size(), 0.0);
    for (int i = 0; i < (int)allSpecies.size(); i++) {
        initState[i] = stod(argv[argIndex++]);
    }
    cout << "Parsed initial state: ";
    for (auto &v : initState)
        cout << v << " ";
    cout << "\n";

    double t_stop = stod(argv[argIndex++]);
    cout << "Parsed t_stop: " << t_stop << "\n";

    // Build mapping from species name to index.
    map<string,int> speciesMap;
    for (int i = 0; i < (int)allSpecies.size(); i++) {
        speciesMap[allSpecies[i]] = i;
    }

    if (speciesMap.find("A") != speciesMap.end()) {
        initial_A = initState[speciesMap["A"]];
        cout << "Global initial_A: " << initial_A << "\n";
    } else {
        cerr << "Species A is not in the union; cannot define initial_A.\n";
    }

    // Determine if Chemisorption and/or Surface Diffusion are present.
    bool chemPresent = false, surfPresent = false;
    for (auto &r : reactions) {
        if (r == "Chemisorption") chemPresent = true;
        if (r == "Surface Diffusion") surfPresent = true;
    }

    vector<ReactionEvent> events;
    int ratePos = 0;
    for (auto &r : reactions) {
        vector<ReactionEvent> these = buildEventsForReaction(r, rates, ratePos, speciesMap, chemPresent, surfPresent);
        events.insert(events.end(), these.begin(), these.end());
    }
    cout << "Total ReactionEvents built: " << events.size() << "\n";

    string outputFilename = "output.txt";
    simulateMultiReaction(t_stop, events, initState, allSpecies, outputFilename);

    return 0;
}
