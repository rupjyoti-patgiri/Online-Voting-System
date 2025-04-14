// Voter.h
#ifndef VOTER_H
#define VOTER_H

#include <string>
using namespace std;

class Voter
{
private:
    string name;
    int age;
    string voterID;
    string privatePin; // New
    bool hasVoted;

public:
    Voter(string name, int age, string id, string pin); // Updated constructor
    bool isEligible() const;
    bool voteStatus() const;
    void castVote();
    string getVoterID() const;
    string getName() const;
    int getAge() const;           // New getter
    string getPrivatePin() const; // New getter
};

#endif