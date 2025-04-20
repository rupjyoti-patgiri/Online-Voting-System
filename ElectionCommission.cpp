// ElectionCommission.cpp
#include "ElectionCommission.h"
#include <iostream>
#include <algorithm> // For remove_if, find_if
#include <memory>    // For unique_ptr if managing Election pointer smarter

using namespace std;

// --- ElectionCommission Base Class ---

void ElectionCommission::registerVoter(Voter *v)
{
    // Optional: Check for duplicate Voter ID before adding
    auto it = std::find_if(voters.begin(), voters.end(),
                           [&](const Voter *existingVoter)
                           { return existingVoter->getVoterID() == v->getVoterID(); });
    if (it == voters.end())
    {
        voters.push_back(v);
    }
    else
    {
        cout << "Warning: Voter with ID " << v->getVoterID() << " already exists. Not adding duplicate." << endl;
        delete v; // Avoid memory leak if duplicate is not added
    }
}

void ElectionCommission::registerCandidate(Candidate *c)
{
    // Optional: Check for duplicate Candidate ID before adding
    auto it = std::find_if(candidates.begin(), candidates.end(),
                           [&](const Candidate *existingCand)
                           { return existingCand->getCandidateID() == c->getCandidateID(); });
    if (it == candidates.end())
    {
        candidates.push_back(c);
    }
    else
    {
        cout << "Warning: Candidate with ID " << c->getCandidateID() << " already exists. Not adding duplicate." << endl;
        delete c; // Avoid memory leak
    }
}

void ElectionCommission::listVoters()
{
    cout << "\n--- Registered Voters ---\n";
    if (voters.empty())
    {
        cout << "No voters registered.\n";
        return;
    }
    for (const auto &v : voters)
    {
        cout << "Name: " << v->getName() << ", Age: " << v->getAge()
             << ", ID: " << v->getVoterID() << ", Assembly: " << v->getAssembly()
             << ", Voted: " << (v->voteStatus() ? "Yes" : "No") << endl;
    }
    cout << "-------------------------\n";
}

void ElectionCommission::listCandidates()
{
    cout << "\n--- Registered Candidates ---\n";
    if (candidates.empty())
    {
        cout << "No candidates registered.\n";
        return;
    }
    for (const auto &c : candidates)
    {
        cout << "Name: " << c->getCandidateName() << ", Age: " << c->getAge()
             << ", Party: " << c->getParty() << ", ID: " << c->getCandidateID()
             << ", Assembly: " << c->getAssembly() << endl;
    }
    cout << "---------------------------\n";
}

void ElectionCommission::clearVoters()
{
    for (auto v : voters)
    {
        delete v; // Free memory
    }
    voters.clear(); // Clear the vector
}

void ElectionCommission::clearCandidates()
{
    for (auto c : candidates)
    {
        delete c; // Free memory
    }
    candidates.clear(); // Clear the vector
}

// --- VoterVerification Derived Class ---

// Verify eligibility based on rules (e.g., age)
bool VoterVerification::verifyVoter(const Voter *v) const
{
    if (v == nullptr)
        return false;
    if (v->isEligible())
    {
        // cout << "Voter " << v->getName() << " is eligible.\n"; // Less verbose
        return true;
    }
    cout << "Voter " << v->getName() << " is NOT eligible (Age < 18).\n";
    return false;
}

// Verify eligibility based on rules (e.g., age)
bool VoterVerification::verifyCandidate(const Candidate *c) const
{
    if (c == nullptr)
        return false;
    if (c->isEligible())
    {
        // cout << "Candidate " << c->getCandidateName() << " is eligible.\n"; // Less verbose
        return true;
    }
    cout << "Candidate " << c->getCandidateName() << " is NOT eligible (Age < 25).\n";
    return false;
}

bool VoterVerification::deleteVoter(const string &voterID)
{
    // Use erase-remove idiom correctly
    auto it = std::remove_if(voters.begin(), voters.end(),
                             [&](Voter *v)
                             {
                                 if (v->getVoterID() == voterID)
                                 {
                                     delete v;    // Delete the object
                                     return true; // Indicate removal
                                 }
                                 return false;
                             });

    if (it != voters.end())
    {
        voters.erase(it, voters.end()); // Erase the null pointers left by remove_if
        return true;
    }
    return false; // Not found
}

bool VoterVerification::deleteCandidate(const string &candidateID)
{
    // cout << "Searching for candidate with ID: \"" << candidateID << "\"" << endl; // Debugging

    auto it = std::remove_if(candidates.begin(), candidates.end(),
                             [&](Candidate *c)
                             {
                                 // cout << "Checking candidate with ID: \"" << c->getCandidateID() << "\"" << endl; // Debugging
                                 if (c->getCandidateID() == candidateID)
                                 {
                                     delete c;    // Delete the object
                                     return true; // Indicate removal
                                 }
                                 return false;
                             });

    if (it != candidates.end())
    {
        candidates.erase(it, candidates.end()); // Erase the pointers
        return true;
    }
    // cout << "Candidate not found after loop." << endl; // Debugging
    return false; // Not found
}

// --- ElectionOrganizers Derived Class ---

ElectionOrganizers::~ElectionOrganizers()
{
    delete currentElection; // Clean up the election object when organizers are destroyed
}

// This copies the *pointers* from the source (verifier) to this object's vectors.
// Be mindful of ownership. If verifier clears its lists, these pointers become dangling.
// A better approach might be cloning objects or using shared ownership (smart pointers).
// For this project's structure, this simple pointer copy is maintained.
void ElectionOrganizers::inheritDataFrom(const ElectionCommission &source)
{
    // We don't clear here, we just copy the current state of pointers
    // The AdminCLI controls the main list via VoterVerification.
    this->voters = source.getVoters();
    this->candidates = source.getCandidates();

    // If an election is already running, inheriting new data might be problematic.
    // Consider resetting or warning if currentElection is not null.
    if (currentElection != nullptr)
    {
        cout << "Warning: Inheriting data while an election might be in progress/declared." << endl;
        // Optionally: delete currentElection; currentElection = nullptr;
    }
}

void ElectionOrganizers::startElection(string type, string date, int pollId)
{
    // Clean up any previous election object
    delete currentElection;
    currentElection = nullptr; // Avoid dangling pointer

    if (voters.empty() || candidates.empty())
    {
        cout << "Cannot start election: No registered voters or candidates found.\n";
        cout << "Please register voters and candidates first.\n";
        return;
    }

    cout << "Initializing election with Poll ID: " << pollId << endl;
    currentElection = new Election(type, date, pollId);

    // Add candidates relevant to this election (could be filtered by assembly later if needed)
    for (auto c : candidates)
    {
        // Maybe add checks here if candidates need specific criteria for *this* election
        currentElection->addCandidate(c);
    }

    cout << "Starting voting process...\n";
    // Pass the *current list* of voters from organizers to the election
    currentElection->conductElection(this->voters); // Pass the vector of pointers
    cout << "Voting process finished.\n";

    // After conductElection, the 'hasVoted' status in the Voter objects
    // (pointed to by both verifier and organizers lists) should be updated.
}

void ElectionOrganizers::declareResults()
{
    if (currentElection)
    {
        cout << "Declaring results for Election (Poll ID: " << currentElection->getPollID() << ")..." << endl;
        currentElection->displayResults();
    }
    else
    {
        cout << "No election has been started or conducted yet.\n";
        cout << "Please start an election (Option 7 in Admin Menu) first.\n";
    }
}

// Removed displayVoterToCandidateMapping() implementation