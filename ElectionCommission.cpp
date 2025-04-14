#include "ElectionCommission.h"
#include <iostream>
using namespace std;

void ElectionCommission::registerVoter(Voter *v)
{
    voters.push_back(v);
}

void ElectionCommission::registerCandidate(Candidate *c)
{
    candidates.push_back(c);
}

void ElectionCommission::listVoters()
{
    for (auto v : voters)
    {
        cout << "Voter: " << v->getName() << " - ID: " << v->getVoterID() << endl;
    }
}

void ElectionCommission::listCandidates()
{
    for (auto c : candidates)
    {
        cout << "Candidate: " << c->getCandidateName() << " - Party: " << c->getParty() << endl;
    }
}

bool VoterVerification::verifyVoter(Voter *v)
{
    if (v->isEligible())
    {
        cout << "Voter " << v->getName() << " verified.\n";
        return true;
    }
    cout << "Voter " << v->getName() << " not eligible.\n";
    return false;
}

bool VoterVerification::verifyCandidate(Candidate *c)
{
    if (c->isEligible())
    {
        cout << "Candidate " << c->getCandidateName() << " verified.\n";
        return true;
    }
    cout << "Candidate " << c->getCandidateName() << " not eligible.\n";
    return false;
}

void VoterVerification::clearVoters()
{
    for (auto v : voters)
        delete v;
    voters.clear();
}

void VoterVerification::clearCandidates()
{
    for (auto c : candidates)
        delete c;
    candidates.clear();
}

void ElectionOrganizers::startElection(string type, string date)
{
    election = new Election(type, date);
    for (auto c : candidates)
    {
        election->addCandidate(c);
    }
    election->conductElection(voters);
}

void ElectionOrganizers::declareResults()
{
    if (election)
        election->displayResults();
}
