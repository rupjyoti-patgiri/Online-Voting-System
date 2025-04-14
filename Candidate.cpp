// Candidate.cpp
#include "Candidate.h"

Candidate::Candidate(string name, int age, string qual, string party, string id, string assembly)
    : candidateID(id), assembly(assembly)
{
    candidateName = name;
    this->age = age;
    qualification = qual;
    partyName = party;
    voteCount = 0;
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

string Candidate::getCandidateID() const // Updated getter
{
    return candidateID;
}

string Candidate::getAssembly() const
{
    return assembly;
}