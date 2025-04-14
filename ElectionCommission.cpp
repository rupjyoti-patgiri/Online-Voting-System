// ElectionCommission.cpp
#include "ElectionCommission.h"
#include <iostream>
#include <algorithm> // For remove_if
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
    for (const auto &v : voters)
    {
        cout << "Voter: " << v->getName() << " - ID: " << v->getVoterID() << endl;
    }
}

void ElectionCommission::listCandidates()
{
    for (const auto &c : candidates)
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

bool VoterVerification::deleteVoter(const string &voterID)
{
    auto it = remove_if(voters.begin(), voters.end(),
                        [&](Voter *v)
                        { return v->getVoterID() == voterID; });
    if (it != voters.end())
    {
        delete *it;
        voters.erase(it, voters.end());
        return true;
    }
    return false;
}

bool VoterVerification::deleteCandidate(const string &candidateName)
{
    auto it = remove_if(candidates.begin(), candidates.end(),
                        [&](Candidate *c)
                        { return c->getCandidateName() == candidateName; });
    if (it != candidates.end())
    {
        delete *it;
        candidates.erase(it, candidates.end());
        return true;
    }
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

void ElectionOrganizers::inheritDataFrom(const VoterVerification &verifier)
{
    this->voters.clear(); // Ensure no duplicate data
    for (const auto &v : verifier.getVoters())
    {
        this->voters.push_back(new Voter(*v)); // Create copies
    }
    this->candidates.clear(); // Ensure no duplicate data
    for (const auto &c : verifier.getCandidates())
    {
        this->candidates.push_back(new Candidate(*c)); // Create copies
    }
}

void ElectionOrganizers::startElection(string type, string date, int pollId)
{
    election = new Election(type, date, pollId);
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
    else
        cout << "No election has been started yet.\n";
}

void ElectionOrganizers::displayVoterToCandidateMapping() const
{
    if (election)
        election->displayVoterToCandidateMapping();
    else
        cout << "No election has been started yet to display voter mapping.\n";
}