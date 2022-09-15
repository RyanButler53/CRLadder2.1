#include "treequeue.hpp"
#include <iostream>
#include <ostream>
#include <fstream>

using namespace std;

void testConstructor(vector<Player*> &playerArr){
    TreeQueue t = TreeQueue(playerArr);
    cout << "Ex: 0; Ac: " << t.size() << endl;
}

void testAdd(vector<Player*> &playerArr){
    TreeQueue t = TreeQueue(playerArr);
    t.addPlayer(5,0);
    t.addPlayer(6,0);
    cout << "Ex: 2; Ac:" << t.size() << endl;
    t.addPlayer(5,0);
    cout << "Ex: 2; Ac:" << t.size() << endl;
    bool in5 = t.exists(5);
    bool in6 = t.exists(6);
    bool in7 = t.exists(7);
    // 1 1 0
    cout << "Expected: 1 1 0;";
    cout << "Actual:" << in5 << " " << in6 << " " << in7 << endl;
}

void testFindOpponent1(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);
    for (size_t i = 0; i < 9; ++i){
        t.addPlayer(i,0);
    }
    size_t opp = t.findOpponent(9, 2);
    cout << "Expected: 9; Actual: "<< opp << endl;
}

void testFindOpponent2(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);
    t.addPlayer(1,0);
    t.addPlayer(2,0);
    t.addPlayer(0,0);
    t.addPlayer(5,0);
    size_t opp = t.findOpponent(3, 2);
    cout << "Ex: 2; Ac:" << opp << endl;
    size_t opp2 = t.findOpponent(2, 1);
    cout << "Ex: 2; Ac:" << opp2 << endl;
    size_t opp3 = t.findOpponent(7, 2);
    cout << "Ex: 7; Ac:" << opp3 << endl;
}

void testDelete1(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);
    t.addPlayer(0,0);
    t.addPlayer(2,0);
    t.addPlayer(8,0);
    t.deletePlayer(8);
}

void testDelete2(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);
    cout << "Test 1 child delete" << endl;
    t.addPlayer(8,0);
    long long time = t.deletePlayer(4);
    cout << "deleted 4" << endl;
    time = t.deletePlayer(0);
    cout << "delete 0" << endl;
    t.deletePlayer(8);
    cout << "Ex: 0; Ac: " << time << endl;
    cout << "Ex: 0; Ac: " << t.size() << endl;
    cout << "Ex: 0; Ac: " << t.exists(1) << endl;
    cout << "Ex: 0; Ac: " << t.exists(4) << endl;

}

void testDelete3(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);

    cout << "Testing left 1 child delete" << endl;
    t.addPlayer(0,0);
    t.addPlayer(1,0);
    t.addPlayer(3,0);
    t.addPlayer(5,0);
    t.addPlayer(7,0);

    cout << "Ex: 5; Ac: " << t.size() << endl;
    t.deletePlayer(1);
    cout << "Ex: 4; Ac: " << t.size() << endl;
    t.deletePlayer(7);
    cout << "Ex: 3; Ac: " << t.size() << endl;
    cout << "Ex: 0; Ac: " << t.exists(1) << endl;
    cout << "Ex: 0; Ac: " << t.exists(7) << endl;
    t.deletePlayer(7);
    t.deletePlayer(5);
    t.deletePlayer(3);  
}

void miniSim(vector<Player*>& playerArr){
    //mini simulation

    TreeQueue t = TreeQueue(playerArr);
    size_t opp;
    size_t newP;
    int numBattles = 0;
    mt19937 rng(70);
    uniform_int_distribution<size_t> pdist(0, 149);
    uniform_real_distribution<double> doubleDist(0, 1);
    while (numBattles < 4000 and t.size() < 75)
    {
        newP = pdist(rng);
        opp = t.findOpponent(newP, 2); // unrestricted mm rules
        
        if (opp != newP) { // found an opponent.  Remove opp and play match
            double rval = doubleDist(rng);
            long long enteredQueue = t.deletePlayer(opp); //delete BEFORE touching tree
            // cout << opp << " entered Queue at "  << enteredQueue << " and exited the queue at " << numBattles <<  endl;
            // cout << "time spend in queue: " << (numBattles - enteredQueue) << endl;
            playerArr[opp]->updateQueueTime(numBattles - enteredQueue);
            playerArr[newP]->playMatch(*playerArr[opp], 4000, 1, 14, rval);

            ++numBattles;
        } else {
            if (!t.exists(newP)){ //new player isn't in the queue. Add it!
                t.addPlayer(newP, numBattles);
            }
        }
    }
    // cout << "max queue reached" << endl;
    cout << t.size() << endl;
    cout << numBattles << endl;
}

void testDelete4(vector<Player*>& playerArr){
    TreeQueue t = TreeQueue(playerArr);
    cout << "Testing right child and 2 child deletes " << endl;
    t.addPlayer(6,0);
    t.addPlayer(0,0);
    t.addPlayer(1,0);
    t.addPlayer(2,0);
    t.addPlayer(3,0);
    t.addPlayer(7,0);
    t.addPlayer(9,0);
    t.deletePlayer(0);
    t.deletePlayer(1);
    t.deletePlayer(6);

    t.addPlayer(6,0);
    t.addPlayer(4,0);
    t.addPlayer(3,0);

    t.deletePlayer(5);
    t.deletePlayer(3);
    t.deletePlayer(1);
    cout << "Ex: 4; Ac: " << t.size() << endl;
}

int main() {
    ifstream playerStats("./ladderSample/0.1scale.txt");
    constexpr size_t numPlayers = 150;
    short kt, cl;
    short trophies;
    float partyPct;
    // double skill;
    vector<Player*> pVec;
    for (size_t i = 0; i < numPlayers; ++i)
    {
        playerStats >> kt;
        playerStats >> trophies;
        playerStats >> partyPct;
        playerStats >> cl;
        // playerStats >> skill;
        pVec.push_back(new Player(i, trophies, kt, cl, 0, 0, partyPct)); 
    }
    playerStats.close();
    // cout << "\n" << "\n" << endl;
    // cout << "Beginning Tests:" << endl;
    // cout << "Testing Constructor (and destructor) \n";
    // testConstructor(pVec);
    // cout << "Testing addPlayer and exists" << endl;
    // testAdd(pVec);
    // cout << "Testing Find opponent"<< endl;
    // testFindOpponent1(pVec);
    // testFindOpponent2(pVec);
    // These tests were largely conducted by drawing the tree on paper
    // and tracing the print statements in the destructor to check.
    cout << "\n" << "Testing Delete" << endl;
    // testDelete1(pVec);
    // testDelete2(pVec);
    // testDelete3(pVec);
    // testDelete4(pVec);
    miniSim(pVec); //Mini simulation.  Tests all cases and uses of the queue tree with a set seed
    // deallocation
    for (Player *p : pVec)
    {
        delete p;
    }
}