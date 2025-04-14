#include "Election.h"
#include <iostream>
#include <limits>
#include <unordered_map>
using namespace std;

Election::Election(string type, string date)
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

    while (true)
    {
        string id;
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
    Candidate *winner = nullptr;
    int maxVotes = -1;
    for (auto &c : candidates)
    {
        cout << c->getCandidateName() << " (" << c->getParty() << ") - " << c->getVotes() << " votes\n";
        if (c->getVotes() > maxVotes)
        {
            maxVotes = c->getVotes();
            winner = c;
        }
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
