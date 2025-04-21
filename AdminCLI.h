// AdminCLI.h
#ifndef ADMINCLI_H
#define ADMINCLI_H

#include "ElectionCommission.h" // Includes Party.h via ElectionCommission.h
#include <bits/stdc++.h>
using namespace std;

class AdminCLI
{
private:
    VoterVerification voterVerifier; // Manages voters and parties/candidates
    ElectionOrganizers organizers;

    // --- Helper ---
    string generateUniquePin(const vector<Voter *> &existingVoters);
    Party *ensureIndependentPartyExists(); // Helper to get/create Independent party

    // --- Admin Flow ---
    bool login();
    void showMenu();

    // --- Data Management ---
    void registerVoter();
    void deleteVoter();
    void registerParty();     
    void registerCandidate(); 
    void deleteCandidate();   
    void loadAllData();       
    void saveAllData();       
    void resetSystem();       

    // --- Reporting ---
    void listVoters();               
    void listPartiesAndCandidates(); 
    void listAlreadyVoted();

    // --- Security ---
    string resetPassword = "admin_reset_pass";

public:
    void run();
};

#endif // ADMINCLI_H
