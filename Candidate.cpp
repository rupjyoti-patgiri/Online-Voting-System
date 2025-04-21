// Candidate.cpp
#include "Candidate.h"
#include <bits/stdc++.h>
using namespace std;

Candidate::Candidate(string name, int age, string qual, string party, string id, string assembly_val)
    : candidateName(name), age(age), qualification(qual), partyName(party),
      candidateID(id), assembly(assembly_val), voteCount(0)
{
    
}

bool Candidate::isEligible() const
{
    return age >= 25;
}

void Candidate::vote()
{
    voteCount++;
}

int Candidate::getVotes() const
{
    return voteCount;
}

string Candidate::getCandidateName() const
{
    return candidateName;
}

string Candidate::getParty() const
{
    return partyName;
}

string Candidate::getQualification() const
{
    return qualification;
}

int Candidate::getAge() const
{
    return age;
}

string Candidate::getCandidateID() const
{
    return candidateID;
}

string Candidate::getAssembly() const
{
    return assembly;
}

// Optional Setter implementation if added to header
// void Candidate::setPartyName(const string& pName) {
//     this->partyName = pName;
// }
