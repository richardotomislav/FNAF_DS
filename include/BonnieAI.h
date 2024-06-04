#ifndef BONNIEAI_H
#define BONNIEAI_H

class BonnieAI
{
    public:
        BonnieAI();
        int position;
        int difficulty;
        void Reset();
        void MoveOpportunity(int number);
        int ReturnDifficulty();
        void SetDifficulty(int number);
};

#endif