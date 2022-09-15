#include "player.hpp"
#include <iostream>
#include <ostream>
#include <algorithm>

using namespace std;

Player::Player(size_t id):
    id{id}, trophies{5000}, kingTower{11}, cardLevel{88},
    wins{0}, losses{0}, pb{5000}, inQueue{false}, totalLvlDiff{0},
    netLvlAdv{0}, partyPct{0.5},  timeInQueue{0}{
        //nothing to do here
    }

Player::Player(size_t id, short trophies, short kingTower, short cardLevel) :
    id{id}, trophies{trophies}, kingTower{kingTower}, cardLevel{cardLevel},
    wins{0}, losses{0}, pb{trophies}, inQueue{false}, totalLvlDiff{0},
    netLvlAdv{0}, partyPct{0.5}, timeInQueue{0}{
        //nothing to do here
    }

Player::Player(size_t id, short trophies, short kingTower, short cardLevel,
           short wins, short losses, float partyPct):
    id{id}, trophies{trophies}, kingTower{kingTower}, cardLevel{cardLevel},
    wins{wins}, losses{losses}, pb{trophies}, inQueue{false}, totalLvlDiff{0},
    netLvlAdv{0}, partyPct{partyPct}, timeInQueue{0}{
    }

short Player::getTrophies(){
    return trophies;
}

size_t Player::getId(){
    return id;
}

short Player:: getKt(){
    return kingTower;
}

short Player::getCl(){
    return cardLevel;
}

float Player::getPartyPct(){
    return partyPct;
}

short Player::getWins(){
    return wins;
}

short Player::getLosses(){
    return losses;
}

double Player::getPB(){
    return pb;
}

int Player::getTotalLvlDiff(){
    return totalLvlDiff;
}

int Player::getNetLvlAdv(){
    return netLvlAdv;
}

long long Player::getQueueTime(){
    return timeInQueue;
}

void Player::updateQueueTime(long long newTime) {
    timeInQueue += newTime;
}
void Player::reset(short resetBase, short resetMax, float resetPct){
    short trophiesLost = resetPct * (trophies - resetBase);
    short newTrophies = trophies - trophiesLost;
    trophies = min(resetMax, newTrophies);
}

bool  Player::operator==(const Player& other) const {
    return id == other.id;
}

bool Player::operator!=(const Player& other) const {
    return !(*this == other);
}


bool Player::matchAllowed(Player& other, u_int16_t mmType){
    // 40 trophies is the maximum trophy difference allowed
    // 1 lvl is max difference for KT mm
    // 4 lvls is max difference for CL mm
    if (*this == other){ //same player
        return false;
    }
    else if (abs(trophies - other.trophies) > 40) //to far apart
    {
        return false;
    }
    else if (mmType == 0 && abs(kingTower - other.kingTower) < 2) //close KT
    {
        return false;
    }
    else if (mmType == 1 && abs(cardLevel - other.cardLevel) > 5) // include 4
    {
        return false;
    }
    return true;
}
void Player::playMatch(Player& other, int16_t gate, float lossPct, int clCap, double randomVal){

    u_int8_t playerLvl = min(cardLevel, short(clCap*8)) + kingTower;
    u_int8_t otherLvl = min(other.cardLevel, short(clCap*8)) + other.kingTower;
    u_int8_t lvlDiff = abs(playerLvl - otherLvl);
    //REDO THIS EQUATION
    float overLvlWins = 0.0186 * lvlDiff + 0.521; 
    //equal levels
    if (lvlDiff == 0){
        short pbDiff = abs(pb - other.pb);
        // calculated from 80k tournament standard battles.
        long double higherPBwins = -0.0000001097213*pbDiff*pbDiff + 0.00030971*pbDiff + 0.48544;
        if ((pb > other.pb and randomVal < higherPBwins) or (pb < other.pb and randomVal > higherPBwins)){ 
            // ^ player is higher pb and higher pb wins^^        ^ player is lower pb and higher pb loses
            winsMatch(other, gate, lossPct);
        } else {
            other.winsMatch(*this, gate, lossPct);
        }
    //player is overleveled
    } else if (cardLevel > other.cardLevel){
        if (randomVal < overLvlWins){
            winsMatch(other, gate, lossPct);
        } else {
            other.winsMatch(*this, gate, lossPct);
        }
    // other is overleveled
    } else { // (cardLevel < other.cardLevel) 
        if (randomVal < overLvlWins){
            other.winsMatch(*this, gate, lossPct);
        } else {
            winsMatch(other, gate, lossPct);
        }
    }
    pb = max(pb, trophies);
    other.pb = max(other.pb, other.trophies);
}
void Player::winsMatch(Player& other, short gate, float lossPct){
    short trophyDiff = abs(other.trophies - trophies);
    short exchanged = 30 + short(trophyDiff/12);
    u_int8_t playerLvl = cardLevel + kingTower;
    u_int8_t otherLvl = other.cardLevel + other.kingTower;
    short lvlDiff = abs(playerLvl - otherLvl);
    trophies += exchanged;
    //temp trophies could be negative
    short tempTrophies = other.trophies - short(lossPct * exchanged);
    other.trophies = max(tempTrophies, gate);
    ++wins;
    ++other.losses;
    totalLvlDiff += lvlDiff;
    other.totalLvlDiff += lvlDiff;
    netLvlAdv += playerLvl - otherLvl;
    other.netLvlAdv += otherLvl - playerLvl;
    return;
}
void Player::changeQueueStatus() {
    if (inQueue){
        inQueue = false;
    } else {
        inQueue = true;
    }
}

bool Player::queueStatus(){
    return inQueue;
}

void Player::printToStream(ostream& out) const {

    out << "(" << kingTower << "," << cardLevel << ", " << trophies <<")" << endl;
}

ostream& operator<<(ostream& outStream, const Player p){
    p.printToStream(outStream);
    return outStream;
}