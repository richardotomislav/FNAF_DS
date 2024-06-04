#include <nds.h>
#include "FreddyAI.h"

// ########################################################################################
// ########################### FREDDY AI IS NOT IMPLEMENTED ###############################
// ########################################################################################

FreddyAI::FreddyAI(){}

void FreddyAI::Reset()
{
    position = 0;
    difficulty = 0;
}

void FreddyAI::MoveOpportunity(int number, bool locked)
{
    if (number <= difficulty && locked == false)
    {
        position += 1;
    }
}
int FreddyAI::ReturnDifficulty()
{
    return difficulty;
}

void FreddyAI::SetDifficulty(int number)
{
    difficulty = number;
}