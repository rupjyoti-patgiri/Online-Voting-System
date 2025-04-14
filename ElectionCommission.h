// ElectionCommission.h
#ifndef ELECTIONCOMMISSION_H
#define ELECTIONCOMMISSION_H

#include <vector>
#include <string> // For string
#include "Voter.h"
#include "Candidate.h"
#include "Election.h"

class ElectionCommission
{
protected:
    std::vector<Voter *> voters;
    std::vector<Candidate *> candidates;
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
    bool deleteVoter(const std::string &voterID);           // New
    bool deleteCandidate(const std::string &candidateID); // New
    void clearVoters();
    void clearCandidates();

    // Assuming VoterVerification inherits from ElectionCommission
    std::vector<Voter *> getVoters() const
    {
        return voters;
    }

    std::vector<Candidate *> getCandidates() const
    {
        return candidates;
    }
};

class ElectionOrganizers : public ElectionCommission
{
public:
    void inheritDataFrom(const VoterVerification &verifier);
    void startElection(std::string type, std::string date, int pollId); // Updated
    void declareResults();
    void displayVoterToCandidateMapping() const; // New
};

#endif