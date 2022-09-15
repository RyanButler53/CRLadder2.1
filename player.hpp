/**
 * @file player.hpp
 * @author Nova Light (keymckeyface@gmail.com)
 * @brief Player class that supports a model of a CR ladder player
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022. MIT lisence
 * 
 */

#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>
#include <ostream>
#include <random>
#include <algorithm>


class Player
{
private:
    size_t id;
    short trophies;
    short kingTower;
    short cardLevel;
    short wins;
    short losses;
    short pb;
    bool inQueue;
    int totalLvlDiff;
    int netLvlAdv; // net level ADVANTAGE
    float partyPct;
    long long timeInQueue;


public:
    // constructors and destructors
    Player() = delete; // default constructor
    Player(size_t id);// simple "default" constructor
    //more useful constructor
    Player(size_t id, short trophies, short kingTower, short cardLevel);
    //all arguments, for importing players from file
    Player(size_t id, short trophies, short kingTower, short cardLevel,
           short wins, short losses, float partyPct);
    Player(const Player& other) = delete; // copy constructor
    ~Player() = default;
    Player &operator=(const Player &other);

    // methods

    // Getters
    short getTrophies();
    size_t getId();
    short getKt();
    short getCl();
    float getPartyPct();
    bool queueStatus();
    short getWins();
    short getLosses();
    double getPB();
    int getTotalLvlDiff();
    int getNetLvlAdv();
    long long getQueueTime();

    /**
     * @brief Updates a player's time spent in queue
     * 
     * @param newTime How long player spent in the queue
     */
    void updateQueueTime(long long newTime);

    /**
     * @brief Plays a match with another player
     * @param other player to play match against
     */
    void playMatch(Player& other, int16_t gate, float lossPct, int clCap, double randomVal);

    /**
     * @brief updates players when a match is played
     * 
     * @param other the player that *this won against
     * @param gate the relevant trophy gate
     * @param lossPct the loss percent of the trophy region
     */
    void winsMatch(Player& other, short gate, float lossPct);

    /**
     * @brief Checks if a match is allowed to happen
     * 
     * @param other Player to possibly match against
     * @param mmType Type of mm rule: 0 = KT, 1 = CL, 2 = none
     * @return true Match is allowed to happen -> play match
     * @return false Match is not allowed -> add to queue tree
     */
    bool matchAllowed(Player& other, uint16_t mmType);

    /**
     * @brief Changes the queue status of the player. Flips the boolean
     * 
     */
    void changeQueueStatus();

    void reset(short resetBase, short resetMax, float resetPct);

    /**
     * @brief Checks if 2 players are identical
     *
     * @param other player to check against
     * @return true players are equal  (equal ids)
     * @return false players are distinct
     */
    bool operator==(const Player& other) const;

    bool operator!=(const Player& other) const;

    /**
     * @brief prints out the player
     * 
     * @param out ostream to modify
     */
    void printToStream(std::ostream& out) const;
};

// Non Class methods

std::ostream& operator<<(std::ostream& outStream, const Player& p);

#endif