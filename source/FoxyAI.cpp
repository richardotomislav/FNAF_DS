#include "FoxyAI.h"

FoxyAI::FoxyAI(){}

/**
 * Reset Foxy function.
 *
 * Reset the position and difficulty of Foxy
*/
void FoxyAI::Reset()
{
    position = 0;
    difficulty = 8;
}

/**
 * Move Foxy function.
 *
 * If the number is less than or equal to the difficulty, Foxy will move
 *
 * @param number Random number generated
 */
void FoxyAI::MoveOpportunity(int number, bool locked)
{
    if (number <= difficulty && locked == false)
    {
        position += 1;
    }

}

/**
 * Increase the difficulty of Foxy
 *
 * @param number Difficulty of Foxy
 */
void FoxyAI::SetDifficulty(int number)
{
    difficulty += 1;
}

int FoxyAI::ReturnDifficulty()
{
    return difficulty;
}