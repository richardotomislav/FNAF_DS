#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <string>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "GameState.h"
#include "BonnieAI.h"
#include "back_tiles.h"
#include "cam_tiles.h"
#include "battery_tiles.h"

#include "GameRenderer.h"

using namespace std;

//---------------------------------------------------------------------------------

// Debug vars
int debug = 0;
// Test loops
void Debug() { debug += 1; }

/**
 * Debugs the game state by printing a message along with the value of a variable.
 * @attention ONLY WORKS WITH THE DEBUG VERSION OF NO$GBA.
 *
 * @param gameState Pass the GameState
 * @param varToDebug The variable to debug.
 * @param message The message to display.
 *
 * @note
 * Information about the nocashMessage() command obtained from libnds, debug.h
 * Solution to convert an int to string: https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
 * Solution to concatenate strings: https://stackoverflow.com/questions/1898657/how-to-concatenate-a-stdstring-and-an-int
 * Solution to convert a string to char*: https://stackoverflow.com/questions/7352099/stdstring-to-char
 * Solution proposed by Copilot, the above sources are those used by Copilot to generate this method.
 */
void nocashDebug(GameState &gameState, int varToDebug, const std::string &message)
{
    string toDebugStr = to_string(varToDebug);
    string fullMessage = message + ": " + toDebugStr;
    const char *fullmessageChar = fullMessage.c_str();

    nocashMessage(fullmessageChar);
}

// Global timer: Counts the number of ticks. 300 ticks equal to 5 minutes.
int globalTimer = 0;

// Foxy cooldown: Tracks the cooldown period for Foxy, when he can make a move again.
int foxyCooldown = 0;

// Foxy death timer: Tracks the time until Foxy kills the player.
int foxyDeathTimer = 0;

// Freddy movement timer: Tracks the time since Freddy's last movement.
int freedyMovementTimer = 0;

// Progress Variables
bool gracePeriod = true;
bool foxyEnabled = false;
bool freedyEnabled = false;

// Game Vars
touchPosition touchPosXY;
bool camerasUp;
int camChecking = 0;

// Timer Variables
bool aiCalled = false;
bool batteryDrain = false;

// Tiles Memory Definitions
static u8*  tileMemory;
static u16* mapMemory;
static u8*  tileMemorySub;
static u16* mapMemorySub;

int playerPosition = 1;

//---------------------------------------------------------------------------------

void int_timer_0()
{
    globalTimer += 1; //Global timer for events
}
void int_timer_1()
{
    foxyCooldown += 1; // Foxy Timer when looked on cameras (movement cooldown)
}
void int_timer_2()
{
    foxyDeathTimer += 1; // Foxy Attack Timer
}
void int_timer_3()
{
    freedyMovementTimer += 1; // Freddy Movement Timer
}

void ConfigureTimers()
{
    /*
        T 1     = 32768
        T 2     = 0
        T 0.5   = 49152
        T 0.25  = 58982
    */
    irqEnable(IRQ_TIMER0);
    irqSet(IRQ_TIMER0, int_timer_0);

    irqEnable(IRQ_TIMER1);
    irqSet(IRQ_TIMER1, int_timer_1);

    irqEnable(IRQ_TIMER2);
    irqSet(IRQ_TIMER2, int_timer_2);

    irqEnable(IRQ_TIMER3);
    irqSet(IRQ_TIMER3, int_timer_3);

    TIMER_DATA(0) = 32768; // 1 second
    TIMER_DATA(1) = 32768; // 1 second
    TIMER_DATA(2) = 32768; // 1 second
    TIMER_DATA(3) = 32768; // 1 second

    // Enable timers
    TIMER_CR (0) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;
    TIMER_CR (1) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;
    TIMER_CR (2) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;
    TIMER_CR (3) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;
}

//---------------------------------------------------------------------------------

void ConfigureScreen()
{
    REG_POWERCNT = (vu16) POWER_LCD | POWER_ALL_2D; // Turn on the screens
	REG_DISPCNT  = MODE_FB0 | DISPLAY_BG0_ACTIVE; // Set main LCD mode to Images mode
    REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE; // Set sub LCD mode to 2D mode, tiles compatible

	VRAM_A_CR    = VRAM_ENABLE | VRAM_A_LCD ;
    VRAM_B_CR    = VRAM_ENABLE | VRAM_B_LCD ;
    VRAM_C_CR    = VRAM_ENABLE | VRAM_C_SUB_BG;
	// BGCTRL [0]   = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	tileMemory = (u8*)  BG_TILE_RAM(1);
	mapMemory  = (u16*) BG_MAP_RAM(0);

    tileMemorySub = (u8*)  BG_TILE_RAM_SUB(1);
    mapMemorySub  = (u16*) BG_MAP_RAM_SUB(0);

    BG_PALETTE_SUB[0] = RGB15(255*0/255,  255*0/255, 255*0/255);        // black
    BG_PALETTE_SUB[1] = RGB15(255*31/255, 255*31/255, 255*31/255);      // white
    // 221, 221, 221
    BG_PALETTE_SUB[2] = RGB15(221*31/255, 221*31/255, 221*31/255);      // grey
    BG_PALETTE_SUB[3] = RGB15(255*31/255, 0*31/255, 0*31/255);          // Red / Critical Battery
    // rgb(0, 173, 29)
    BG_PALETTE_SUB[4] = RGB15(0*31/255, 173*31/255, 29*31/255);         // Green Almost Full Battery
    // rgb(109, 204, 0)
    BG_PALETTE_SUB[5] = RGB15(109*31/255, 204*31/255, 0*31/255);        // Green Half Battery
    // rgb(219, 219, 1)
    BG_PALETTE_SUB[6] = RGB15(219*31/255, 219*31/255, 1*31/255);        // Green Low Battery
    // rgb(255, 159, 0)
    BG_PALETTE_SUB[7] = RGB15(255*31/255, 159*31/255, 0*31/255);        // Orange Very Low Battery
    // rgb(79, 75, 131)
    BG_PALETTE_SUB[8] = RGB15(79*31/255, 75*31/255, 131*31/255);        // magenta
    // yellow
    BG_PALETTE_SUB[9] = RGB15(255*31/255, 255*31/255, 0*31/255);        // yellow

    // BG_PALETTE_SUB[0] = RGB15(255*5/255,  255*5/255, 255*5/255); // black
    // BG_PALETTE_SUB[1] = RGB15(255*31/255, 255*31/255, 255*31/255); // white

    int tileMultiplier = 0;

    /*  Solution to simplify the introduction of tiles with a loop. However, because sizeof(),
        sizeof(vectorTeselas[i]) is a pointer, it gives the size of the pointer, not the tile itself.

        u8* vectorTeselas[] = {black , white, grey, CA, AM, cam1, cam2, cam3, cam4, cam5, cam6, cam7, fullBat, mostlyFullBat, halfBat, almostEmptyBat, criticalBat};
        int teselasSize = sizeof(vectorTeselas) / sizeof(vectorTeselas[0]);
        for (int i = 0; i < teselasSize; i++)
        {
            dmaCopy(vectorTeselas[i], tileMemory + 64 * tileMultiplier, sizeof(vectorTeselas[i]));
            tileMultiplier++;
        }
    */
    // Solution provided by Copilot, make the size constant.
    const int ARRAY_SIZE = 64;

    u8* tilesArray[] =  {
                            black ,             //0
                            white,              //1
                            grey,               //2
                            CA,                 //3
                            AM,                 //4
                            cam1,               //5
                            cam2,               //6
                            cam3,               //7
                            cam4,               //8
                            cam5,               //9
                            cam6,               //10
                            cam7,               //11
                            fullBat,            //12
                            mostlyFullBat,      //13
                            halfBat,            //14
                            almostEmptyBat,     //15
                            criticalBat,        //16
                            A,                  //17
                            B,                  //18
                            C,                  //19
                            T,                  //20
                            arrow_down,         //21
                            arrow_left,         //22
                            arrow_right,        //23
                            arrow_up,           //24
                            yellow,             //25
                            magenta,            //26
                            M,                  //27

                        };

    int tilesSize = sizeof(tilesArray) / sizeof(tilesArray[0]);
    for (int i = 0; i < tilesSize; i++)
    {
        dmaCopy(tilesArray[i], tileMemorySub + 64 * tileMultiplier, ARRAY_SIZE);
        tileMultiplier++;
    }

    /*  Primitive solution to introduce tiles one by one.
        dmaCopy(black,      tileMemory      + 64 * tileMultiplier  , sizeof(black));    tileMultiplier++;        //0
        dmaCopy(white,      tileMemory      + 64 * tileMultiplier,   sizeof(white));    tileMultiplier++;//1
        dmaCopy(grey,       tileMemory      + 64 * tileMultiplier,   sizeof(grey));     tileMultiplier++;//2

        dmaCopy(CA,         tileMemory      + 64 * tileMultiplier,   sizeof(CA));       tileMultiplier++; //3
        dmaCopy(AM,         tileMemory      + 64 * tileMultiplier,   sizeof(AM));       tileMultiplier++;//4

        dmaCopy(cam1,       tileMemory      + 64 * tileMultiplier,   sizeof(cam1));     tileMultiplier++;//5
        dmaCopy(cam2,       tileMemory      + 64 * tileMultiplier,   sizeof(cam2));     tileMultiplier++;//6
        dmaCopy(cam3,       tileMemory      + 64 * tileMultiplier,   sizeof(cam3));     tileMultiplier++;//7
        dmaCopy(cam4 ,      tileMemory      + 64 * tileMultiplier,   sizeof(cam4));     tileMultiplier++;//8
        dmaCopy(cam5 ,      tileMemory      + 64 * tileMultiplier,   sizeof(cam5));     tileMultiplier++;//9
        dmaCopy(cam6 ,      tileMemory      + 64 * tileMultiplier,   sizeof(cam6));     tileMultiplier++; //10
        dmaCopy(cam7 ,      tileMemory      + 64 * tileMultiplier,   sizeof(cam7));     tileMultiplier++; //11

        dmaCopy(fullBat,        tileMemory  + 64 * tileMultiplier,   sizeof(fullBat));         tileMultiplier++; //12
        dmaCopy(mostlyFullBat,  tileMemory  + 64 * tileMultiplier,   sizeof(mostlyFullBat));   tileMultiplier++; //13
        dmaCopy(halfBat,        tileMemory  + 64 * tileMultiplier,   sizeof(halfBat));         tileMultiplier++; //14
        dmaCopy(almostEmptyBat, tileMemory  + 64 * tileMultiplier,   sizeof(almostEmptyBat));  tileMultiplier++; //15
        dmaCopy(criticalBat,    tileMemory  + 64 * tileMultiplier,   sizeof(criticalBat));     tileMultiplier++; //16
    */

    lcdMainOnTop(); // Switch sub screen to main, so touch is handled on bottom screen on a real DS
}

//---------------------------------------------------------------------------------
mm_sound_effect pump;
/**
 * Sounds and songs were not implemented due to time constraints.
*/
void ConfigureSound()
{
    mmInitDefaultMem((mm_addr)soundbank_bin);
	mmSetModuleVolume(0);
	// load the module
	mmLoad( MOD_THEPENIS);
	// load sound effects
	mmLoadEffect( SFX_PUMP );

    // Song is a test, it's called like this
    mmStart( MOD_THEPENIS, MM_PLAY_LOOP );

    pump = {
        { SFX_PUMP } ,			// id
        (int)(1.0f * (1<<10)),	// rate
        0,		// handle
        255,	// volume
        128,	// panning
	};
}

/**
 * Reset the game variables to their initial state.
 * Works as a reset button for the game, so it can be reset without rebooting the game.
*/
void Reset()
{
    // Reset timers
    globalTimer = 0;
    foxyCooldown = 0;
    foxyDeathTimer = 0;
    freedyMovementTimer = 0;

    // Reset progress variables
    gracePeriod = true;
    foxyEnabled = false;
    freedyEnabled = false;

    // Reset game variables
    camChecking = 0;
    camerasUp = 0;

    // Reset timer viariables
    aiCalled = false;
    batteryDrain = false;

    // Reset player position
    playerPosition = 1;
}

void Controls(GameState& gameState)
{
    u32 key_down;
    u32 key_held;
    scanKeys();
    key_down = keysDown();
    key_held = keysHeld();

    if (key_down & KEY_START)
    {
        Reset();
        gameState.Reset();
    }

    /**
     * Player movement and interaction with the doors.
    */
    if (playerPosition != 3)
    {
        camerasUp = false;
        if (key_down & KEY_LEFT)
        {
            if (playerPosition == 0)
                playerPosition = 0;
            else
            playerPosition -= 1;
        }
        if (key_down & KEY_RIGHT )
        {
            if (playerPosition == 2)
                playerPosition = 2;
            else
                playerPosition += 1;
        }
        camChecking = 0;
    }

    if ((key_down & KEY_UP) || (key_down & KEY_A))
    {
        if (playerPosition == 0)
            gameState.leftDoor = !gameState.leftDoor;
        else if (playerPosition == 2)
            gameState.rightDoor = !gameState.rightDoor;
    }

    if ((key_down & KEY_DOWN) || (key_down & KEY_B))
    {

        if (playerPosition == 1)
        {
            playerPosition = 3;
            camChecking = 1;
        }
        else if (playerPosition == 3)
        {
            playerPosition = 1;
        }
    }

    /**
     * Player interaction with the touch screen.
    */
	touchRead(&touchPosXY); // Read current stylus position

    /**
     * Touch position coordinate divided by eight to get the position as a tile position.
    */
    int posX = touchPosXY.px / 8; int posY = touchPosXY.py / 8;

    if (key_down & KEY_TOUCH)
    {
        // Go left
        if ((posX == 0 || posX == 1)    && (posY >= 2 && posY <= 22)){
            if (playerPosition == 1)
                playerPosition = 0;
            if (playerPosition == 2)
                playerPosition = 1;
        }

        // Go right
        if ((posX == 30 || posX == 31)  && (posY >= 2 && posY <= 22))
        {
            if (playerPosition == 1)
                playerPosition = 2;
            if (playerPosition == 0)
                playerPosition = 1;
        }

        // Open camera monitor
        if ((posX >= 10 && posX <= 20)  && (posY >= 21 && posY <= 23)) {
            playerPosition = 3;
            camChecking = 1;
        }

        if ((posX == 16 && posY == 12) && (playerPosition == 0)){
            gameState.leftDoor = !gameState.leftDoor;
        }
        else if ((posX == 15 && posY == 10) && (playerPosition == 2)){
            gameState.rightDoor = !gameState.rightDoor;
        }

        // nocashDebug(gameState, camChecking, "CamChecking");
    }

    if (key_held & KEY_TOUCH)
    {
        // Exit camera monitor
        if ((posX >= 1 && posX <= 7)    && (posY >= 21 && posY <= 23)) {
            playerPosition = 1;
        }

        if (playerPosition == 3)
        {
            camerasUp = true;
            // Check CAM1A
            if ((posX == 11 || posX == 12) && (posY == 1 || posY == 2)          ){
                camChecking = 1;
            }
            // Check CAM1B
            else if ((posX == 11 || posX == 12) && (posY == 6 || posY == 7)     ){
                camChecking = 2;
            }
            // Check CAM1C
            else if ((posX == 5 || posX == 6) && (posY == 9 || posY == 10)      ){
                camChecking = 3;
            }
            // Check CAM5A
            else if ((posX == 2 || posX == 3) && (posY == 2 || posY == 3)       ){
                camChecking = 4;
            }
            // Check CAM5B
            else if ((posX == 27 || posX == 28) && (posY == 3 || posY == 4)     ){
                camChecking = 5;
            }
            // Check CAM2A
            else if ((posX == 10 || posX == 11) && (posY == 14 || posY == 15)   ){
                camChecking = 6;
            }
            // Check CAM2B
            else if ((posX == 10 || posX == 11) && (posY == 19 || posY == 20)   ){
                camChecking = 7;
            }
            // Check CAM4A
            else if ((posX == 19 || posX == 20) && (posY == 14 || posY == 15)   ){
                camChecking = 8;
            }
            // Check CAM4B
            else if ((posX == 19 || posX == 20) && (posY == 19 || posY == 20)   ){
                camChecking = 9;
            }
            // Check CAM3
            else if ((posX == 4 || posX == 5) && (posY == 17 || posY == 18)     ){
                camChecking = 10;
            }
            // Check CAM6
            else if ((posX == 29 || posX == 30) && (posY == 14 || posY == 15)   ){
                camChecking = 11;
            }
        }
    }

    // Trucoteca.com cheat code
    if (key_down & KEY_R)
    {
        gameState.batteryLevel = 100;
        gameState.leftDoor = true;
        gameState.rightDoor = true;
    }
}

//---------------------------------------------------------------------------------
/**
 * Setup game at first boot.
*/
void SetUp(GameState& gameState)
{
    ConfigureScreen();
    ConfigureTimers();
    ConfigureSound();
    Controls(gameState);
    Reset();
    gameState.Reset();
}

//---------------------------------------------------------------------------------
int pastTime=0;
/**
 * Update loop for the main core of the game.
 * @tparam Render Calls the tiles renderer.
 * @tparam RenderImages Renders the images on the screen.
 * @tparam SecondaryRender Renders the secondary screen. Related to the camera monitor.
*/
void Update(GameState& gameState, GameRenderer& gameRenderer)
{

    gameRenderer.Render(playerPosition, mapMemorySub, camChecking, gameState.bonnieAI.position, gameState.chicaAI.position, gameState.freddyAI.position, gameState.foxyAI.position, gameState.leftDoor, gameState.rightDoor);
    gameRenderer.RenderImages(camChecking, gameState.bonnieAI.position, gameState.chicaAI.position, gameState.freddyAI.position, gameState.foxyAI.position, gameState.leftDoor, gameState.rightDoor);
    gameRenderer.SecondaryRender(gameState.batteryLevel, globalTimer);
    Controls(gameState);
    consoleClear();

    // If the game is set on consoleMode, it will display the debug information on the console.
    // Use nocashDebug() to display the information on the debug console.
    iprintf("\x1b[2;2HDebug: %d", debug);
    // 3
    // 4
    // 5
    iprintf("\x1b[7;2HPosicion x=%04i y=%04i ",
        touchPosXY.px / 8,
        touchPosXY.py / 8);

    iprintf("\x1b[8;2HBonniePos  : %d", gameState.bonnieAI.position);
    iprintf("\x1b[9;2HChicaPos   : %d", gameState.chicaAI.position);
    iprintf("\x1b[10;2HFreddyPos  : %d", gameState.freddyAI.position);
    iprintf("\x1b[11;2HFoxyPos    : %d", gameState.foxyAI.position);

    iprintf("\x1b[12;2HCamerascheck   : %d", camChecking);

    iprintf("\x1b[14;2HBattery    : %d", gameState.batteryLevel);
    iprintf("\x1b[15;2HTime       : %d", globalTimer);
    iprintf("\x1b[16;2HFoxy Timer : %d", foxyCooldown);
    iprintf("\x1b[17;2HFoxyDeath  : %d", foxyDeathTimer);
    iprintf("\x1b[18;2HFoxylock   : %d", gameState.lockFoxy);


    // Reduce battery level every mutliple of 4 seconds
    if (globalTimer % 4 == 0)
    {
        if (!batteryDrain) // Prevent multiple draining operations
        {
            batteryDrain = true;
            gameState.batteryLevel -= 1;
        }
    }
    else batteryDrain = false;

    // Gives the player breathing room to get used to the game
    if (globalTimer == 15)
    {
        gracePeriod = false;
    }

    // Locks Foxy while the player is looking at the cameras
    if (camChecking == 3){ gameState.lockFoxy = true; }
    else { gameState.lockFoxy = false; foxyCooldown = 0; }

    // After grace period is over, the game starts.
    if (gracePeriod == false)
    {
        // Give the animatronics a movement oportunity every multiple of 5 seconds
        if (globalTimer % 5 == 0)
        {
            if (!aiCalled)
            {
                aiCalled = true;
                gameState.CallAI();
            }
        }
        else aiCalled = false;

        if (gameState.foxyAI.position == 4 && camChecking == 3)
        {
            timerUnpause(2);
        }

        if (foxyDeathTimer > 7) // If timers reaches 7 seconds, check possibilities
        {
            nocashMessage("Foxy running");
            if (gameState.leftDoor == false)
            {
                nocashMessage("Foxy has killled player");
                gameState.whoKilled = 22; // Set render to show Foxy jumpscare
                gameState.gameOver = true;
            }
            else if (gameState.leftDoor == true)
            {
                nocashMessage("Foxy returns to Cove");
                gameState.foxyAI.position = 0;
                timerStop(2);
                foxyDeathTimer = 0;
            }
        }
    }


    if (gameState.batteryLevel == 0) // End game when battery reaches 0, Freddy scene is not implemented.
    {
        gameState.gameOver = true;

    }

    if (globalTimer == 300) // End game after 5 minutes
    {
        gameState.gameOver = true;

    }

   /**
    * Debugging function to display the game state variables on the debug console.
    */
    if (pastTime != globalTimer)
    {
        nocashMessage("-------------");
        nocashDebug(gameState, globalTimer, "Time");
        nocashDebug(gameState, gameState.batteryLevel, "Battery:");
        nocashDebug(gameState, gameState.bonnieAI.position, "BonniePos");
        nocashDebug(gameState, gameState.chicaAI.position, "ChicaPos");
        nocashDebug(gameState, gameState.freddyAI.position, "FreddyPos");
        nocashDebug(gameState, gameState.foxyAI.position, "FoxyPos");
        nocashMessage("-------------");
        nocashMessage("Foxy Disabled");
        if (foxyDeathTimer != 0)
            nocashDebug(gameState, foxyDeathTimer, "FoxyDeath");
        nocashMessage("-------------");
        if (gameState.chicaAI.position == 7)
            nocashMessage("Chica at the door");
        if (gameState.bonnieAI.position == 7)
            nocashMessage("Bonnie at the door");
        if (gameState.freddyAI.position == 7)
            nocashMessage("Freddy at the door");
        if (gameState.foxyAI.position == 7)
            nocashMessage("Foxy at the door");
        nocashMessage("-------------");
        nocashDebug(gameState, camerasUp, "CamerasUp");
        nocashDebug(gameState, batteryDrain, "BatteryDrain");
    }
    pastTime = globalTimer;
}

//---------------------------------------------------------------------------------

int main( void )
{
    GameState gameState;
    GameRenderer gameRenderer;

    SetUp(gameState);

    timerPause(1); // Stop Foxy timer at the beginning of the game
    timerPause(2); // Stop Foxy attack timer at the beginning of the game
    timerPause(3); // reddy movement timer at the beginning of the game (not implemented)

	while(1) // Game Main Loop
	{
        if (gameState.IsTerminal()) // When game is over...
        {
            u32 key;
            scanKeys();
            key = keysDown();

            consoleClear();
            iprintf("\x1b[20;2HGame Over");

            // Print time lasted
            iprintf("\x1b[18;2HTime: %d", globalTimer);

            // Stop all timers
            irqDisable(IRQ_TIMER0);
            irqDisable(IRQ_TIMER1);
            irqDisable(IRQ_TIMER2);
            irqDisable(IRQ_TIMER3);

            // Check if the player has lost to an animatronic
            if (gameState.whoKilled != 0)
            {
                gameRenderer.Render(11, mapMemorySub, camChecking, gameState.bonnieAI.position, gameState.chicaAI.position, gameState.freddyAI.position, gameState.foxyAI.position, gameState.leftDoor, gameState.rightDoor);

                gameRenderer.RenderJumpscare(gameState.whoKilled);
            }
            else // If the player has emptied the battery
            {
                gameRenderer.Render(10, mapMemorySub, camChecking, gameState.bonnieAI.position, gameState.chicaAI.position, gameState.freddyAI.position, gameState.foxyAI.position, gameState.leftDoor, gameState.rightDoor);
            }

            // Restart the game
            if (key & KEY_START)
            {
                Reset();
                gameState.Reset();
            }
        }
        else // When game is running / not over...
        {
            Update(gameState, gameRenderer);
        }

        // Update(gameState, gameRenderer);
        swiWaitForVBlank();
	}
}
