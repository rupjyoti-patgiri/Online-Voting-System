// AdminCLI.h
#ifndef ADMINCLI_H
#define ADMINCLI_H

#include "ElectionCommission.h"
#include <string>
#include <vector> // Include for vector used in generateUniquePin declaration if moved here

class AdminCLI
{
private:
    VoterVerification voterVerifier;
    ElectionOrganizers organizers;

    // --- Helper for PIN Generation ---
    std::string generateUniquePin(const std::vector<Voter *> &existingVoters);

    // --- Admin Flow ---
    bool login();
    void showMenu();

    // --- Data Management ---
    void registerVoter();
    void deleteVoter();
    void registerCandidate();
    void deleteCandidate();
    void loadAllData();
    void saveAllData();
    void resetSystem();

    // --- Reporting ---
    void listAlreadyVoted();

    // --- Security ---
    std::string resetPassword = "admin_reset_pass"; // Password for system reset

public:
    void run();
};

#endif // ADMINCLI_H