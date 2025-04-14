// Voter.cpp
#include "Voter.h"

Voter::Voter(string name, int age, string id, string pin, string assembly)
    : privatePin(pin), assembly(assembly)
{
    this->name = name;
    this->age = age;
    this->voterID = id;
    this->hasVoted = false;
}

bool Voter::isEligible() const
{
    return age >= 18;
}

bool Voter::voteStatus() const
{
    return hasVoted;
}

void Voter::castVote()
{
    hasVoted = true;
}

string Voter::getVoterID() const
{
    return voterID;
}

string Voter::getName() const
{
    return name;
}

int Voter::getAge() const
{
    return age;
}

string Voter::getPrivatePin() const
{
    return privatePin;
}

string Voter::getAssembly() const
{
    return assembly;
}