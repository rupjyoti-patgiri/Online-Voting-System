// PartyName.cpp
#include "PartyName.h"
#include "Candidate.h"

PartyName::PartyName(string name, string sym, string man)
{
    partyName = name;
    symbol = sym;
    manifesto = man;
}

void PartyName::addCandidate(Candidate *c)
{
    candidates.push_back(c);
}

string PartyName::getPartyName() const
{
    return partyName;
}

void PartyName::displayCandidates() const
{
    cout << "Party: " << partyName << "\nCandidates:\n";
    for (const auto &c : candidates)
    {
        cout << "  - " << c->getCandidateName() << endl;
    }
}