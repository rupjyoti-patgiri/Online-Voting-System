// AdminCLI.h
#ifndef ADMINCLI_H
#define ADMINCLI_H

#include "ElectionCommission.h"
#include <string>

class AdminCLI
{
private:
    VoterVerification voterVerifier;
    ElectionOrganizers organizers;

    bool login();
    void showMenu();
    void registerVoter();
    void deleteVoter(); // New feature
    void registerCandidate();
    void deleteCandidate(); // New feature
    void loadAllData();
    void saveAllData();
    void resetSystem();
    void listAlreadyVoted();            // New feature
    void showVoterToCandidateMapping(); // New feature

    std::string voterMappingPassword = "admin_secret"; // Password to protect voter mapping

public:
    void run();
};

#endif