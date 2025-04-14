#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "PartyName.h"

class Candidate : public PartyName
{
private:
    string candidateName;
    int age;
    string qualification;
    int voteCount;

public:
    Candidate(string name, int age, string qual, string party);
    bool isEligible() const;
    void vote();
    int getVotes() const;
    string getCandidateName() const;
    string getParty() const;
};

#endif
