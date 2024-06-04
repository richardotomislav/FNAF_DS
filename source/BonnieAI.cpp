#include <nds.h>
#include "BonnieAI.h"

BonnieAI::BonnieAI(){}

/**
 * Reset Bonnie function.
 *
 * Reset the position and difficulty of Bonnie
 */
void BonnieAI::Reset()
{
    position = 0;
    difficulty = 5;
}

/**
 * Move Bonnie function.
 *
 * If the number is less than or equal to the difficulty, Bonnie will move
 *
 * @param number Random number generated
 */
void BonnieAI::MoveOpportunity(int number)
{
    if (number <= difficulty)
    {
        position += 1;
    }

}

/**
 * iNCREASE THE DIFFICULTY OF BONNIE
 *
 * @param number Difficulty of Bonnie
 */
void BonnieAI::SetDifficulty(int number)
{
    difficulty += 1;
}

int BonnieAI::ReturnDifficulty()
{
    return difficulty;
}