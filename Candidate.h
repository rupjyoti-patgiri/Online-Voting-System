// Candidate.h
#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <bits/stdc++.h>
using namespace std;


class Candidate
{
private:
    string candidateName;
    int age;
    string qualification;
    string partyName; 
    int voteCount;
    string candidateID; 
    string assembly;

public:
        Candidate(string name, int age, string qual, string party, string id, string assembly = "");

    // Eligibility and Voting methods
    bool isEligible() const;
    void vote();
    int getVotes() const;

    // Getters for candidate details
    string getCandidateName() const;
    string getParty() const; 
    string getQualification() const;
    int getAge() const;
    string getCandidateID() const;
    string getAssembly() const;

    
};

#endif // CANDIDATE_H
