// AdminCLI.cpp
#include "AdminCLI.h"
#include <bits/stdc++.h>

using namespace std;

string AdminCLI::generateUniquePin(const vector<Voter *> &existingVoters)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100000, 999999);
    string newPinStr;
    bool pinExists;
    do
    {
        stringstream ss;
        ss << setw(6) << setfill('0') << distrib(gen);
        newPinStr = ss.str();
        pinExists = any_of(existingVoters.begin(), existingVoters.end(),
                           [&](const Voter *v)
                           { return v && v->getPrivatePin() == newPinStr; });
    } while (pinExists);
    return newPinStr;
}

Party *AdminCLI::ensureIndependentPartyExists()
{
    Party *independentParty = voterVerifier.findPartyByID(INDEPENDENT_PARTY_ID);
    if (!independentParty)
    {
        independentParty = new Party(INDEPENDENT_PARTY_ID, INDEPENDENT_PARTY_NAME, INDEPENDENT_PARTY_SYMBOL);
        voterVerifier.addParty(independentParty);
    }
    return independentParty;
}

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
    voterVerifier.clearVoters();
    voterVerifier.clearParties();

    ifstream vin("data/voters.txt");
    string v_line;
    cout << "Loading voters from data/voters.txt..." << endl;
    while (getline(vin, v_line))
    {
        stringstream ss(v_line);
        string name, id, pin_str, assembly, votedStatus_str;
        int age;
        if (getline(ss, name, '|') && (ss >> age) && (ss.peek() == '|') && ss.ignore() &&
            getline(ss, id, '|') && getline(ss, pin_str, '|') && getline(ss, assembly, '|') &&
            getline(ss, votedStatus_str))
        {
            bool votedStatus = (votedStatus_str == "1");
            Voter *v = new Voter(name, age, id, pin_str, assembly, votedStatus);
            voterVerifier.registerVoter(v);
        }
        else
        {
            stringstream ss_old(v_line);
            if (getline(ss_old, name, '|') && (ss_old >> age) && (ss_old.peek() == '|') && ss_old.ignore() &&
                getline(ss_old, id, '|') && getline(ss_old, pin_str, '|') && getline(ss_old, assembly))
            {
                cerr << "Warning: Loaded voter '" << name << "' using old format (vote status set to false)." << endl;
                Voter *v = new Voter(name, age, id, pin_str, assembly, false);
                voterVerifier.registerVoter(v);
            }
            else
            {
                cerr << "Warning: Skipping malformed line in voters.txt: " << v_line << endl;
            }
        }
    }
    vin.close();
    cout << "Loaded " << voterVerifier.getVoters().size() << " voters." << endl;

    ifstream pin("data/parties.txt");
    string p_line;
    Party *currentParty = nullptr;
    cout << "Loading parties and candidates from data/parties.txt..." << endl;

    while (getline(pin, p_line))
    {
        stringstream ss(p_line);
        string type;
        getline(ss, type, '|');

        if (type == "PARTY")
        {
            string id, name, symbol;
            if (getline(ss, id, '|') && getline(ss, name, '|') && getline(ss, symbol))
            {
                Party *newParty = new Party(id, name, symbol);
                voterVerifier.addParty(newParty);
                currentParty = newParty;
            }
            else
            {
                currentParty = nullptr;
            }
        }
        else if (type == "INDEPENDENT")
        {
            currentParty = ensureIndependentPartyExists();
        }
        else if (type == "CANDIDATE")
        {
            if (!currentParty)
            {
                continue;
            }
            string cand_id, name, qual, assembly_str;
            int age;
            if (getline(ss, cand_id, '|') && getline(ss, name, '|') && (ss >> age) && (ss.peek() == '|') && ss.ignore() &&
                getline(ss, qual, '|') && getline(ss, assembly_str))
            {
                Candidate *newCand = new Candidate(name, age, qual, currentParty->getName(), cand_id, assembly_str);
                currentParty->addCandidate(newCand);
            }
        }
        else if (!type.empty())
        {
            cerr << "Warning: Unknown type '" << type << "' in parties.txt line: " << p_line << endl;
            currentParty = nullptr;
        }
    }
    pin.close();
    ensureIndependentPartyExists();
    cout << "Loaded " << voterVerifier.getParties().size() << " parties/groups." << endl;
}

void AdminCLI::saveAllData()
{
    ofstream vout("data/voters.txt");
    if (!vout)
    {
        cerr << "Error: Could not open data/voters.txt for writing!" << endl;
    }
    else
    {
        cout << "Saving " << voterVerifier.getVoters().size() << " voters to data/voters.txt..." << endl;
        for (const auto &v : voterVerifier.getVoters())
        {
            if (v)
            {
                vout << v->getName() << "|" << v->getAge() << "|" << v->getVoterID() << "|"
                     << v->getPrivatePin() << "|" << v->getAssembly() << "|"
                     << (v->voteStatus() ? "1" : "0") << "\n";
            }
        }
        vout.close();
    }

    ofstream pout("data/parties.txt");
    if (!pout)
    {
        cerr << "Error: Could not open data/parties.txt for writing!" << endl;
    }
    else
    {
        cout << "Saving " << voterVerifier.getParties().size() << " parties/groups to data/parties.txt..." << endl;
        Party *independentParty = nullptr;
        for (Party *p : voterVerifier.getParties())
        {
            if (!p)
                continue;
            if (p->getID() == INDEPENDENT_PARTY_ID)
            {
                independentParty = p;
                continue;
            }
            pout << "PARTY|" << p->getID() << "|" << p->getName() << "|" << p->getSymbol() << "\n";
            for (const Candidate *c : p->getCandidates())
            {
                if (c)
                    pout << "CANDIDATE|" << c->getCandidateID() << "|" << c->getCandidateName() << "|" << c->getAge() << "|" << c->getQualification() << "|" << c->getAssembly() << "\n";
            }
        }
        if (independentParty)
        {
            pout << "INDEPENDENT|" << independentParty->getID() << "|" << independentParty->getName() << "|" << independentParty->getSymbol() << "\n";
            for (const Candidate *c : independentParty->getCandidates())
            {
                if (c)
                    pout << "CANDIDATE|" << c->getCandidateID() << "|" << c->getCandidateName() << "|" << c->getAge() << "|" << c->getQualification() << "|" << c->getAssembly() << "\n";
            }
        }
        pout.close();
    }
}


void AdminCLI::registerVoter()
{ 
    string name, id, assembly;
    int age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    while (!(cin >> age))
    { /* ... validation ... */
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Voter ID: ";
    getline(cin, id);
    cout << "Enter Assembly: ";
    getline(cin, assembly);
    string generatedPin = generateUniquePin(voterVerifier.getVoters());
    Voter *v = new Voter(name, age, id, generatedPin, assembly);
    if (voterVerifier.verifyVoter(v))
    {
        voterVerifier.registerVoter(v);
        cout << "Voter Registered Successfully. PIN: " << generatedPin << "\n";
        saveAllData();
    }
    else
    {
        cout << "Voter registration failed (Eligibility check failed).\n";
        delete v;
    }
}

void AdminCLI::deleteVoter()
{ 
    string idToDelete;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

// New: Register Party
void AdminCLI::registerParty()
{
    string id, name, symbol;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\n--- Register New Party ---\n";
    cout << "Enter Party ID (e.g., BJP, CON, AAP - must be unique): ";
    getline(cin, id);
    cout << "Enter Party Name (e.g., Bharatiya Janata Party): ";
    getline(cin, name);
    cout << "Enter Party Symbol (e.g., Lotus, Hand): ";
    getline(cin, symbol);

    // Validate input
    if (id.empty() || name.empty() || symbol.empty() || id == INDEPENDENT_PARTY_ID)
    {
        cout << "Error: Party ID, Name, and Symbol cannot be empty, and ID cannot be '" << INDEPENDENT_PARTY_ID << "'. Registration failed.\n";
        return;
    }

    // Check for uniqueness
    if (voterVerifier.findPartyByID(id) != nullptr)
    {
        cout << "Error: Party ID '" << id << "' already exists. Registration failed.\n";
        return;
    }
    if (voterVerifier.findPartyByName(name) != nullptr)
    {
        cout << "Error: Party Name '" << name << "' already exists. Registration failed.\n";
        return;
    }

    Party *newParty = new Party(id, name, symbol);
    voterVerifier.addParty(newParty); // addParty handles duplicate check again just in case
    cout << "Party '" << name << "' registered successfully with ID '" << id << "'.\n";
    saveAllData();
}


void AdminCLI::registerCandidate()
{
    string name, qual, assembly, cand_id, party_id_input;
    int age;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\n--- Register New Candidate ---\n";
    cout << "Enter Candidate Name: ";
    getline(cin, name);
    cout << "Enter Age: ";
    while (!(cin >> age))
    { /* ... validation ... */
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Qualification: ";
    getline(cin, qual);
    cout << "Enter Candidate ID (must be unique across all candidates): ";
    getline(cin, cand_id);
    cout << "Enter Assembly: ";
    getline(cin, assembly);
    cout << "Enter Party ID to associate with (e.g., BJP, CON) or leave empty for Independent: ";
    getline(cin, party_id_input);

    // Validate input
    if (name.empty() || qual.empty() || cand_id.empty() || assembly.empty())
    {
        cout << "Error: Candidate Name, Qualification, ID, and Assembly cannot be empty. Registration failed.\n";
        return;
    }

    // --- Check Candidate ID Uniqueness ---
    bool id_exists = false;
    std::vector<Candidate *> all_candidates = voterVerifier.getAllCandidates();
    for (const auto *c : all_candidates)
    {
        if (c && c->getCandidateID() == cand_id)
        {
            id_exists = true;
            break;
        }
    }
    if (id_exists)
    {
        cout << "Error: Candidate ID '" << cand_id << "' already exists. Registration failed.\n";
        return;
    }
    // --- ---

    Party *targetParty = nullptr;
    std::string partyNameForCandidate = INDEPENDENT_PARTY_NAME; // Default to Independent

    if (!party_id_input.empty() && party_id_input != INDEPENDENT_PARTY_ID)
    {
        targetParty = voterVerifier.findPartyByID(party_id_input);
        if (!targetParty)
        {
            cout << "Warning: Party ID '" << party_id_input << "' not found. Registering candidate as Independent.\n";
            targetParty = ensureIndependentPartyExists(); // Assign to Independent party object
        }
        else
        {
            partyNameForCandidate = targetParty->getName(); // Use the found party's name
        }
    }
    else
    {
        // Empty input or explicit "IND" means Independent
        targetParty = ensureIndependentPartyExists(); // Assign to Independent party object
        partyNameForCandidate = INDEPENDENT_PARTY_NAME;
    }

    Candidate *newCand = new Candidate(name, age, qual, partyNameForCandidate, cand_id, assembly);

    if (voterVerifier.verifyCandidate(newCand))
    {                                       // Check eligibility
        targetParty->addCandidate(newCand); // Add candidate to the determined party's list
        cout << "Candidate '" << name << "' registered successfully under party '" << partyNameForCandidate << "'.\n";
        saveAllData();
    }
    else
    {
        cout << "Candidate registration failed (Eligibility check failed).\n";
        delete newCand; // Clean up
    }
}

void AdminCLI::deleteCandidate()
{ 
    string idToDelete;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Candidate ID to delete: ";
    getline(cin, idToDelete);

    if (voterVerifier.deleteCandidate(idToDelete))
    { // This now searches through parties
        cout << "Candidate with ID " << idToDelete << " deleted successfully.\n";
        saveAllData();
    }
    else
    {
        // Message is printed within deleteCandidate if not found
        cout << "Candidate with ID " << idToDelete << " not found.\n";
    }
}


void AdminCLI::resetSystem()
{
    string passwordAttempt;
    cout << "\n!!! WARNING: This action will erase all voter, party, candidate, and result data !!!\n";
    cout << "Enter the reset password to confirm: ";
    cin >> passwordAttempt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (passwordAttempt == resetPassword)
    {
        // Clear files
        ofstream vout("data/voters.txt", ios::trunc);
        vout.close();
        ofstream pout("data/parties.txt", ios::trunc);
        pout.close(); // Clear parties file
        
        ofstream resultsFile("data/results.txt", ios::trunc);
        resultsFile.close();

        // Clear in-memory data using ElectionCommission methods
        voterVerifier.clearVoters();
        voterVerifier.clearParties(); // This clears parties and their contained candidates

        cout << "\nSystem has been reset successfully. All data cleared.\n";
        // Ensure Independent party exists after reset for future use
        ensureIndependentPartyExists();
        saveAllData(); // Save the empty state (with Independent party)
    }
    else
    {
        cout << "Incorrect reset password. System reset aborted.\n";
    }
}

// --- Reporting ---
void AdminCLI::listVoters()
{
    voterVerifier.listVoters(); // Use method from ElectionCommission
}

void AdminCLI::listPartiesAndCandidates()
{
    voterVerifier.listCandidatesGroupedByParty(); // Use method from ElectionCommission
}

void AdminCLI::listAlreadyVoted()
{ // Unchanged logic
    cout << "\n==== Already Voted Voters ====\n";
    bool found = false;
    for (const auto &voter : voterVerifier.getVoters())
    {
        if (voter && voter->voteStatus())
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

// --- Menu ---
void AdminCLI::showMenu()
{
    int choice;
    do
    {
        cout << "\n==== Admin Dashboard ====\n";
        cout << " 1. Register Voter\n";
        cout << " 2. Delete Voter\n";
        cout << " 3. Register Party\n";                   
        cout << " 4. Register Candidate\n";               
        cout << " 5. Delete Candidate\n";                 
        cout << " 6. List Voters\n";                      
        cout << " 7. List Parties & Candidates\n";        
        cout << " 8. Start Election\n";                   
        cout << " 9. List Already Voted Voters\n";        
        cout << "10. Declare Results\n";                  
        cout << "11. RESET SYSTEM (Requires Password)\n"; 
        cout << " 0. Exit\n";
        cout << "Choose: ";

        while (!(cin >> choice))
        { /* ... validation ... */
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
            registerParty();
            break; 
        case 4:
            registerCandidate();
            break; // Was 3
        case 5:
            deleteCandidate();
            break; // Was 4
        case 6:
            listVoters();
            break; // Was 5
        case 7:
            listPartiesAndCandidates();
            break; 
        case 8:    // Was 7 (Start Election)
        {
            string type, date;
            int pollId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter Poll ID: ";
            while (!(cin >> pollId))
            { /* validation */
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter Type: ";
            getline(cin, type);
            cout << "Enter Date (DD/MM/YYYY): ";
            getline(cin, date);
            organizers.inheritDataFrom(voterVerifier); // Ensure it gets parties too
            organizers.startElection(type, date, pollId);
            break;
        }
        case 9:
            listAlreadyVoted();
            break;                                     // Was 8
        case 10:                                       // Was 9 (Declare Results)
            organizers.inheritDataFrom(voterVerifier); // Refresh data before declaring
            organizers.declareResults();
            break;
        case 11:
            resetSystem();
            break; // Was 10
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
{ // Load/Save logic remains similar, but uses new methods
    cout << "Attempting to load data from data/ directory..." << endl;
    loadAllData(); // Calls the rewritten loadAllData
    cout << "Data loading complete." << endl;

    if (login())
    {
        showMenu();
        cout << "Saving data..." << endl;
        saveAllData(); // Calls the rewritten saveAllData
        cout << "Data saved. Goodbye!\n";
    }
    else
    {
        cout << "Login failed! Exiting.\n";
    }
}