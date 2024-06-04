#include <nds.h>
#include "GameRenderer.h"
#include "map_tiles.h"

// Load the images related to the cameras
#include "animroom.h"
#include "animroombonnie.h"
#include "bathroom.h"
#include "chicabathroom.h"
#include "chicacorner.h"
#include "chicadinner.h"
#include "chicahallway.h"
#include "closet.h"
#include "closetbonnie.h"
#include "cornerbonnie.h"
#include "cornerchica.h"
#include "cornerwithbonnie.h"
#include "dinner.h"
#include "dinnerbonnie.h"
#include "foxyrun.h"
#include "foxystage1.h"
#include "foxystage2.h"
#include "foxystage3.h"
#include "foxystage4.h"
#include "hallway.h"
#include "hallwaybonnie.h"
#include "hallwaychica.h"
#include "stagebonniefreddy.h"
#include "stagechicafreddy.h"
#include "stagefreddy.h"
#include "stagefull.h"

#include "black.h"

#include "bonnieJumpscare.h"
#include "chicaJumpscare.h"
#include "foxyJumpscare.h"

GameRenderer::GameRenderer(){}

// #############################################################################################
// ########################### CONTINUE WITH CAUTION                                ############
// ########################### IF YOU DON'T LIKE A LOT OF NESTED IFS AND SWITCHES   ############
// ########################### THIS IS NOT THE FILE FOR YOU                         ############
// #############################################################################################

/**
 * Render function.
 *
 * This function is responsible for rendering the tile map of the game.
 *
 * @param pP The current position of the player
 * @param mapMemorySub The memory of the map
 * @param camNum The current camera number
 * @param bPos The current position of Bonnie
 * @param cPos The current position of Chica
 * @param fdPos The current position of Freddy
 * @param fxPos The current position of Foxy
 * @param lDL If the left door is locked
 * @param rDL If the right door is locked
*/
void GameRenderer::Render(int pP, u16* mapMemorySub, int camNum, int bPos, int cPos, int fdPos, int fxPos, bool lDL, bool rDL)
{

    switch (pP)
    {
        case 0:
            if (lDL)
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = leftEntranceLock[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            else if (bPos == 6)
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = leftEntranceBonnie[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            else
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = leftEntrance[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            break;
        case 1:
            pos_mapDataSub = 0;
            for(row=0;row<24;row++)
                for(col=0;col<32;col++)
                {
                    pos_mapMemorySub            = row*32+col;
                    mapMemorySub[pos_mapMemorySub] = centerRoom[pos_mapDataSub];
                    pos_mapDataSub ++;
                }
            break;
        case 2:

            if(rDL)
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = rightEntranceLock[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            else if (cPos == 6)
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = rightEntranceChica[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            else
            {
                pos_mapDataSub = 0;
                for(row=0;row<24;row++)
                    for(col=0;col<32;col++)
                    {
                        pos_mapMemorySub            = row*32+col;
                        mapMemorySub[pos_mapMemorySub] = rightEntrance[pos_mapDataSub];
                        pos_mapDataSub ++;
                    }
                break;
            }
            break;
        case 3:
            pos_mapDataSub = 0;
            for(row=0;row<24;row++)
                for(col=0;col<32;col++)
                {
                    pos_mapMemorySub           = row*32+col;
                    mapMemorySub[pos_mapMemorySub] = camMap[pos_mapDataSub];
                    pos_mapDataSub ++;
                }
            break;
        case 10:
            pos_mapDataSub = 0;
            for(row=0;row<24;row++)
                for(col=0;col<32;col++)
                {
                    pos_mapMemorySub            = row*32+col;
                    mapMemorySub[pos_mapMemorySub] = sixAM[pos_mapDataSub];
                    pos_mapDataSub ++;
                }
            break;
        case 11:
            pos_mapDataSub = 0;
            for(row=0;row<24;row++)
                for(col=0;col<32;col++)
                {
                    pos_mapMemorySub            = row*32+col;
                    mapMemorySub[pos_mapMemorySub] = gameOver[pos_mapDataSub];
                    pos_mapDataSub ++;
                }
            break;
        default:
            break;
    }

}

/**
 * RenderImages function.
 * @attention
 * Images are copied from each file .h converted via grit to a .c and .h file; copied to the VRAM_A memory bank.
 *
 * @param camNum The current camera number
 * @param bPos The current position of Bonnie
 * @param cPos The current position of Chica
 * @param fdPos The current position of Freddy
 * @param fxPos The current position of Foxy
 * @param lDL If the left door is locked
 * @param rDL If the right door is locked
*/
void GameRenderer::RenderImages(int camNum, int bPos, int cPos, int fdPos, int fxPos, bool lDL, bool rDL)
{
    if (camNum == 0)
        dmaCopy(blackBitmap, VRAM_A , 256*192*2);
    else if (camNum == 1)
    {
        if (bPos == 0 && cPos == 0)
            dmaCopy(stagefullBitmap, VRAM_A , 256*192*2);
        else if (bPos == 0 && cPos != 0)
            dmaCopy(stagebonniefreddyBitmap, VRAM_A , 256*192*2);
        else if (bPos != 0 && cPos == 0)
            dmaCopy(stagechicafreddyBitmap, VRAM_A , 256*192*2);
        else if (bPos != 0 && cPos != 0)
            dmaCopy(stagefreddyBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 2)
    {
        if (bPos == 1)
            dmaCopy(dinnerbonnieBitmap, VRAM_A , 256*192*2);
        else if (cPos == 1)
            dmaCopy(chicadinnerBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(dinnerBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 3)
    {
        if (fxPos == 0)
            dmaCopy(foxystage1Bitmap, VRAM_A , 256*192*2);
        else if (fxPos == 1)
            dmaCopy(foxystage2Bitmap, VRAM_A , 256*192*2);
        else if (fxPos == 2)
            dmaCopy(foxystage3Bitmap, VRAM_A , 256*192*2);
        else if (fxPos == 3)
            dmaCopy(foxystage4Bitmap, VRAM_A , 256*192*2);
        else if (fxPos != 3)
            dmaCopy(foxystage4Bitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 4)
    {
        if (bPos == 2)
            dmaCopy(animroombonnieBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(animroomBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 5)
    {
        if (cPos == 2)
            dmaCopy(chicabathroomBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(bathroomBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 6)
    {
        if (fxPos == 4)
            dmaCopy(foxyrunBitmap, VRAM_A , 256*192*2);
        else if (bPos == 3)
            dmaCopy(hallwaybonnieBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(hallwayBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 7)
    {
        if (bPos == 5)
            dmaCopy(cornerwithbonnieBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(cornerbonnieBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 8)
    {
        if (cPos == 4)
            dmaCopy(chicahallwayBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(hallwaychicaBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 9)
    {
        if (cPos == 5)
            dmaCopy(chicacornerBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(cornerchicaBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 10)
    {
        if (bPos == 4)
            dmaCopy(closetbonnieBitmap, VRAM_A , 256*192*2);
        else
            dmaCopy(closetBitmap, VRAM_A , 256*192*2);
    }
    else if (camNum == 11)
        dmaCopy(blackBitmap, VRAM_A , 256*192*2);
}

/**
 * Sets the image to render based on the animatronic that killed the player.
 *
 * @param whoKilled The animatronic that killed the player
*/
void GameRenderer::RenderJumpscare(int whoKilled)
{
    switch (whoKilled)
    {
        case 20:
            dmaCopy(bonnieJumpscareBitmap, VRAM_A , 256*192*2);
            break;
        case 21:
            dmaCopy(chicaJumpscareBitmap, VRAM_A , 256*192*2);
            break;
        case 22:
            dmaCopy(foxyJumpscareBitmap, VRAM_A , 256*192*2);
            break;
        default:
            break;
    }
}

/**
 * Additional renderer used to display the current battery levels and time.
 * @note
 * This function was made to separate renderers and make the code more readable.
 *
 * @param batteryLevel The current battery batteryLevel
 * @param time The current time
*/
void GameRenderer::SecondaryRender(int batteryLevel, int time)
{
    // centerRoom[276] is the position of current hour.
    if (time == 0)
        centerRoom[276] = 5;
    if (time == 50)
        centerRoom[276] = 6;
    if (time == 100)
        centerRoom[276] = 7;
    if (time == 150)
        centerRoom[276] = 8;
    if (time == 200)
        centerRoom[276] = 9;
    if (time == 250)
        centerRoom[276] = 10;

    // Check battery levels and display the tile correspoding to it's color.
    if (batteryLevel ==  79)
    {
        for (int i = 0; i < sizeof(camMap); i++)
        {
            if (camMap[i] == 12)
            {
                camMap[i] = 0;
            }
        }
    }
    else if (batteryLevel ==  50)
    {
        for (int i = 0; i < sizeof(camMap); i++)
        {
            if (camMap[i] == 13)
            {
                camMap[i] = 0;
            }
        }
    }
    else if (batteryLevel ==  25)
    {
        for (int i = 0; i < sizeof(camMap); i++)
        {
            if (camMap[i] == 14)
            {
                camMap[i] = 0;
            }
        }
    }
    else if (batteryLevel ==  10)
    {
        for (int i = 0; i < sizeof(camMap); i++)
        {
            if (camMap[i] == 15)
            {
                camMap[i] = 0;
            }
        }
    }
    else if (batteryLevel ==  5)
    {
        for (int i = 0; i < sizeof(camMap); i++)
        {
            if (camMap[i] == 16)
            {
                camMap[i] = 0;
            }
        }
    }
}