// ElectionCommission.h
#ifndef ELECTIONCOMMISSION_H
#define ELECTIONCOMMISSION_H

#include <bits/stdc++.h>
using namespace std;

#include "Voter.h"
#include "Party.h"
#include "Election.h"

class ElectionCommission
{
protected:
    vector<Voter *> voters;
    vector<Party *> registeredParties;

public:
    virtual ~ElectionCommission();

    // Voter Management
    void registerVoter(Voter *v);
    void listVoters();
    const vector<Voter *> &getVoters() const;
    vector<Voter *> &getVotersNonConst();
    void clearVoters();

    // Party Management
    virtual void addParty(Party *p);
    virtual Party *findPartyByID(const string &id) const;
    virtual Party *findPartyByName(const string &name) const;
    virtual const vector<Party *> &getParties() const;
    virtual void clearParties();

    // Candidate Management
    virtual void listCandidatesGroupedByParty();
    virtual vector<Candidate *> getAllCandidates() const;
};

class VoterVerification : public ElectionCommission
{
public:
    bool verifyVoter(const Voter *v) const;
    bool verifyCandidate(const Candidate *c) const;
    bool deleteVoter(const string &voterID);
    bool deleteCandidate(const string &candidateID);
};

class ElectionOrganizers : public ElectionCommission
{
private:
    Election *currentElection = nullptr;

public:
    ~ElectionOrganizers();
    void inheritDataFrom(const ElectionCommission &source);
    void startElection(string type, string date, int pollId);
    void declareResults();
};

#endif // ELECTIONCOMMISSION_H
