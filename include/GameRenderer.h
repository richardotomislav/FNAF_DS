#include <nds.h>

#ifndef GAMERENDERER_H
#define GAMERENDERER_H

class GameRenderer
{

    public:
        GameRenderer();
        void Render(int pP, u16* mapMemorySub, int camNum, int bPos, int cPos, int fdPos, int fxPos, bool lDL, bool rDL);
        void RenderImages(int camNum, int bPos, int cPos, int fdPos, int fxPos, bool lDL, bool rDL);
        void RenderJumpscare(int whoKilled);
        void SecondaryRender(int level,int time);
        int row,col,pos_mapMemory,pos_mapData,pos_mapMemorySub,pos_mapDataSub;
};

#endif