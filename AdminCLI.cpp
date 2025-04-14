// AdminCLI.cpp
#include "AdminCLI.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream> // For stringstream

using namespace std;

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

void AdminCLI::loadAllData()
{
    ifstream vin("voters.txt");
    string line;
    while (getline(vin, line))
    {
        stringstream ss(line);
        string name, id, pin_str, assembly;
        int age;
        if (getline(ss, name, '|') && ss >> age && getline(ss >> ws, id, '|') && getline(ss >> ws, pin_str, '|') && getline(ss >> ws, assembly))
        {
            Voter *v = new Voter(name, age, id, pin_str, assembly);
            voterVerifier.registerVoter(v);
        }
    }
    vin.close();

    ifstream cinFile("candidates.txt");
    while (getline(cinFile, line))
    {
        stringstream ss(line);
        string name, party, qual, id_str, assembly;
        int age;
        if (getline(ss, name, '|') && ss >> age && getline(ss >> ws, qual, '|') && getline(ss >> ws, party, '|') && getline(ss >> ws, id_str, '|') && getline(ss >> ws, assembly))
        {
            Candidate *c = new Candidate(name, age, qual, party, id_str, assembly); // id_str is already a string
            voterVerifier.registerCandidate(c);
        }
    }
    cinFile.close();
}

void AdminCLI::saveAllData()
{
    ofstream vout("voters.txt");
    for (auto v : voterVerifier.getVoters())
        vout << v->getName() << "|"
             << v->getAge() << "|"
             << v->getVoterID() << "|"
             << v->getPrivatePin() << "|"
             << v->getAssembly() << "\n";
    vout.close();

    ofstream coutFile("candidates.txt");
    for (auto c : voterVerifier.getCandidates())
        coutFile << c->getCandidateName() << "|"
                 << c->getAge() << "|"
                 << c->getQualification() << "|"
                 << c->getParty() << "|"
                 << c->getCandidateID() << "|" // Now a string
                 << c->getAssembly() << "\n";
    coutFile.close();
}

void AdminCLI::registerVoter()
{
    string name, id, pin, assembly;
    int age;
    cin.ignore();
    cout << "\nEnter Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter Voter ID: ";
    getline(cin, id);
    cout << "Enter Private PIN for Voter: ";
    getline(cin, pin);
    cout << "Enter Assembly: "; // Added assembly input
    getline(cin, assembly);

    Voter *v = new Voter(name, age, id, pin, assembly); // Updated constructor call
    if (voterVerifier.verifyVoter(v))
        voterVerifier.registerVoter(v);
    else
        delete v;

    saveAllData();
}

void AdminCLI::deleteVoter()
{
    string idToDelete;
    cin.ignore();
    cout << "Enter Voter ID to delete: ";
    getline(cin, idToDelete);
    if (voterVerifier.deleteVoter(idToDelete))
    {
        cout << "Voter with ID " << idToDelete << " deleted successfully.\n";
        saveAllData();
    }
    else
    {
        cout << "Voter with ID " << idToDelete << " not found.\n";
    }
}

void AdminCLI::registerCandidate()
{
    string name, qual, party, assembly, id; // Changed id type to string
    int age;
    cin.ignore();
    cout << "\nEnter Candidate Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter Qualification: ";
    getline(cin, qual);
    cout << "Enter Party Name (leave empty for independent): ";
    getline(cin, party);
    cout << "Enter Candidate ID: "; // Input as string
    getline(cin, id);
    cout << "Enter Assembly: ";
    getline(cin, assembly);

    Candidate *c = new Candidate(name, age, qual, party, id, assembly); // Updated constructor call
    if (voterVerifier.verifyCandidate(c))
        voterVerifier.registerCandidate(c);
    else
        delete c;

    saveAllData();
}

void AdminCLI::deleteCandidate()
{
    string idToDelete;
    cout << "Enter Candidate ID to delete: ";
    getline(cin, idToDelete);
    cout << "Attempting to delete candidate with ID: " << idToDelete << endl; // Debugging line
    if (voterVerifier.deleteCandidate(idToDelete))
    {
        cout << "Candidate with ID " << idToDelete << " deleted successfully.\n";
        saveAllData();
    }
    else
    {
        cout << "Candidate with ID " << idToDelete << " not found.\n";
    }
}

void AdminCLI::resetSystem()
{
    ofstream("voters.txt", ios::trunc).close();
    ofstream("candidates.txt", ios::trunc).close();
    ofstream("results.txt", ios::trunc).close();

    voterVerifier.clearVoters();
    voterVerifier.clearCandidates();

    cout << "\nSystem has been reset. All data cleared.\n";
}

void AdminCLI::listAlreadyVoted()
{
    cout << "\n==== Already Voted Voters ====\n";
    bool found = false;
    for (const auto &voter : voterVerifier.getVoters())
    {
        if (voter->voteStatus())
        {
            cout << "- " << voter->getName() << " (ID: " << voter->getVoterID() << ")\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "No voters have voted yet.\n";
    }
}

void AdminCLI::showVoterToCandidateMapping()
{
    string passwordAttempt;
    cout << "\nEnter password to view voter to candidate mapping: ";
    cin >> passwordAttempt;
    cin.ignore(); // Consume newline

    if (passwordAttempt == voterMappingPassword)
    {
        cout << "\n==== Voter to Candidate Mapping ====\n";
        organizers.displayVoterToCandidateMapping();
    }
    else
    {
        cout << "Incorrect password.\n";
    }
}

void AdminCLI::showMenu()
{
    int choice;
    do
    {
        cout << "\n==== Admin Dashboard ====\n";
        cout << "1. Register Voter\n";
        cout << "2. Delete Voter\n"; // New option
        cout << "3. Register Candidate\n";
        cout << "4. Delete Candidate\n"; // New option
        cout << "5. Start Election\n";
        cout << "6. Declare Results\n";
        cout << "7. List Voters\n";
        cout << "8. List Candidates\n";
        cout << "9. List Already Voted Voters\n";                    // New option
        cout << "10. View Voter to Candidate Mapping (Protected)\n"; // New option
        cout << "11. RESET SYSTEM\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

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
        case 5:
        {
            string type, date;
            int pollId; // New
            cin.ignore();
            cout << "Enter Poll ID: "; // New
            cin >> pollId;             // New
            cin.ignore();
            cout << "Enter Type (Open/Restricted): ";
            getline(cin, type);
            cout << "Enter Date (DD/MM/YYYY): ";
            getline(cin, date);
            organizers.inheritDataFrom(voterVerifier);
            organizers.startElection(type, date, pollId); // Updated
            break;
        }
        case 6:
            organizers.declareResults();
            break;
        case 7:
            voterVerifier.listVoters();
            break;
        case 8:
            voterVerifier.listCandidates();
            break;
        case 9:
            listAlreadyVoted();
            break;
        case 10:
            showVoterToCandidateMapping();
            break;
        case 11:
            resetSystem();
            break;
        case 0:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void AdminCLI::run()
{
    if (login())
    {
        loadAllData();
        showMenu();
        saveAllData(); // Save any changes made during the session
    }
    else
    {
        cout << "Login failed!\n";
    }
}