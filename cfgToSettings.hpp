#ifndef CFGTOSETTINGS_HPP_INCLUDED
#define CFGTOSETTINGS_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <tuple>

//Function Declarations:

/**
 * @brief Eats up skip words from config file
 * 
 * @param configIn oStream for reading in a cfg file
 * @param skip Number of words to skip
 */
void skip(std::ifstream &configIn, int skip);

/**
 * @brief 
 * 
 * @param cfgFilename Config file to read from 
 * @param settingsFile Setting file to output. 
 * @return std::tuple<std::string, std::string>  PlayerData filename, rng seed
 */
std::tuple<std::string, long long> readFromCfg(std::string cfgFilename);

#endif