#include <nds.h>
#include <stdio.h>

#include "GameState.h"

#include <time.h>
#include <cstdlib>
#include <ctime>
#include <random>

#include "BonnieAI.h"
#include "ChicaAI.h"
#include "FreddyAI.h"
#include "FoxyAI.h"

GameState::GameState(){}

int debugGS;

/**
 * @brief Get a random number between 0 and max, inclusive
 *
 * @param max
 * @return int
*/
int GameState::GetRandomNumber(int max)
{
    // int N = 20;
    int randomNumber = std::rand() % (max + 1);
    return randomNumber;
}

/**
 * @brief Reset the game state
 *
*/
void GameState::Reset()
{
    std:srand(static_cast<unsigned int>(std::time(nullptr))); // Crear la semilla para el generador de numeros aleatorios
    leftDoor = false;
    rightDoor = false;

    bonnieNum = 0;

    chicaNum = 0;

    foxyNum = 0;
    lockFoxy = true;

    freddyNum = 0;
    lockFreedy = true;

    batteryLevel = 100;
    gameOver = false;

    bonnieAI.Reset();
    chicaAI.Reset();
    freddyAI.Reset();
    foxyAI.Reset();

    whoKilled = 0;


}

// Debugging function, not functional, use nocashDebug()
void GameState::DebugGS()
{
    iprintf("\x1b[3;2HDebug: %d", debugGS);
    iprintf("\x1b[4;2HLeft Door: %d", leftDoor);
    iprintf("\x1b[5;2HRight Door: %d", rightDoor);
    debugGS += 1;
}

/**
 * Main AI function, calls the AI for each animatronic
 *
 * Every animatronic is called for a movement oportuniy.
 * When the rolled number is less than the difficulty integer, the animatronic moves one step.
*/
void GameState::CallAI()
{
    // Roll a number for each animatronic
    bonnieNum   = GetRandomNumber(20);
    chicaNum    = GetRandomNumber(20);
    freddyNum   = GetRandomNumber(20);
    foxyNum     = GetRandomNumber(20);

    // Call the movement opotunity for each animatronic
    bonnieAI.MoveOpportunity(bonnieNum);
    chicaAI.MoveOpportunity(chicaNum);
    if (foxyAI.position != 4)
        foxyAI.MoveOpportunity(foxyNum, lockFoxy);
    freddyAI.MoveOpportunity(freddyNum, lockFreedy);

    /**
     * If the animatronic is in the same position as the player and the door is closed, the player is killed.
     * whoKilled is a variable to tell the render function which animatronic killed the player and display it.
    */
    if (bonnieAI.position == 7 && leftDoor == false)
    {
        whoKilled = 20;
        gameOver = true;
    }
    else if (bonnieAI.position == 7 && leftDoor == true)
    {
        bonnieAI.position = 0;
    }

    if (chicaAI.position == 7 && rightDoor == false)
    {
        whoKilled = 21;
        gameOver = true;
    }
    else if (chicaAI.position == 7 && rightDoor == true)
    {
        chicaAI.position = 0;
    }

}

/**
 * @brief Check if the game is over
 *
 * @return bool
*/
bool GameState::IsTerminal()
{
    return gameOver;
}
