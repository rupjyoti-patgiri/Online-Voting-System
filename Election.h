// Election.h
#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
// Removed #include <map> - no longer needed
#include "Candidate.h"
#include "Voter.h"

// using namespace std; // Avoid 'using namespace std' in header files

class Election
{
private:
    std::string electionType;
    std::string date;
    int pollID;
    std::vector<Candidate *> candidatesInElection; // Candidates participating in this specific election
    // Removed map<string, string> voterToCandidate;

public:
    Election(std::string type, std::string date, int pollId);
    int getPollID() const { return pollID; }
    void addCandidate(Candidate *c);
    // Takes a reference to the vector of voter pointers
    void conductElection(std::vector<Voter *> &allVoters);
    void displayResults();
    // Removed displayVoterToCandidateMapping() declaration
};

#endif // ELECTION_H