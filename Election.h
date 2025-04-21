// Election.h
#ifndef ELECTION_H
#define ELECTION_H

#include <bits/stdc++.h>
using namespace std;

#include "Candidate.h"
#include "Voter.h"

class Election
{
private:
    string electionType;
    string date;
    int pollID;
    vector<Candidate *> candidatesInElection;

public:
    Election(string type, string date, int pollId);
    int getPollID() const { return pollID; }
    void addCandidate(Candidate *c);
    void conductElection(vector<Voter *> &allVoters);
    void displayResults();
};

#endif // ELECTION_H
