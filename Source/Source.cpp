#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Setup.h"
#include "Menu_Screens.h"
#include "Game_Rules.h"
#include "Game_FrontEnd.h"
#include "Game_BackEnd.h"
#include "Game_Loop.h"


int menuScroller = 0;
C_Ball PBalls[16];
C_Ball SBalls[22];
C_Pocket Pockets[6];
M_Vector aimPivot;
M_Vector dynamicPivot;
M_Rect cloth;
Pool_Rules Pool_Game;
Snooker_Rules Snooker_Game;

std::vector<std::string> helpText = getHelpText();
std::vector<std::string> creditsText = getCreditsText();


//////////////////////////
// Modes:               //
//   m: menus,          //
//   $: setup,          //
//   p: play pool,      //
//   s: play snooker.   //
// -------------------- //
// Menu Screen:         //
//   m: main menu       //
//   f: freeplay menu   //
//   h: help menu       //
//   c: credits menu    //
// -------------------- //
// Loop States:         //
//   a: aiming,         //
//   w: watching.       //
//////////////////////////
char mode = '$';
char menuScreen = 'm';
char loopState = 'a';

bool keepGameOpen = true;



class WINDOW : public olc::PixelGameEngine
{
public:
    WINDOW() { sAppName = "Billiards"; }

    bool OnUserCreate() override { return true; }

    bool OnUserUpdate(float fElapsedTime) override
    {
        switch (mode)
        {
        case 'm':
        {
            switch (menuScreen)
            {
            case 'm':
                keepGameOpen = DisplayMainMenu(this, &menuScreen, ScreenWidth(), ScreenHeight());
                break;
            case 'f':
                mode = DisplayFreeplayMenu(this, &menuScreen, ScreenWidth(), ScreenHeight());
                break;
            case 'h':
                menuScroller = DisplayHelpMenu(this, &menuScreen, ScreenWidth(), ScreenHeight(), helpText, menuScroller);
                break;
            case 'c':
                menuScroller = DisplayCreditsMenu(this, &menuScreen, ScreenWidth(), ScreenHeight(), creditsText, menuScroller);
                break;
            }
        }
        break;

        case '$':
        {
            cloth = SizeCloth(cloth, ScreenWidth(), ScreenHeight());
            PocketSetup(Pockets, ScreenWidth(), ScreenHeight());
            Pool_BallSetup(PBalls, ScreenWidth(), ScreenHeight());
            Snooker_BallSetup(SBalls, ScreenWidth(), ScreenHeight());
            Pool_Game.ResetAllAttributes();
            Snooker_Game.ResetAllAttributes();
            mode = 'm';
        }
        break;

        case 'p':
        {
            Loop_Pool(this, &Pool_Game, PBalls, Pockets, &mode, &loopState, &aimPivot, &dynamicPivot, cloth, ScreenWidth(), ScreenHeight());
        }
        break;

        case 's':
        {
            Loop_Snooker(this, &Snooker_Game, SBalls, Pockets, &mode, &loopState, &aimPivot, &dynamicPivot, cloth, ScreenWidth(), ScreenHeight());
        }
        break;

        }

        return keepGameOpen;
    }
};


int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    WINDOW Billiards;
    if (Billiards.Construct(640, 360, 2, 2))
    {
        Billiards.Start();
    }
}
