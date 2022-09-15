#include "cfgToSettings.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <tuple>

void skip(std::ifstream& configIn, int skip){
    std::string placeHolder = "";
    for (int word = 0; word < skip; ++word){
        configIn >> placeHolder;
    }
}

std::tuple<std::string, long long> readFromCfg(std::string cfgFilename){
    std::ifstream cfgFile(cfgFilename);
    std::ofstream settingsFile("temp.settings");
    float floatValue = 0.0;
    size_t intValue = 0;
    int trophyRegions = 0;
    int gates = 0;
    int resets = 0;
    // number of players and battles/season
    for (int j = 0; j < 2; ++j) {
        skip(cfgFile, 3);
        cfgFile >> intValue;
        settingsFile << intValue << std::endl;
    }
    skip(cfgFile, 4);
    cfgFile >> trophyRegions;
    skip(cfgFile, 4);
    cfgFile >> gates;
    skip(cfgFile, 4);
    cfgFile >> resets;
    settingsFile << trophyRegions << std::endl;
    settingsFile << gates << std::endl;
    settingsFile << resets << std::endl;
    for (int j = 0; j < 2; ++j) {
        skip(cfgFile, 2);
        cfgFile >> intValue;
        settingsFile << intValue << " " << std::endl;
    } 
    skip(cfgFile, 3);
    cfgFile >> intValue;
    settingsFile << intValue << " " << std::endl;
    skip(cfgFile, 8);
    for (int region = 0; region < trophyRegions; ++region)
    {
        cfgFile >> intValue;
        settingsFile << intValue << " ";
        cfgFile >> floatValue;
        settingsFile << floatValue << " ";
        for (int i = 0; i < 2; ++i){
            cfgFile >> intValue;
            settingsFile << intValue << " ";
        }
        settingsFile << std::endl;
    }
    skip(cfgFile, 2);

    for (int g = 0; g < gates; ++g){
        cfgFile >> intValue;
        settingsFile << intValue << " ";
    }
    settingsFile << std::endl;
    skip(cfgFile, 5);
    for (int region = 0; region < resets; ++region){
        cfgFile >> intValue;
        cfgFile >> floatValue;
        settingsFile << intValue << " " << floatValue << std::endl;
    }
    skip(cfgFile, 2);
    std::string playerFile;
    long long seed;
    cfgFile >> playerFile;
    // std::cout << "Player file: " << playerFile << std::endl;
    skip(cfgFile, 1);
    cfgFile >> seed;
    cfgFile.close();
    return {playerFile, seed};
}