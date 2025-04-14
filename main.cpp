#include "AdminCLI.h"

int main()
{
    AdminCLI cli;
    cli.run();
    return 0;
}

// To compile :
// g++ main.cpp AdminCLI.cpp Voter.cpp Candidate.cpp ElectionCommission.cpp Election.cpp PartyName.cpp -o votingSystem

// To run :
// ./votingSystem
