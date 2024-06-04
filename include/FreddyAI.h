#ifndef FREDDYAI_H
#define FREDDYAI_H

class FreddyAI
{
    public:
        FreddyAI();
        int position;
        int difficulty;
        void Reset();
        void MoveOpportunity(int number, bool locked);
        int ReturnDifficulty();
        void SetDifficulty(int number);
};


#endif