// Party.cpp
#include "Party.h"
#include "Candidate.h"
#include <bits/stdc++.h>
using namespace std;

// --- Constructor ---
Party::Party(string id, string name, string sym)
    : partyID(id), partyName(name), symbol(sym)
{
}

// --- Destructor ---
Party::~Party()
{
    for (Candidate *c : candidates)
    {
        delete c;
    }
    candidates.clear();
}

// --- Getters ---
string Party::getID() const
{
    return partyID;
}

string Party::getName() const
{
    return partyName;
}

string Party::getSymbol() const
{
    return symbol;
}

const vector<Candidate *> &Party::getCandidates() const
{
    return candidates;
}

// --- Modifiers ---
void Party::addCandidate(Candidate *c)
{
    if (c)
    {
        bool exists = false;
        for (const auto *existing_c : candidates)
        {
            if (existing_c && existing_c->getCandidateID() == c->getCandidateID())
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            candidates.push_back(c);
        }
        else
        {
            cerr << "Warning: Candidate with ID " << c->getCandidateID()
                 << " already exists in party " << partyName << ". Not adding duplicate." << endl;
            delete c;
        }
    }
}

bool Party::removeCandidate(const string &candidateID)
{
    auto it = remove_if(candidates.begin(), candidates.end(),
                        [&](Candidate *c)
                        {
                            if (c && c->getCandidateID() == candidateID)
                            {
                                delete c;
                                return true;
                            }
                            return false;
                        });

    if (it != candidates.end())
    {
        candidates.erase(it, candidates.end());
        return true;
    }
    return false;
}

// --- Finders ---
Candidate *Party::findCandidateByID(const string &candidateID)
{
    for (Candidate *c : candidates)
    {
        if (c && c->getCandidateID() == candidateID)
        {
            return c;
        }
    }
    return nullptr;
}

// --- Display ---
void Party::displayPartyInfo() const
{
    cout << "Party ID: " << partyID << ", Name: " << partyName << ", Symbol: " << symbol << endl;
    cout << "  Candidates (" << candidates.size() << "):\n";
    if (candidates.empty())
    {
        cout << "    <None>\n";
    }
    else
    {
        for (const auto *c : candidates)
        {
            if (c)
            {
                cout << "    - ID: " << c->getCandidateID() << ", Name: " << c->getCandidateName()
                     << ", Age: " << c->getAge() << ", Assembly: " << c->getAssembly() << endl;
            }
        }
    }
}
