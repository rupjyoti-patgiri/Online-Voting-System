#include "AdminCLI.h"
#include <iostream>
#include <fstream>
#include <limits>

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
    string name, id;
    int age;

    while (vin >> ws && getline(vin, name))
    {
        vin >> age >> ws;
        getline(vin, id);
        Voter *v = new Voter(name, age, id);
        voterVerifier.registerVoter(v);
    }
    vin.close();

    ifstream cinFile("candidates.txt");
    string party, qual;
    while (cinFile >> ws && getline(cinFile, name))
    {
        cinFile >> age >> ws;
        getline(cinFile, qual);
        getline(cinFile, party);
        Candidate *c = new Candidate(name, age, qual, party);
        voterVerifier.registerCandidate(c);
    }
    cinFile.close();
}

void AdminCLI::saveAllData()
{
    ofstream vout("voters.txt");
    for (auto v : voterVerifier.getVoters())
        vout << v->getName() << "\n"
             << v->isEligible() << "\n"
             << v->getVoterID() << "\n";
    vout.close();

    ofstream coutFile("candidates.txt");
    for (auto c : voterVerifier.getCandidates())
        coutFile << c->getCandidateName() << "\n"
                 << c->isEligible() << "\n"
                 << c->getParty() << "\n";
    coutFile.close();
}

void AdminCLI::registerVoter()
{
    string name, id;
    int age;
    cin.ignore();
    cout << "\nEnter Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter Voter ID: ";
    getline(cin, id);

    Voter *v = new Voter(name, age, id);
    if (voterVerifier.verifyVoter(v))
        voterVerifier.registerVoter(v);
    else
        delete v;

    saveAllData();
}

void AdminCLI::registerCandidate()
{
    string name, qual, party;
    int age;
    cin.ignore();
    cout << "\nEnter Candidate Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter Qualification: ";
    getline(cin, qual);
    cout << "Enter Party Name: ";
    getline(cin, party);

    Candidate *c = new Candidate(name, age, qual, party);
    if (voterVerifier.verifyCandidate(c))
        voterVerifier.registerCandidate(c);
    else
        delete c;

    saveAllData();
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

void AdminCLI::showMenu()
{
    int choice;
    do
    {
        cout << "\n==== Admin Dashboard ====\n";
        cout << "1. Register Voter\n";
        cout << "2. Register Candidate\n";
        cout << "3. Start Election\n";
        cout << "4. Declare Results\n";
        cout << "5. List Voters\n";
        cout << "6. List Candidates\n";
        cout << "7. RESET SYSTEM\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            registerVoter();
            break;
        case 2:
            registerCandidate();
            break;
        case 3:
        {
            string type, date;
            cin.ignore();
            cout << "Enter Type (Open/Restricted): ";
            getline(cin, type);
            cout << "Enter Date (DD/MM/YYYY): ";
            getline(cin, date);
            organizers.inheritDataFrom(voterVerifier);
            // inherit data
            organizers.startElection(type, date);
            break;
        }
        case 4:
            organizers.declareResults();
            break;
        case 5:
            voterVerifier.listVoters();
            break;
        case 6:
            voterVerifier.listCandidates();
            break;
        case 7:
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
    }
    else
    {
        cout << "Login failed!\n";
    }
}
