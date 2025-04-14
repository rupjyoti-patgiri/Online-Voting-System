// Candidate.h
#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "PartyName.h"
#include <string>

class Candidate : public PartyName
{
private:
    std::string candidateName;
    int age;
    std::string qualification;
    int voteCount;
    std::string candidateID; // Changed to string
    std::string assembly;

public:
    Candidate(std::string name, int age, std::string qual, std::string party, std::string id, std::string assembly = ""); // Updated constructor
    bool isEligible() const;
    void vote();
    int getVotes() const;
    std::string getCandidateName() const;
    std::string getParty() const;
    std::string getQualification() const;
    int getAge() const;
    std::string getCandidateID() const; // Updated getter
    std::string getAssembly() const;
};

#endif