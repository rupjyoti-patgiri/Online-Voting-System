#ifndef ELECTIONCOMMISSION_H
#define ELECTIONCOMMISSION_H

#include <vector>
#include "Voter.h"
#include "Candidate.h"
#include "Election.h"

class ElectionCommission
{
protected:
    vector<Voter *> voters;
    vector<Candidate *> candidates;
    Election *election;

public:
    void registerVoter(Voter *v);
    void registerCandidate(Candidate *c);
    void listVoters();
    void listCandidates();
    
};

class VoterVerification : public ElectionCommission
{
public:
    bool verifyVoter(Voter *v);
    bool verifyCandidate(Candidate *c);
    void clearVoters();
    void clearCandidates();

    // Assuming VoterVerification inherits from ElectionCommission
    vector<Voter *> getVoters() const
    {
        return voters;
    }

    vector<Candidate *> getCandidates() const
    {
        return candidates;
    }
};

class ElectionOrganizers : public ElectionCommission
{
public:
    void inheritDataFrom(const VoterVerification &verifier)
    {
        this->voters = verifier.getVoters();
        this->candidates = verifier.getCandidates();
    }

    void startElection(string type, string date);
    void declareResults();
};

#endif
