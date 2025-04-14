// Election.h
#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
#include <map> // For voter to candidate mapping
#include "Candidate.h"
#include "Voter.h"

using namespace std;

class Election
{
private:
    string electionType;
    string date;
    int pollID; // New
    vector<Candidate *> candidates;
    map<string, string> voterToCandidate; // VoterID -> CandidateName (New)

public:
    Election(string type, string date, int pollId); // Updated constructor
    int getPollID() const { return pollID; }        // New getter
    void addCandidate(Candidate *c);
    void conductElection(vector<Voter *> &voters);
    void displayResults();
    void displayVoterToCandidateMapping() const; // New
};

#endif