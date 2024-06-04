#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "BonnieAI.h"
#include "ChicaAI.h"
#include "FreddyAI.h"
#include "FoxyAI.h"

class GameState
{
    public:
        GameState();
        int bonnieNum;
        int chicaNum;
        int freddyNum;
        int foxyNum;

        int batteryLevel;

        bool leftDoor;
        bool rightDoor;
        bool lockFoxy;
        bool lockFreedy;

        bool gameOver;

        void Reset();
        void CallAI();
        bool IsTerminal();
        void DebugGS();
        void GameOverChecks(int gT);
        int GetRandomNumber(int max);

        BonnieAI bonnieAI;
        ChicaAI chicaAI;
        FoxyAI foxyAI;
        FreddyAI freddyAI;

        int whoKilled;
};

#endif