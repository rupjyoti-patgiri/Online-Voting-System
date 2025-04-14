#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
#include "Candidate.h"
#include "Voter.h"

using namespace std;

class Election
{
private:
    string electionType;
    string date;
    vector<Candidate *> candidates;

public:
    Election(string type, string date);
    void addCandidate(Candidate *c);
    void conductElection(vector<Voter *> &voters);
    void displayResults();
};

#endif
