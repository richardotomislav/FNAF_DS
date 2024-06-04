#ifndef FOXY_AI_H
#define FOXY_AI_H

class FoxyAI
{
    public: 
        FoxyAI();
        int position;
        int difficulty;
        void Reset();
        void MoveOpportunity(int number, bool locked);
        int ReturnDifficulty();
        void SetDifficulty(int number);
};


#endif