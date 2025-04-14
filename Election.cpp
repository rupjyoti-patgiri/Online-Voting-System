// Election.cpp
#include "Election.h"
#include <iostream>
#include <limits>
#include <unordered_map>
using namespace std;

Election::Election(string type, string date, int pollId) : pollID(pollId)
{
    this->electionType = type;
    this->date = date;
}

void Election::addCandidate(Candidate *c)
{
    candidates.push_back(c);
}

void Election::conductElection(vector<Voter *> &voters)
{
    unordered_map<string, Voter *> voterMap;
    for (auto &v : voters)
    {
        voterMap[v->getVoterID()] = v;
    }

    int currentPollID;
    cout << "Enter Poll ID to proceed with voting: ";
    cin >> currentPollID;

    if (currentPollID != pollID)
    {
        cout << "Incorrect Poll ID. Voting cannot proceed.\n";
        return;
    }

    while (true)
    {
        string id, pin; // Added pin
        cout << "\nEnter Voter ID to vote (or type 'exit' to end voting): ";
        cin >> id;

        if (id == "exit")
            break;

        if (voterMap.find(id) == voterMap.end())
        {
            cout << "Invalid Voter ID.\n";
            continue;
        }

        Voter *voter = voterMap[id];

        cout << "Enter your Private PIN: "; // Added pin prompt
        cin >> pin;

        if (pin != voter->getPrivatePin()) // Verify pin
        {
            cout << "Incorrect PIN.\n";
            continue;
        }

        if (!voter->isEligible())
        {
            cout << "You are not eligible to vote.\n";
            continue;
        }
        if (voter->voteStatus())
        {
            cout << "You have already voted.\n";
            continue;
        }

        cout << "\nWelcome, " << voter->getName() << "! Please cast your vote:\n";
        for (int i = 0; i < candidates.size(); ++i)
        {
            cout << i + 1 << ". " << candidates[i]->getCandidateName()
                 << " (" << candidates[i]->getParty() << ")\n";
        }

        int choice;
        cout << "Enter choice (number): ";
        cin >> choice;

        if (choice >= 1 && choice <= candidates.size())
        {
            candidates[choice - 1]->vote();
            voter->castVote();
            voterToCandidate[voter->getVoterID()] = candidates[choice - 1]->getCandidateName(); // Record vote
            cout << "Vote cast successfully.\n";
        }
        else
        {
            cout << "Invalid choice. Vote not recorded.\n";
        }
    }
}

void Election::displayResults()
{
    cout << "\n--- Election Results ---\n";
    unordered_map<string, int> partyVotes;
    Candidate *winner = nullptr;
    int maxVotes = -1;
    for (auto &c : candidates)
    {
        cout << c->getCandidateName() << " (" << c->getParty() << ") - " << c->getVotes() << " votes\n";
        partyVotes[c->getParty()] += c->getVotes();
        if (c->getVotes() > maxVotes)
        {
            maxVotes = c->getVotes();
            winner = c;
        }
    }

    cout << "\n--- Party-wise Results ---\n";
    for (const auto &pair : partyVotes)
    {
        cout << pair.first << ": " << pair.second << " votes\n";
    }

    if (winner)
    {
        cout << "\nWinner: " << winner->getCandidateName() << " from " << winner->getParty() << " with " << maxVotes << " votes.\n";
    }
    else
    {
        cout << "No winner.\n";
    }
}

void Election::displayVoterToCandidateMapping() const
{
    for (const auto &pair : voterToCandidate)
    {
        cout << "Voter ID: " << pair.first << " voted for: " << pair.second << "\n";
    }
}