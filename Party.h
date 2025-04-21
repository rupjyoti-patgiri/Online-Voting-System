// Party.h
#ifndef PARTY_H
#define PARTY_H

#include <bits/stdc++.h>
#include "Candidate.h"

using namespace std;

class Party
{
private:
    string partyID;
    string partyName;
    string symbol;
    vector<Candidate *> candidates;

public:
    // Constructor
    Party(string id, string name, string sym);

    // Destructor to clean up candidates
    ~Party();

    // Getters
    string getID() const;
    string getName() const;
    string getSymbol() const;
    const vector<Candidate *> &getCandidates() const;

    // Modifiers
    void addCandidate(Candidate *c);
    bool removeCandidate(const string &candidateID);

    // Find candidate within this party
    Candidate *findCandidateByID(const string &candidateID);

    // Display (optional utility)
    void displayPartyInfo() const;
};

// Define constants for the Independent Party
const string INDEPENDENT_PARTY_ID = "IND";
const string INDEPENDENT_PARTY_NAME = "Independent";
const string INDEPENDENT_PARTY_SYMBOL = "N/A";

#endif // PARTY_H
