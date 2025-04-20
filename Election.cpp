// Election.cpp
#include "Election.h"
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <iomanip> // Required for std::fixed and std::setprecision

// using namespace std; // Avoid in implementation file unless necessary, prefer std:: prefix

Election::Election(std::string type, std::string date, int pollId) : electionType(type), date(date), pollID(pollId)
{
    // Constructor body
}

void Election::addCandidate(Candidate *c)
{
    if (c)
    {
        candidatesInElection.push_back(c);
    }
}

// Conduct election using the provided list of all registered voters
void Election::conductElection(std::vector<Voter *> &allVoters)
{
    // Create a map for efficient lookup of voters by ID
    std::unordered_map<std::string, Voter *> voterMap;
    for (Voter *v : allVoters)
    {
        if (v)
        {
            voterMap[v->getVoterID()] = v;
        }
    }

    // --- Calculate Total Eligible Voters for Percentage Calculation ---
    int totalEligibleVoters = 0;
    for (const auto &v : allVoters)
    {
        if (v && v->isEligible())
        { // Check for null pointer and eligibility
            totalEligibleVoters++;
        }
    }
    int votesCastThisSession = 0; // Counter for votes in this run
    // --- ---

    int currentPollIDAttempt;
    std::cout << "\n--- Voting Booth (Poll ID: " << this->pollID << ") ---\n";
    if (totalEligibleVoters == 0)
    {
        std::cout << "Warning: No eligible voters found for this election.\n";
        // Election can technically proceed if candidates exist, but turnout will be 0%.
    }
    else
    {
        std::cout << "Total eligible voters for this election: " << totalEligibleVoters << "\n";
    }
    std::cout << "Enter Poll ID again to proceed with voting: ";

    while (!(std::cin >> currentPollIDAttempt))
    {
        std::cout << "Invalid input. Please enter the numeric Poll ID: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (currentPollIDAttempt != this->pollID)
    {
        std::cout << "Incorrect Poll ID entered. Voting cannot proceed for this session.\n";
        return;
    }

    std::cout << "Poll ID Verified. Voting can commence.\n";

    while (true)
    {
        std::string id_attempt, pin_attempt, voterAssembly_attempt;
        std::cout << "\nEnter Voter ID to cast vote (or type 'exit' to end voting session): ";
        std::getline(std::cin, id_attempt);

        if (id_attempt == "exit")
        {
            std::cout << "Exiting voting session.\n";
            break;
        }

        auto map_iter = voterMap.find(id_attempt);
        if (map_iter == voterMap.end())
        {
            std::cout << "Error: Voter ID '" << id_attempt << "' not found in the registry.\n";
            continue;
        }

        Voter *currentVoter = map_iter->second;

        std::cout << "Enter Private PIN for Voter ID " << id_attempt << ": ";
        std::getline(std::cin, pin_attempt);

        if (pin_attempt != currentVoter->getPrivatePin())
        {
            std::cout << "Error: Incorrect PIN entered. Please try again.\n";
            continue;
        }

        // std::cout << "Enter Your Assembly Constituency: ";
        // std::getline(std::cin, voterAssembly_attempt);

        // if (voterAssembly_attempt != currentVoter->getAssembly())
        // {
        //     std::cout << "Error: Entered assembly '" << voterAssembly_attempt
        //               << "' does not match the registered assembly '" << currentVoter->getAssembly()
        //               << "' for this voter.\n";
        //     continue;
        // }

        if (!currentVoter->isEligible())
        {
            std::cout << "Error: Voter " << currentVoter->getName() << " is not eligible to vote (based on age criteria).\n";
            continue;
        }
        if (currentVoter->voteStatus())
        {
            std::cout << "Info: Voter " << currentVoter->getName() << " (ID: " << id_attempt << ") has already cast their vote.\n";
            continue;
        }

        std::cout << "\nWelcome, " << currentVoter->getName() << "!\n";
        std::cout << "Candidates for your assembly (" << currentVoter->getAssembly() << "):\n";

        std::vector<Candidate *> eligibleCandidates;
        int displayIndex = 1;
        for (Candidate *c : candidatesInElection)
        {
            if (c && c->getAssembly() == currentVoter->getAssembly())
            {
                eligibleCandidates.push_back(c);
                std::cout << "  " << displayIndex++ << ". " << c->getCandidateName()
                          << " (" << (c->getParty().empty() ? "Independent" : c->getParty()) << ")\n";
            }
        }

        if (eligibleCandidates.empty())
        {
            std::cout << "No candidates found registered for your assembly (" << currentVoter->getAssembly() << ") in this election.\n";
            continue;
        }

        int choice = 0;
        std::cout << "Enter the number corresponding to your chosen candidate: ";

        while (!(std::cin >> choice) || choice < 1 || choice > eligibleCandidates.size())
        {
            std::cout << "Invalid choice. Please enter a number between 1 and " << eligibleCandidates.size() << ": ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Candidate *chosenCandidate = eligibleCandidates[choice - 1];
        chosenCandidate->vote();
        currentVoter->castVote(); // Mark voter as voted

        std::cout << "Vote cast successfully for " << chosenCandidate->getCandidateName() << ".\n";
        std::cout << "Thank you for voting, " << currentVoter->getName() << "!\n";

        // --- Calculate and Display Turnout Percentage ---
        votesCastThisSession++;
        if (totalEligibleVoters > 0)
        {
            double percentage = (static_cast<double>(votesCastThisSession) / totalEligibleVoters) * 100.0;
            std::cout << std::fixed << std::setprecision(1); // Format to 1 decimal place
            std::cout << "--> Turnout update: " << votesCastThisSession << " / "
                      << totalEligibleVoters << " eligible voters have voted ("
                      << percentage << "%).\n";
        }
        else
        {
            std::cout << "--> Turnout update: " << votesCastThisSession
                      << " vote(s) cast, but 0 eligible voters were found for this election.\n";
        }
        // --- ---

    } // End of while(true) voting loop
}

// displayResults function remains the same as before...
void Election::displayResults()
{
    std::cout << "\n--- Election Results (" << this->electionType << " - " << this->date << ", Poll ID: " << this->pollID << ") ---\n";

    if (candidatesInElection.empty())
    {
        std::cout << "No candidates participated or no votes were cast in this election.\n";
        return;
    }

    std::cout << "Candidate Vote Counts:\n";
    std::unordered_map<std::string, int> partyVotes; // Track total votes per party
    std::string overallWinnerName = "N/A";
    std::string overallWinnerParty = "N/A";
    int maxVotes = -1;

    for (const auto &c : candidatesInElection)
    {
        if (!c)
            continue; // Skip null pointers if any crept in

        std::string party = c->getParty().empty() ? "Independent" : c->getParty();
        std::cout << "- " << c->getCandidateName() << " (" << party << ")"
                  << " | Assembly: " << c->getAssembly()
                  << " | Votes: " << c->getVotes() << "\n";

        partyVotes[party] += c->getVotes();

        // Determine overall winner (highest votes across all candidates)
        if (c->getVotes() > maxVotes)
        {
            maxVotes = c->getVotes();
            overallWinnerName = c->getCandidateName();
            overallWinnerParty = party;
        }
        // Handle ties? (Currently just shows the first one encountered with max votes)
        else if (c->getVotes() == maxVotes && maxVotes > 0)
        {
            // Indicate a tie for the highest vote count if desired
            // overallWinnerName += " (Tie)"; // Simple tie indication
        }
    }

    std::cout << "\n--- Party-wise Vote Summary ---\n";
    std::string winningParty = "N/A";
    int maxPartyVotes = -1;

    if (partyVotes.empty())
    {
        std::cout << "No votes recorded for any party.\n";
    }
    else
    {
        for (const auto &pair : partyVotes)
        {
            std::cout << "- " << pair.first << ": " << pair.second << " total votes\n";
            if (pair.second > maxPartyVotes)
            {
                maxPartyVotes = pair.second;
                winningParty = pair.first;
            }
            else if (pair.second == maxPartyVotes && maxPartyVotes > 0)
            {
                // Handle party ties if needed
                // winningParty += " (Tie)";
            }
        }
    }

    std::cout << "\n--- Election Summary ---\n";
    if (maxVotes < 0)
    { // Checks if any votes were cast at all
        std::cout << "No votes were cast in this election.\n";
    }
    else if (maxVotes == 0)
    {
        std::cout << "Votes were cast, but the maximum count was zero (or only zero-vote candidates).\n";
        std::cout << "Winning Candidate: None\n";
        std::cout << "Winning Party: None\n";
    }
    else
    {
        std::cout << "Winning Candidate: " << overallWinnerName
                  << " (" << overallWinnerParty << ") with " << maxVotes << " votes.\n";
        if (maxPartyVotes >= 0)
        {
            std::cout << "Party with Most Votes: " << winningParty << " with " << maxPartyVotes << " total votes.\n";
        }
        else
        {
            std::cout << "Party with Most Votes: N/A (No party votes recorded).\n";
        }
    }
    std::cout << "---------------------------\n";
}