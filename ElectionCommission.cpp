// ElectionCommission.cpp
#include "ElectionCommission.h"
#include <bits/stdc++.h>

using namespace std;

// --- ElectionCommission Base Class ---

// Virtual destructor implementation
ElectionCommission::~ElectionCommission()
{
    clearVoters();
    clearParties(); 
}

// --- Voter Management --- 
void ElectionCommission::registerVoter(Voter *v)
{
    auto it = std::find_if(voters.begin(), voters.end(),
                           [&](const Voter *existingVoter)
                           { return v && existingVoter && existingVoter->getVoterID() == v->getVoterID(); });
    if (it == voters.end() && v != nullptr)
    {
        voters.push_back(v);
    }
    else
    {
        if (v)
            cerr << "Warning: Voter with ID " << v->getVoterID() << " already exists or is null. Not adding." << endl;
        delete v;
    }
}

void ElectionCommission::listVoters()
{
    cout << "\n--- Registered Voters ---\n";
    if (voters.empty())
    {
        cout << "No voters registered.\n";
    }
    else
    {
        for (const auto &v : voters)
        {
            if (v)
                cout << "Name: " << v->getName() << ", Age: " << v->getAge()
                     << ", ID: " << v->getVoterID() << ", Assembly: " << v->getAssembly()
                     << ", Voted: " << (v->voteStatus() ? "Yes" : "No") << endl;
        }
    }
    cout << "-------------------------\n";
}

const std::vector<Voter *> &ElectionCommission::getVoters() const { return voters; }
std::vector<Voter *> &ElectionCommission::getVotersNonConst() { return voters; }

void ElectionCommission::clearVoters()
{
    for (auto v : voters)
    {
        delete v;
    }
    voters.clear();
}

// --- Party Management ---
void ElectionCommission::addParty(Party *p)
{
    if (!p)
        return;
    // Check for duplicate Party ID or Name before adding
    bool exists = false;
    for (const auto *existing_p : registeredParties)
    {
        if (existing_p && (existing_p->getID() == p->getID() || existing_p->getName() == p->getName()))
        {
            exists = true;
            break;
        }
    }
    if (!exists)
    {
        registeredParties.push_back(p);
    }
    else
    {
        cerr << "Warning: Party with ID " << p->getID() << " or Name " << p->getName() << " already exists. Not adding duplicate." << endl;
        delete p; // Avoid memory leak
    }
}

Party *ElectionCommission::findPartyByID(const std::string &id) const
{
    for (Party *p : registeredParties)
    {
        if (p && p->getID() == id)
        {
            return p;
        }
    }
    return nullptr; // Not found
}

Party *ElectionCommission::findPartyByName(const std::string &name) const
{
    for (Party *p : registeredParties)
    {
        if (p && p->getName() == name)
        {
            return p;
        }
    }
    return nullptr; // Not found
}

const std::vector<Party *> &ElectionCommission::getParties() const
{
    return registeredParties;
}

void ElectionCommission::clearParties()
{
    // Destructor of Party handles deleting candidates within it
    for (auto p : registeredParties)
    {
        delete p; // Delete the party object itself
    }
    registeredParties.clear(); // Clear the vector of pointers
}

// --- Candidate Management (Grouped Listing) ---
void ElectionCommission::listCandidatesGroupedByParty()
{
    cout << "\n--- Registered Candidates by Party ---\n";
    if (registeredParties.empty())
    {
        cout << "No parties (and thus no candidates) registered.\n";
        return;
    }
    for (const auto *party : registeredParties)
    {
        if (party)
        {
            party->displayPartyInfo(); // Use Party's display method
            cout << "-------------------------------------\n";
        }
    }
}

// --- Utility ---
std::vector<Candidate *> ElectionCommission::getAllCandidates() const
{
    std::vector<Candidate *> allCandidates;
    for (const auto *party : registeredParties)
    {
        if (party)
        {
            const auto &partyCandidates = party->getCandidates();
            allCandidates.insert(allCandidates.end(), partyCandidates.begin(), partyCandidates.end());
        }
    }
    return allCandidates;
}

// --- VoterVerification Derived Class ---

bool VoterVerification::verifyVoter(const Voter *v) const
{ // Unchanged
    if (v == nullptr)
        return false;
    if (v->isEligible())
        return true;
    cout << "Voter " << v->getName() << " is NOT eligible (Age < 18).\n";
    return false;
}

bool VoterVerification::verifyCandidate(const Candidate *c) const
{ // Unchanged
    if (c == nullptr)
        return false;
    if (c->isEligible())
        return true;
    cout << "Candidate " << c->getCandidateName() << " is NOT eligible (Age < 25).\n";
    return false;
}

bool VoterVerification::deleteVoter(const string &voterID)
{ // Unchanged
    auto it = std::remove_if(voters.begin(), voters.end(),
                             [&](Voter *v)
                             { if (v && v->getVoterID() == voterID) { delete v; return true; } return false; });
    if (it != voters.end())
    {
        voters.erase(it, voters.end());
        return true;
    }
    return false;
}


bool VoterVerification::deleteCandidate(const std::string &candidateID)
{
    cout << "Searching for Candidate ID: " << candidateID << " across all parties..." << endl;
    for (Party *party : registeredParties)
    { 
        if (party && party->removeCandidate(candidateID))
        { // Ask party to remove
            cout << "Candidate found and removed from party '" << party->getName() << "'." << endl;
            return true; // Found and removed
        }
    }
    cout << "Candidate ID " << candidateID << " not found in any party." << endl;
    return false; // Not found
}

// --- ElectionOrganizers Derived Class ---

ElectionOrganizers::~ElectionOrganizers()
{ // Unchanged
    delete currentElection;
}


void ElectionOrganizers::inheritDataFrom(const ElectionCommission &source)
{
    // Copy pointers - assumes source (Verifier) manages the lifetime
    this->voters = source.getVoters();
    this->registeredParties = source.getParties(); // Copy party list pointers

    if (currentElection != nullptr)
    {
        cout << "Warning: Inheriting data while an election might be in progress/declared." << endl;
    }
}

void ElectionOrganizers::startElection(string type, string date, int pollId)
{
    delete currentElection;
    currentElection = nullptr;

    // Get *all* candidates from the party structure
    std::vector<Candidate *> allCandidatesForElection = getAllCandidates();

    if (voters.empty() || allCandidatesForElection.empty())
    {
        cout << "Cannot start election: No registered voters or no candidates found in any party.\n";
        cout << "Please register voters, parties, and candidates first.\n";
        return;
    }

    cout << "Initializing election with Poll ID: " << pollId << endl;
    currentElection = new Election(type, date, pollId);

    // Add all available candidates to this election
    cout << "Adding " << allCandidatesForElection.size() << " candidates to the election..." << endl;
    for (auto c : allCandidatesForElection)
    {
        if (c)
        { 
            currentElection->addCandidate(c);
        }
    }

    cout << "Starting voting process...\n";
    currentElection->conductElection(this->voters);
    cout << "Voting process finished.\n";
}

void ElectionOrganizers::declareResults()
{ // Unchanged
    if (currentElection)
    {
        cout << "Declaring results for Election (Poll ID: " << currentElection->getPollID() << ")..." << endl;
        currentElection->displayResults();
    }
    else
    {
        cout << "No election has been started or conducted yet.\n";
        cout << "Please start an election first.\n";
    }
}