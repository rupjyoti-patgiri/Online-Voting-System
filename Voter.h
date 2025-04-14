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
    bool hasVoted;

public:
    Voter(string name, int age, string id);
    bool isEligible() const;
    bool voteStatus() const;
    void castVote();
    string getVoterID() const;
    string getName() const;
};

#endif
