// ElectionCommission.h
#ifndef ELECTIONCOMMISSION_H
#define ELECTIONCOMMISSION_H

#include <vector>
#include <string>
#include "Voter.h"
#include "Candidate.h"
#include "Election.h" // Include Election.h here

// Forward declare Election if needed, but including is generally okay here
// class Election;

class ElectionCommission
{
protected:
    std::vector<Voter *> voters;
    std::vector<Candidate *> candidates;
    // Removed Election *election; - Let Organizers manage the election object

public:
    virtual ~ElectionCommission() = default; // Add virtual destructor for base class

    void registerVoter(Voter *v);
    void registerCandidate(Candidate *c);
    void listVoters();
    void listCandidates();

    // Added const versions of getters for safety
    const std::vector<Voter *> &getVoters() const { return voters; }
    const std::vector<Candidate *> &getCandidates() const { return candidates; }

    // Provide non-const access for derived classes that need to modify
    std::vector<Voter *> &getVotersNonConst() { return voters; }
    std::vector<Candidate *> &getCandidatesNonConst() { return candidates; }

    // Methods to clear data, potentially useful in base or derived
    void clearVoters();
    void clearCandidates();
};

class VoterVerification : public ElectionCommission
{
public:
    // Verification now primarily checks eligibility rules
    bool verifyVoter(const Voter *v) const;         // Made const correct
    bool verifyCandidate(const Candidate *c) const; // Made const correct

    // Deletion remains responsibility of verification/admin side
    bool deleteVoter(const std::string &voterID);
    bool deleteCandidate(const std::string &candidateID);

    // clearVoters/clearCandidates inherited is sufficient
};

class ElectionOrganizers : public ElectionCommission
{
private:
    Election *currentElection = nullptr; // Organizers own the specific election instance

public:
    ~ElectionOrganizers(); // Need destructor to clean up currentElection

    // Inherit data *copies pointers* - careful with ownership if base clears
    void inheritDataFrom(const ElectionCommission &source); // Take base class ref

    void startElection(std::string type, std::string date, int pollId);
    void declareResults();
    // Removed displayVoterToCandidateMapping() declaration
};

#endif // ELECTIONCOMMISSION_H