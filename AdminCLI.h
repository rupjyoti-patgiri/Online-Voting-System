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
    void registerCandidate();
    void loadAllData();
    void saveAllData();
    void resetSystem();

public:
    void run();
};

#endif
