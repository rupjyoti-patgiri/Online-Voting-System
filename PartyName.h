#ifndef PARTYNAME_H
#define PARTYNAME_H

#include <iostream>
#include <vector>
using namespace std;

class Candidate;

class PartyName
{
protected:
    string partyName;
    string symbol;
    string manifesto;
    vector<Candidate *> candidates;

public:
    PartyName(string name = "", string sym = "", string man = "");
    void addCandidate(Candidate *c);
    string getPartyName() const;
    void displayCandidates() const;
};

#endif
