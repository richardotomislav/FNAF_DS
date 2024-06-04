#include <nds.h>
#include "ChicaAI.h"

ChicaAI::ChicaAI(){}

/**
 * Reset Chica function.
*/
void ChicaAI::Reset()
{
    position = 0;
    difficulty = 5;
}


/**
 * Move Chica function.
 *
 * If the number is less than or equal to the difficulty, Chica will move
 *
 * @param number Random number generated
 */
void ChicaAI::MoveOpportunity(int number)
{
    if (number <= difficulty)
    {
        position += 1;
    }

}

/**
 * Increase the difficulty of Chica
 *
 * @param number Difficulty of Chica
 */
void ChicaAI::SetDifficulty(int number)
{
    difficulty += 1;
}

int ChicaAI::ReturnDifficulty()
{
    return difficulty;
}