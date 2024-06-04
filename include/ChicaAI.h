#ifndef CHICAAI_H
#define CHICAAI_H

class ChicaAI
{
    public:
        ChicaAI();
        int position;
        int difficulty;
        void Reset();
        void MoveOpportunity(int number);
        int ReturnDifficulty();
        void SetDifficulty(int number);
};

#endif