// Voter.h
#ifndef VOTER_H
#define VOTER_H

#include <bits/stdc++.h>

using namespace std;

class Voter
{
private:
    string name;
    int age;
    string voterID;
    string privatePin;
    bool hasVoted; 
    string assembly;

public:
    Voter(string name, int age, string id, string pin, string assembly, bool initialVoteStatus = false);

    bool isEligible() const;
    bool voteStatus() const;
    void castVote();
    string getVoterID() const;
    string getName() const;
    int getAge() const;
    string getPrivatePin() const;
    string getAssembly() const;
};

#endif // VOTER_H
