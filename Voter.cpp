// Voter.cpp
#include "Voter.h"
#include <bits/stdc++.h>

using namespace std;

Voter::Voter(string name_val, int age_val, string id_val, string pin_val, string assembly_val, bool initialVoteStatus)
    : name(name_val), age(age_val), voterID(id_val), privatePin(pin_val),
      hasVoted(initialVoteStatus), assembly(assembly_val)
{
    // Constructor body
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
