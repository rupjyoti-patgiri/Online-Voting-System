#include "Election.h"
#include <bits/stdc++.h>
using namespace std;

Election::Election(string type, string date, int pollId) : electionType(type), date(date), pollID(pollId)
{
}

void Election::addCandidate(Candidate *c)
{
    if (c)
    {
        candidatesInElection.push_back(c);
    }
}

void Election::conductElection(vector<Voter *> &allVoters)
{
    unordered_map<string, Voter *> voterMap;
    for (Voter *v : allVoters)
    {
        if (v)
        {
            voterMap[v->getVoterID()] = v;
        }
    }

    int totalEligibleVoters = 0;
    for (const auto &v : allVoters)
    {
        if (v && v->isEligible())
        {
            totalEligibleVoters++;
        }
    }

    int currentPollIDAttempt;
    cout << "\n--- Voting Booth (Poll ID: " << this->pollID << ") ---\n";
    if (totalEligibleVoters == 0)
    {
        cout << "Warning: No eligible voters found for this election.\n";
    }
    else
    {
        cout << "Total eligible voters for this election: " << totalEligibleVoters << "\n";
    }

    int currentTotalVoted = 0;
    for (const auto &v : allVoters)
    {
        if (v && v->isEligible() && v->voteStatus())
        {
            currentTotalVoted++;
        }
    }
    cout << "Currently voted eligible voters (start of session): " << currentTotalVoted << "\n";

    cout << "Enter Poll ID again to proceed with voting: ";
    while (!(cin >> currentPollIDAttempt))
    {
        cout << "Invalid input. Please enter the numeric Poll ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (currentPollIDAttempt != this->pollID)
    {
        cout << "Incorrect Poll ID entered. Voting cannot proceed for this session.\n";
        return;
    }

    cout << "Poll ID Verified. Voting can commence.\n";

    while (true)
    {
        string id_attempt, pin_attempt, voterAssembly_attempt;
        cout << "\nEnter Voter ID to cast vote (or type 'exit' to end voting session): ";
        getline(cin, id_attempt);
        if (id_attempt == "exit")
            break;

        auto map_iter = voterMap.find(id_attempt);
        if (map_iter == voterMap.end())
        {
            cout << "Error: Voter ID '" << id_attempt << "' not found in the registry.\n";
            continue;
        }

        Voter *currentVoter = map_iter->second;

        cout << "Enter Private PIN for Voter ID " << id_attempt << ": ";
        getline(cin, pin_attempt);
        if (pin_attempt != currentVoter->getPrivatePin())
        {
            cout << "Error: Incorrect PIN entered. Please try again.\n";
            continue;
        }

        if (!currentVoter->isEligible())
        {
            cout << "Error: Voter " << currentVoter->getName() << " is not eligible to vote.\n";
            continue;
        }

        if (currentVoter->voteStatus())
        {
            cout << "Info: Voter " << currentVoter->getName() << " has already voted.\n";
            continue;
        }

        cout << "\nWelcome, " << currentVoter->getName() << "!\n";
        cout << "Candidates for your assembly (" << currentVoter->getAssembly() << "):\n";

        vector<Candidate *> eligibleCandidates;
        int displayIndex = 1;
        for (Candidate *c : candidatesInElection)
        {
            if (c && c->getAssembly() == currentVoter->getAssembly())
            {
                eligibleCandidates.push_back(c);
                cout << "  " << displayIndex++ << ". " << c->getCandidateName()
                     << " (" << (c->getParty().empty() ? "Independent" : c->getParty()) << ")\n";
            }
        }

        if (eligibleCandidates.empty())
        {
            cout << "No candidates found for your assembly.\n";
            continue;
        }

        int choice = 0;
        cout << "Enter the number corresponding to your chosen candidate: ";
        while (!(cin >> choice) || choice < 1 || choice > eligibleCandidates.size())
        {
            cout << "Invalid choice. Please enter a number between 1 and " << eligibleCandidates.size() << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Candidate *chosenCandidate = eligibleCandidates[choice - 1];
        chosenCandidate->vote();
        currentVoter->castVote();

        cout << "Vote cast successfully for " << chosenCandidate->getCandidateName() << ".\n";
        cout << "Thank you for voting, " << currentVoter->getName() << "!\n";

        currentTotalVoted = 0;
        for (const auto &v : allVoters)
        {
            if (v && v->isEligible() && v->voteStatus())
            {
                currentTotalVoted++;
            }
        }

        if (totalEligibleVoters > 0)
        {
            double percentage = (static_cast<double>(currentTotalVoted) / totalEligibleVoters) * 100.0;
            cout << fixed << setprecision(1);
            cout << "--> Turnout update: " << currentTotalVoted << " / " << totalEligibleVoters
                 << " eligible voters have voted (" << percentage << "%).\n";
        }
        else
        {
            cout << "--> Turnout update: " << currentTotalVoted
                 << " vote(s) cast, but 0 eligible voters were found.\n";
        }
    }
}

void Election::displayResults()
{
    cout << "\n--- Election Results (" << this->electionType << " - " << this->date << ", Poll ID: " << this->pollID << ") ---\n";

    if (candidatesInElection.empty())
    {
        cout << "No candidates participated or no votes were cast in this election.\n";
        return;
    }

    cout << "Candidate Vote Counts:\n";
    unordered_map<string, int> partyVotes;
    string overallWinnerName = "N/A";
    string overallWinnerParty = "N/A";
    int maxVotes = -1;

    for (const auto &c : candidatesInElection)
    {
        if (!c)
            continue;

        string party = c->getParty().empty() ? "Independent" : c->getParty();
        cout << "- " << c->getCandidateName() << " (" << party << ")"
             << " | Assembly: " << c->getAssembly()
             << " | Votes: " << c->getVotes() << "\n";

        partyVotes[party] += c->getVotes();

        if (c->getVotes() > maxVotes)
        {
            maxVotes = c->getVotes();
            overallWinnerName = c->getCandidateName();
            overallWinnerParty = party;
        }
    }

    cout << "\n--- Party-wise Vote Summary ---\n";
    string winningParty = "N/A";
    int maxPartyVotes = -1;

    for (const auto &entry : partyVotes)
    {
        cout << entry.first << ": " << entry.second << " votes\n";
        if (entry.second > maxPartyVotes)
        {
            maxPartyVotes = entry.second;
            winningParty = entry.first;
        }
    }

    cout << "\nOverall Winner: " << overallWinnerName << " (" << overallWinnerParty << ") with " << maxVotes << " vote(s)\n";
    cout << "Party with most votes: " << winningParty << " (" << maxPartyVotes << " vote(s))\n";
}
