// AdminCLI.cpp
#include "AdminCLI.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>   // For stringstream
#include <random>    // For PIN generation
#include <string>    // For PIN generation (std::to_string)
#include <iomanip>   // For std::setw, std::setfill (PIN formatting)
#include <algorithm> // For std::any_of (checking PIN uniqueness)

using namespace std;

// --- Helper for PIN Generation ---
string AdminCLI::generateUniquePin(const std::vector<Voter *> &existingVoters)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(100000, 999999); // 6-digit PINs

    string newPinStr;
    bool pinExists;

    do
    {
        int newPin = distrib(gen);
        stringstream ss;
        ss << std::setw(6) << std::setfill('0') << newPin; // Format as 6 digits with leading zeros
        newPinStr = ss.str();

        // Check if the generated PIN already exists
        pinExists = std::any_of(existingVoters.begin(), existingVoters.end(),
                                [&](const Voter *v)
                                { return v->getPrivatePin() == newPinStr; });

    } while (pinExists); // Keep generating until a unique PIN is found

    return newPinStr;
}

// --- Admin Flow ---
bool AdminCLI::login()
{
    string username, password;
    cout << "==== Admin Login ====\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    return (username == "admin" && password == "pass123");
}

// --- Data Management ---
void AdminCLI::loadAllData()
{
    // Load from data/ subdirectory
    ifstream vin("data/voters.txt");
    string line;
    while (getline(vin, line))
    {
        stringstream ss(line);
        string name, id, pin_str, assembly;
        int age;
        // Format: Name|Age|VoterID|PIN|Assembly
        if (getline(ss, name, '|') && (ss >> age) && (ss.peek() == '|') && ss.ignore() && /* Ignore the '|' after age */
            getline(ss, id, '|') && getline(ss, pin_str, '|') && getline(ss, assembly))
        {
            Voter *v = new Voter(name, age, id, pin_str, assembly);
            voterVerifier.registerVoter(v); // Directly register, verification happens logically before saving
        }
        else
        {
            cerr << "Warning: Skipping malformed line in voters.txt: " << line << endl;
        }
    }
    vin.close();

    // Load from data/ subdirectory
    ifstream cinFile("data/candidates.txt");
    while (getline(cinFile, line))
    {
        stringstream ss(line);
        string name, party, qual, id_str, assembly;
        int age;
        // Format: Name|Age|Qualification|Party|CandidateID|Assembly
        if (getline(ss, name, '|') && (ss >> age) && (ss.peek() == '|') && ss.ignore() &&
            getline(ss, qual, '|') && getline(ss, party, '|') && getline(ss, id_str, '|') && getline(ss, assembly))
        {
            Candidate *c = new Candidate(name, age, qual, party, id_str, assembly);
            voterVerifier.registerCandidate(c); // Directly register
        }
        else
        {
            cerr << "Warning: Skipping malformed line in candidates.txt: " << line << endl;
        }
    }
    cinFile.close();
}

void AdminCLI::saveAllData()
{
    // Save to data/ subdirectory
    ofstream vout("data/voters.txt");
    if (!vout)
    {
        cerr << "Error: Could not open data/voters.txt for writing!" << endl;
        return;
    }
    for (const auto &v : voterVerifier.getVoters())
    {
        vout << v->getName() << "|"
             << v->getAge() << "|"
             << v->getVoterID() << "|"
             << v->getPrivatePin() << "|"
             << v->getAssembly() << "\n";
    }
    vout.close();

    // Save to data/ subdirectory
    ofstream coutFile("data/candidates.txt");
    if (!coutFile)
    {
        cerr << "Error: Could not open data/candidates.txt for writing!" << endl;
        return;
    }
    for (const auto &c : voterVerifier.getCandidates())
    {
        coutFile << c->getCandidateName() << "|"
                 << c->getAge() << "|"
                 << c->getQualification() << "|"
                 << c->getParty() << "|"
                 << c->getCandidateID() << "|"
                 << c->getAssembly() << "\n";
    }
    coutFile.close();
}

void AdminCLI::registerVoter()
{
    string name, id, assembly;
    int age;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getline
    cout << "\nEnter Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    while (!(cin >> age))
    { // Input validation for age
        cout << "Invalid input. Please enter a number for Age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "Enter Voter ID: ";
    getline(cin, id);
    // PIN is no longer asked for here
    cout << "Enter Assembly: ";
    getline(cin, assembly);

    // --- Generate unique PIN ---
    string generatedPin = generateUniquePin(voterVerifier.getVoters());
    // --- ---

    Voter *v = new Voter(name, age, id, generatedPin, assembly);

    // Perform eligibility check *before* adding to the main list
    if (voterVerifier.verifyVoter(v)) // verifyVoter now only checks eligibility
    {
        voterVerifier.registerVoter(v);
        cout << "Voter Registered Successfully.\n";
        cout << "Generated Private PIN for " << name << " (ID: " << id << "): " << generatedPin << " (Please note this down securely)\n";
        saveAllData(); // Save immediately after successful registration
    }
    else
    {
        cout << "Voter registration failed (Eligibility check failed).\n";
        delete v; // Clean up memory if not registered
    }
}

void AdminCLI::deleteVoter()
{
    string idToDelete;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "Enter Voter ID to delete: ";
    getline(cin, idToDelete);
    if (voterVerifier.deleteVoter(idToDelete))
    {
        cout << "Voter with ID " << idToDelete << " deleted successfully.\n";
        saveAllData(); // Save changes
    }
    else
    {
        cout << "Voter with ID " << idToDelete << " not found.\n";
    }
}

void AdminCLI::registerCandidate()
{
    string name, qual, party, assembly, id;
    int age;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "\nEnter Candidate Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    while (!(cin >> age))
    { // Input validation for age
        cout << "Invalid input. Please enter a number for Age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "Enter Qualification: ";
    getline(cin, qual);
    cout << "Enter Party Name (leave empty for independent): ";
    getline(cin, party);
    if (party.empty())
    {
        party = "Independent"; // Assign a default if empty
    }
    cout << "Enter Candidate ID: ";
    getline(cin, id);
    cout << "Enter Assembly: ";
    getline(cin, assembly);

    Candidate *c = new Candidate(name, age, qual, party, id, assembly);

    if (voterVerifier.verifyCandidate(c)) // verifyCandidate checks eligibility
    {
        voterVerifier.registerCandidate(c);
        cout << "Candidate registered successfully.\n";
        saveAllData(); // Save changes
    }
    else
    {
        cout << "Candidate registration failed (Eligibility check failed).\n";
        delete c; // Clean up
    }
}

void AdminCLI::deleteCandidate()
{
    string idToDelete;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "Enter Candidate ID to delete: ";
    getline(cin, idToDelete);
    // cout << "Attempting to delete candidate with ID: \"" << idToDelete << "\"" << endl; // Debugging line (optional)
    if (voterVerifier.deleteCandidate(idToDelete))
    {
        cout << "Candidate with ID " << idToDelete << " deleted successfully.\n";
        saveAllData(); // Save changes
    }
    else
    {
        cout << "Candidate with ID " << idToDelete << " not found.\n";
    }
}

void AdminCLI::resetSystem()
{
    string passwordAttempt;
    cout << "\n!!! WARNING: This action will erase all voter, candidate, and result data !!!\n";
    cout << "Enter the reset password to confirm: ";
    cin >> passwordAttempt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline

    if (passwordAttempt == resetPassword)
    {
        // Use data/ subdirectory
        ofstream vout("data/voters.txt", ios::trunc); // Open in truncation mode to clear
        if (!vout)
        {
            cerr << "Error: Could not clear data/voters.txt\n";
        }
        else
        {
            vout.close();
        }

        ofstream coutFile("data/candidates.txt", ios::trunc);
        if (!coutFile)
        {
            cerr << "Error: Could not clear data/candidates.txt\n";
        }
        else
        {
            coutFile.close();
        }

        ofstream resultsFile("data/results.txt", ios::trunc); // Also clear results file if it exists
        if (!resultsFile)
        {
            cerr << "Warning: Could not clear data/results.txt (may not exist yet)\n";
        }
        else
        {
            resultsFile.close();
        }

        // Clear in-memory data
        voterVerifier.clearVoters();
        voterVerifier.clearCandidates();
        // Optional: Clear ElectionOrganizers' state if needed, though it gets data fresh usually
        // organizers = ElectionOrganizers(); // Reset organizers object if it holds persistent state beyond voter/candidate lists

        cout << "\nSystem has been reset successfully. All data cleared.\n";
    }
    else
    {
        cout << "Incorrect reset password. System reset aborted.\n";
    }
}

// --- Reporting ---
void AdminCLI::listAlreadyVoted()
{
    cout << "\n==== Already Voted Voters ====\n";
    bool found = false;
    // Get voters directly from verifier as it holds the authoritative list before election starts
    for (const auto &voter : voterVerifier.getVoters())
    {
        if (voter->voteStatus()) // Check the hasVoted flag
        {
            cout << "- " << voter->getName() << " (ID: " << voter->getVoterID() << ", Assembly: " << voter->getAssembly() << ")\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "No voters have voted yet or the election hasn't started/finished.\n";
    }
    cout << "============================\n";
}

// Removed showVoterToCandidateMapping() method entirely

// --- Menu ---
void AdminCLI::showMenu()
{
    int choice;
    do
    {
        cout << "\n==== Admin Dashboard ====\n";
        cout << " 1. Register Voter\n";
        cout << " 2. Delete Voter\n";
        cout << " 3. Register Candidate\n";
        cout << " 4. Delete Candidate\n";
        cout << " 5. List Voters\n";                      // Renumbered
        cout << " 6. List Candidates\n";                  // Renumbered
        cout << " 7. Start Election\n";                   // Renumbered
        cout << " 8. List Already Voted Voters\n";        // Renumbered
        cout << " 9. Declare Results\n";                  // Renumbered
        cout << "10. RESET SYSTEM (Requires Password)\n"; // Renumbered
        cout << " 0. Exit\n";
        cout << "Choose: ";

        // Input validation for choice
        while (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
        case 1:
            registerVoter();
            break;
        case 2:
            deleteVoter();
            break;
        case 3:
            registerCandidate();
            break;
        case 4:
            deleteCandidate();
            break;
        case 5: // Was 7
            voterVerifier.listVoters();
            break;
        case 6: // Was 8
            voterVerifier.listCandidates();
            break;
        case 7: // Was 5
        {
            string type, date;
            int pollId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

            cout << "Enter Poll ID (numeric): ";
            while (!(cin >> pollId))
            { // Input validation
                cout << "Invalid input. Please enter a number for Poll ID: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

            cout << "Enter Type (e.g., General, By-election): "; // Changed prompt slightly
            getline(cin, type);
            cout << "Enter Date (DD/MM/YYYY): ";
            getline(cin, date);

            // Important: Ensure organizers have the latest data *before* starting
            organizers.inheritDataFrom(voterVerifier);
            organizers.startElection(type, date, pollId);
            break;
        }
        case 8: // Was 9
            listAlreadyVoted();
            break;
        case 9: // Was 6
                // Ensure organizers have the latest data if results are declared after more admin actions
            organizers.inheritDataFrom(voterVerifier);
            organizers.declareResults();
            break;
        case 10: // Was 11
            resetSystem();
            break;
        // Case for removed feature (old 10) is gone
        case 0:
            cout << "Exiting Admin Panel.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

// --- Main Execution ---
void AdminCLI::run()
{
    // Attempt to load data first, even before login,
    // so PIN generation check has access to existing voters.
    // Handle potential file errors gracefully if files don't exist on first run.
    cout << "Attempting to load data from data/ directory..." << endl;
    loadAllData(); // Load existing data if available
    cout << "Data loading complete." << endl;

    if (login())
    {
        showMenu();
        // Save data on exit AFTER the menu loop finishes
        cout << "Saving data..." << endl;
        saveAllData();
        cout << "Data saved. Goodbye!\n";
    }
    else
    {
        cout << "Login failed! Exiting.\n";
        // Optionally save data even on failed login if load was successful?
        // saveAllData();
    }
}