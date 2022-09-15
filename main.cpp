#include <iostream>
#include "treequeue.hpp"
#include "player.hpp"
#include "cfgToSettings.hpp"
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;

//Forward Declarations for useful functions
template <typename T>
size_t relevantIndex(const vector<T> &v, T element, size_t lower, size_t upper);
void reset(vector<Player *> &playerVec, vector<float> &resetPcts, vector<u_int16_t> &resetRegions,
           u_int16_t resetBase, u_int16_t resetMax);
void printCompilerCommands();
void resultsToFile(vector<Player *> &playerVec, string filename);

int main(int argc, const char **argv){
    cout << endl; //start with newline
    string outputPrefix = "simulation";
    string settingsFile = "settings.txt";
    string playerStatsFile = "./ladderSample/25k.txt";
    bool resultsEverySeason = false;
    bool verbose = false;
    string configName = "";
    long long seed = time(0);
    vector<string> args(argv + 1, argv + argc);
    
    //process compiler flags
    for (size_t i = 0; i < args.size(); ++i) {
        string arg = args[i];
        if (arg == "-h") {
            printCompilerCommands();
            exit(0);
        } else if (arg == "-s") {
            resultsEverySeason = true;
        } else if (arg == "-v") {
            verbose = true;
        } else if (arg == "-o" and i != args.size() - 1) {
            ++i;
            outputPrefix = args[i];
        } else if (arg == "-i" and i != args.size() - 1) {
            ++i;
            settingsFile = args[i];
        } else if (arg == "-p" and i != args.size() - 1) {
            ++i;
            playerStatsFile = args[i];
        } else if (arg == "-d" and i != args.size() - 1) {
            ++i;
            seed = stoi(args[i]);
        }
        else if (arg == "-c" and i != args.size() - 1) {
            ++i;
            configName = args[i];
        }
    }
    //Read settings from config file
    if (configName != ""){
        auto [cfgStats, cfgSeed] = readFromCfg(configName);
        settingsFile = "temp.settings";
        seed = cfgSeed;
        playerStatsFile = cfgStats;
    }
    string outputFile = outputPrefix + ".results";
    cout << "Settings File: " << settingsFile << endl;
    cout << "Getting Player stats from: " << playerStatsFile << endl;
    cout << "Results file: " << outputFile << endl;
    if (resultsEverySeason){
        cout << "Writing results to a file every season" << endl;
    }
    cout << "RNG Seed: " << seed << endl;
    ifstream params(settingsFile);
    ifstream playerStats(playerStatsFile);
    if (!params or !playerStats){
        if (!params){
            cout << "File " << settingsFile <<" not found" << endl;
        } 
        if (!playerStats){
            cout << "File " << playerStatsFile << " not found" << endl;
        }
        exit(1);
        }
    //numPlayers CANNOT ME MORE THAN THE NUMBER OF PLAYERS IN THE INPUT FILE
    size_t numPlayers;
    // maximum number of battles per season is 4.29 billion
    // CR plays roughly 2.1 billion battles per season with 7.5 million players. 
    unsigned int numBattles;

    //parameter reading in vars
    size_t numRegions,numGates,numRegionsReset;
    u_int16_t trophyReg, gate, resetRegion, resetMax;
    float lossPct, resetPct;
    u_int8_t mmType;
    int clCap, numSeasons;

    //player reading in vars
    short kt, cl;
    short trophies;
    float partyPct;

    //vectors of data
    vector<u_int16_t> trophyRegions;
    vector<float> lossPcts;
    vector<u_int8_t> mmTypes;
    vector<int> clCaps;
    vector<u_int16_t> gates;
    vector<u_int16_t> resetRegions;
    vector<float> resetPcts;
    vector<Player *> playerVec;
    u_int16_t resetBase;

    //Read in Settings
    params >> numBattles;
    params >> numPlayers;
    params >> numRegions;
    params >> numGates;
    params >> numRegionsReset;
    params >> resetBase;
    params >> resetMax;
    params >> numSeasons;
    for (size_t i = 0; i < numRegions; ++i) {
        params >> trophyReg;
        params >> lossPct;
        params >> mmType;
        params >> clCap;
        trophyRegions.push_back(trophyReg);
        lossPcts.push_back(lossPct);
        mmTypes.push_back(mmType);
        clCaps.push_back(clCap);
    }
    for (size_t i = 0; i < numGates; ++i) {
        params >> gate;
        gates.push_back(gate);
    }
    for (size_t i = 0; i < numRegionsReset; ++i) {
        params >> resetRegion;
        params >> resetPct;
        resetRegions.push_back(resetRegion);
        resetPcts.push_back(resetPct);
    }
    params.close();
    
    // Read in players
    for (size_t i = 0; i < numPlayers; ++i) {
        playerStats >> kt;
        playerStats >> trophies;
        playerStats >> partyPct;
        playerStats >> cl;
        // playerStats >> skill;
        playerVec.push_back(new Player(i, trophies, kt, cl, 0, 0, partyPct));
    }

    // Write to Config File
    ofstream configOut(outputPrefix + ".cfg");
    configOut << "Battles Per Season: " << numBattles << endl;
    configOut << "Number of Players: " << numPlayers << endl;
    configOut << "Number of Trophy Regions: " << numRegions << endl;
    configOut << "Number of Trophy Gates: " << numGates << endl;
    configOut << "Number of Regions reset: " << numRegionsReset << endl;
    configOut << "Reset Base: " << resetBase << endl;
    configOut << "Reset Max: " << resetMax << endl;
    configOut << "Number of Seasons: " << numSeasons << endl;
    configOut << "Trophy Region,  Loss Percent, mmType, Card Level Cap" << endl;
    for (unsigned int i = 0; i < numRegions; ++i){
        configOut << trophyRegions[i] << " "<< lossPcts[i] << " ";
        configOut << mmTypes[i] << " " << clCaps[i] << endl;
    }
    configOut << "Trophy Gates: ";
    for (u_int16_t g : gates)
    {
        configOut << g << " ";
    }
    configOut << endl;
    configOut << "Reset Regions (Trophies, Reset Pct): " << endl;
    for (size_t i = 0; i < numRegionsReset; ++i) {
        configOut << resetRegions[i] << " " << resetPcts[i] << endl;
    }
    configOut << "Players from: " << playerStatsFile << endl;
    configOut << "RNG Seed: " << seed << endl;
    configOut.close();

    unsigned int battles = 0;
    size_t maxQueueSize;

    // Before first season, do a season reset. 
    reset(playerVec, resetPcts, resetRegions, resetBase, resetMax);

    // Tracking battles/second
    auto simStart = chrono::high_resolution_clock::now();

    //Establish RNG
    mt19937 rng(seed);
    uniform_real_distribution<double> doubleDist(0, 1);
    uniform_int_distribution<size_t> playerDist(0, numPlayers - 1);
    // begin simulation
    long long totalBattles = numBattles * numSeasons;
    cout << "Beginning simulation of " << totalBattles << " battles with " << numPlayers << " players" << endl;
    cout << "Number of Seasons: " << numSeasons << endl;
    for (int season = 0; season < numSeasons; ++season)
    {
        battles = 0;
        maxQueueSize = 0;
        cout << "\nBeginning Season " << (season + 1) << endl;
        auto seasonStart = chrono::high_resolution_clock::now();
        TreeQueue queue = TreeQueue(playerVec);
        reset(playerVec, resetPcts, resetRegions, resetBase, resetMax);
        while (battles < numBattles + 10 and queue.size() < numPlayers / 2) {
            if (verbose and (battles % (numBattles / 10) == 0))
            { // runtime checkpoint
                cout << "Battles Completed:" << battles << "\n";
                auto curTime = chrono::high_resolution_clock::now();
                std::chrono::duration<double> timeSoFar = curTime - seasonStart;
                cout << "Average Battles/sec: " << battles / timeSoFar.count() << "\n";
                cout << "Max Queue size: " << maxQueueSize << "\n" << endl;
                ++battles;
            }
            size_t newPlayer = playerDist(rng);
            if (queue.size() > maxQueueSize)
            {
                maxQueueSize = queue.size();
            }
            bool playParty = doubleDist(rng) > playerVec[newPlayer]->getPartyPct();
            if (queue.size() == 0 and !playParty)
            {
                queue.addPlayer(newPlayer, battles);
            }
            else if (!playParty)
            {
                u_int16_t trophies = u_int16_t(playerVec[newPlayer]->getTrophies());
                size_t ind = relevantIndex(trophyRegions, trophies, 0, numRegions);
                mmType = mmTypes[ind];
                size_t opp = queue.findOpponent(newPlayer, mmType);
                if (opp == newPlayer) { // no suitable opponent
                    queue.addPlayer(newPlayer, battles); // log  n
                } else { // play a match
                    size_t gateInd = relevantIndex(gates, trophies, 0, numGates);
                    gate = gates[gateInd];
                    lossPct = lossPcts[ind];
                    clCap = clCaps[ind];
                    double randomVal = doubleDist(rng);
                    long long queueEnter = queue.deletePlayer(opp);
                    long long timeInQueue = battles - queueEnter;
                    playerVec[opp]->updateQueueTime(timeInQueue);
                    playerVec[newPlayer]->playMatch(*playerVec[opp], gate, lossPct, clCap, randomVal);
                    ++battles;
                }
            }
        }
        if (resultsEverySeason and season != numSeasons-1){
            string filename = outputPrefix + ".season" + to_string(season) + ".results";
            resultsToFile(playerVec, filename);
        }
    }
    cout << "Finished Simulation" << endl;
    auto simEnd = chrono::high_resolution_clock::now();
    // finished simulation, write to file.
    std::chrono::duration<double> totalTime = simEnd - simStart;
    resultsToFile(playerVec, outputFile);

    // print interesting statistics
    cout << "\n"<< endl;
    cout << "Total time: " << totalTime.count() << " Seconds" << endl;
    cout << "Battles per Second: " << totalBattles /totalTime.count()  << endl;
    cout << "Maximum queue size: " << maxQueueSize << endl;

    // destuction and deallocation
    for (Player* p : playerVec){
        delete p;
    }
    
    return 0;
}

/**
 * @brief Finds the index associated with a particular value
 * 
 * @tparam T An integer type, the type of values in the vector
 * @param v vector to search through. Vector must be sorted
 * @param element Element to look for the lowest value under
 * @param lower Lower index being searched
 * @param upper Upper index being searched
 * @return size_t Index of greatest element less than element param
 */
template <typename T>
size_t relevantIndex(const vector<T> &v, T element, size_t lower, size_t upper) {
    size_t mid = (upper - lower)/2 + lower;
    if (upper - lower <= 1) {
        return lower;
    }
    else if (v[mid] > element) {
        return relevantIndex(v, element, lower, mid);
    }
    else {
        return relevantIndex(v, element, mid, upper);
    }
}

/**
 * @brief Resets all Players according to their trophies and the reset params. 
 * 
 * @param playerVec Vector of pointers to Player objects that get reset. 
 * @param resetPcts A vector of percentages corresponding to reset Regions
 * @param resetRegions The trophy regions that get reset
 * @param resetBase The base reset point.  Nobody can reset lower than this
 * @param resetMax Maximum value a player can reset to.  (Default is 6600)
 */
void reset(vector<Player*>& playerVec, vector<float>& resetPcts, vector<u_int16_t>& resetRegions, 
             u_int16_t resetBase, u_int16_t resetMax){
    for (Player* p : playerVec){
        size_t ind = relevantIndex(resetRegions, static_cast<u_int16_t>(p->getTrophies()), 0, resetRegions.size());
        float resetPct = resetPcts[ind];
        if (p->getTrophies() > resetBase){
            p->reset(resetBase, resetMax, resetPct); 
        }
    }
}

/**
 * @brief Prints the complier flags associated with this project
 * 
 */
void printCompilerCommands(){
    cout << "Compiler Flags: \n" << endl;
    cout << "-h: Prints this help message" << endl;
    cout << "-o OUTPUT-PREFIX: The prefix of the results file. " << endl;
    cout << "   Ex: -o sim10k => sim10k.results" << endl;
    cout << "   Default: simulation.results" << endl;
    cout << "-v: Whether or not to print verbose progress checks" << endl;
    cout << "   Default: False (Highly recommended for development and optimzation purposes)" << endl;
    cout << "-i INPUT-PREFIX: The settings input file" << endl;
    cout << "   Ex: -i currentSettings.txt => readsfrom file \"currentSettings.txt\" " << endl;
    cout << "   Default: \"settings.txt\" (included in original repo)" << endl;
    cout << "-p FILENAME: The file where initial player data comes from" << endl;
    cout << "   Ex: -p 25kPlayers.txt => players come from file \"25kPlayers.txt\"" << endl;
    cout << "   Default: /ladderSample/25k.txt" << endl;
    cout << "-s: Whether or not to write player data to a file every season" << endl;
    cout << "   Default: False" << endl;
    cout << "-d SEED: Seed for RNG." << endl;
    cout << "    Default: Seed is set as time(0)" << endl;
    cout << "-c CONFIG-FILENAME: Reads parameters from config file" << endl;
    cout << "    Default: Does not read from cfg file" << endl;
}

/**
 * @brief Writes Player stats to file
 * 
 * @param playerVec Vector of Player* of players getting written to file
 * @param filename Name of file player data is sent to
 */
void resultsToFile(vector<Player*>& playerVec, string filename){
    ofstream results(filename);
    for (Player *p : playerVec) {
        short totalGames = p->getWins() + p->getLosses();
        results << p->getTrophies() << " ";
        results << p->getKt() << " ";
        results << p->getCl() << " ";
        results << p->getWins() << " ";
        results << p->getLosses() << " ";
        results << totalGames << " ";
        results << p->getNetLvlAdv() << " ";
        results << (p->getNetLvlAdv() / float(totalGames)) << " "; // explicit conversion
        results << p->getTotalLvlDiff() << " ";
        results << p->getTotalLvlDiff() / float(totalGames) << " ";
        results << p->getPB() << " ";
        results << p->getPartyPct() << " ";
        results << p->getQueueTime() << endl; 
    }
    results.close();
}