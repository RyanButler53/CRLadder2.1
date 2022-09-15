#include "player.hpp"
#include <iostream>

using namespace std;

void testSimpleConstructor() {
    Player p(6000);
    bool t = p.getId() == 6000;
    bool t2 = p.getCl() == 88;
    bool t3 = p.getKt() == 11;
    bool t4 = p.getTrophies() == 5000;
    cout << "Test id " << t << endl;
    cout << "Test cl " << t2 << endl;
    cout << "Test kt " << t3 << endl;
    cout << "Test trophies " << t4 << endl;
}

void testMediumConstructor(){
    Player p(765420, 8946, 14, 112);
    bool t = p.getId() == 765420;
    bool t2 = p.getCl() == 112;
    bool t3 = p.getKt() == 14;
    bool t4 = p.getTrophies() == 8946;
    cout << "Test id " << t << endl;
    cout << "Test cl " << t2 << endl;
    cout << "Test kt " << t3 << endl;
    cout << "Test trophies " << t4 << endl;
}

void testOperators() {
    Player p(65, 7224, 14, 111);
    Player p2(74, 5200, 12, 100);
    Player p3(65);
    bool t = p == p3;
    bool t2 = p != p2;
    cout << "Test == " << t << endl;
    cout << "Test != " << t2 << endl;
}

void testMatchAllowed(){
    Player p(820, 6000, 14, 112);
    Player p2(23, 6040, 12, 110);
    Player p3(2, 6020, 13, 105);
    Player p4(2, 6020, 13, 104); // same id as p3
    Player p5(59, 5950, 14, 108);
    Player p6(57, 5925, 8, 104);
    cout << "Testing matchAllowed" << endl;
    cout << "Expected: 1 " << p.matchAllowed(p2, 2) << endl;
    cout << "Ex: 1 " << p2.matchAllowed(p3, 0) << endl;
    cout << "Ex: 0 " << p.matchAllowed(p3, 1) << endl;
    cout << "Ex: 1 " << p.matchAllowed(p2, 1) << endl;
    cout << "Ex: 0 " << p3.matchAllowed(p4, 0) << endl;
    cout << "Ex: 1 " << p5.matchAllowed(p6, 1) << endl;
    cout << "Ex: 0 " << p6.matchAllowed(p5, 0) << endl;
    cout << "Ex: 0 " << p4.matchAllowed(p5, 2) << endl;
    cout << "Ex: 0 " << p5.matchAllowed(p4, 2) << endl;
}

void testPlayWinMatch(){
    Player p(810, 6000, 14, 112);
    Player p2(814, 6020, 1, 72);
    Player p3(800, 5501, 12, 104);
    Player p4(860, 5476, 13, 75);
    p.playMatch(p2, 5000, 1, 14, 0.7);
    //p almost guarantees winning this match
    cout << p.getTrophies() << "  " << p2.getTrophies() << endl;
    
    p4.winsMatch(p3, 5500, 0.8);
    cout << "Expected: 5500 5508; Actual: ";
    cout << p3.getTrophies() << " " << p4.getTrophies() << endl;
}
void testPrint(){
    Player p(810, 6000, 14, 112);
    cout << "Expected: (14,112, 6000); Actual";
    p.printToStream(std::cout);
    // no overloading << operator, but good enough.
}

int main()
{
    cout << "Begin Player Tests" << endl;
    testSimpleConstructor();
    testMediumConstructor();
    testOperators();
    testMatchAllowed();
    testPlayWinMatch();
    testPrint();

    return 0;
}