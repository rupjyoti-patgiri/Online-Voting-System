#include "Voter.h"

Voter::Voter(string name, int age, string id)
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
