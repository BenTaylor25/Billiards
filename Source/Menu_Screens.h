void DrawBackground(olc::PixelGameEngine* olcfunc, int SW, int SH)
{
    // Side Fade
    for (int x = 0; x < SW; x++)
    {
        int fade = std::abs((SW / 2) - x);
        fade -= SW / 3;
        if (fade < 0) { fade = 0; }
        fade *= 1.1;
        if (fade > 100) { fade = 100; }

        olcfunc->DrawLine(x, 0, x, SH, olc::Pixel(125 - fade, 255 - fade, 125 - fade));
    }
}


int Scroll(olc::PixelGameEngine* olcfunc, int mouseX, int mouseY, int SW, int SH, int menuScroller, int upperBound)
{
    bool holdingLeftClick = olcfunc->GetMouse(0).bHeld;
    bool mouseOnScrollBar = (mouseX > SW * 0.9f) ? (mouseX < SW * 0.975f) : false;   // Conditional Operator

    if (holdingLeftClick && mouseOnScrollBar)
    {
        // Scroll to the right part of the page.
        menuScroller = (mouseY - (SH * 0.1)) / (SH * 0.8f) * upperBound;
    }

    menuScroller += olcfunc->GetMouseWheel() / -15;

    if (menuScroller < 0) { menuScroller = 0; }
    else if (menuScroller > upperBound) { menuScroller = upperBound; }

    return menuScroller;
}


bool DisplayMainMenu(olc::PixelGameEngine* olcfunc, char* menuScreen, int SW, int SH)
{
    float displacement = SH / 6;

    M_Rect fplyBox = MakeRectangle(SW / 4, SH / 7 * 2 + (0 * displacement), SW / 5, SH / 20, olc::RED);
    M_Rect helpBox = MakeRectangle(SW / 4, SH / 7 * 2 + (1 * displacement), SW / 5, SH / 20, olc::RED);
    M_Rect crdsBox = MakeRectangle(SW / 4, SH / 7 * 2 + (2 * displacement), SW / 5, SH / 20, olc::RED);
    M_Rect quitBox = MakeRectangle(SW / 4, SH / 7 * 2 + (3 * displacement), SW / 5, SH / 20, olc::RED);

    int mouseX = olcfunc->GetMouseX();
    int mouseY = olcfunc->GetMouseY();
    int leftBound = fplyBox.x;
    int rightBound = fplyBox.x + fplyBox.w;

    bool leftClicked = olcfunc->GetMouse(0).bReleased;

    if (leftClicked && mouseX >= leftBound && mouseX <= rightBound)
    {
        if (mouseY >= fplyBox.y && mouseY <= fplyBox.y + fplyBox.h) { *menuScreen = 'f'; }
        else if (mouseY >= helpBox.y && mouseY <= helpBox.y + helpBox.h) { *menuScreen = 'h'; }
        else if (mouseY >= crdsBox.y && mouseY <= crdsBox.y + crdsBox.h) { *menuScreen = 'c'; }
        else if (mouseY >= quitBox.y && mouseY <= quitBox.y + quitBox.h) { return false; }      // Close Game
    }

    DrawBackground(olcfunc, SW, SH);

    // Text
    olcfunc->DrawString(SW / 4, SH / 10, "Billiards", olc::BLACK, SW / 180);
    olcfunc->DrawString(fplyBox.x, fplyBox.y, "Freeplay", olc::BLACK, SW / 250);
    olcfunc->DrawString(helpBox.x, helpBox.y, "Help", olc::BLACK, SW / 250);
    olcfunc->DrawString(crdsBox.x, crdsBox.y, "Credits", olc::BLACK, SW / 250);
    olcfunc->DrawString(quitBox.x, quitBox.y, "Quit", olc::BLACK, SW / 250);

    return true;
}


char DisplayFreeplayMenu(olc::PixelGameEngine* olcfunc, char* menuScreen, int SW, int SH)
{
    float displacement = SH / 6;

    M_Rect poolBox = MakeRectangle(SW / 4, SH / 3 + (0 * displacement), SW / 5, SH / 20, olc::RED);
    M_Rect snkrBox = MakeRectangle(SW / 4, SH / 3 + (1 * displacement), SW / 5, SH / 20, olc::RED);
    M_Rect backBox = MakeRectangle(SW / 40, SH / 20 * 19, SW / 20, SH / 40, olc::RED);

    int mouseX = olcfunc->GetMouseX();
    int mouseY = olcfunc->GetMouseY();

    bool leftClicked = olcfunc->GetMouse(0).bReleased;
    bool inBackXRange = (mouseX >= backBox.x) && (mouseX <= backBox.x + backBox.w);
    bool inBackYRange = (mouseY >= backBox.y) && (mouseY <= backBox.y + backBox.h);
    bool inGameXRange = (mouseX >= poolBox.x) && (mouseX <= poolBox.x + poolBox.w);

    if (leftClicked)
    {
        if (inBackXRange && inBackYRange)
        {
            // Return to main menu
            *menuScreen = 'm';
        }
        else if (inGameXRange)
        {
            if (mouseY >= poolBox.y && mouseY <= poolBox.y + poolBox.h)
            {
                *menuScreen = 'm';
                return 'p';
            }
            else if (mouseY >= snkrBox.y && mouseY <= snkrBox.y + snkrBox.h)
            {
                *menuScreen = 'm';
                return 's';
            }
        }
    }

    DrawBackground(olcfunc, SW, SH);

    // Text
    olcfunc->DrawString(SW / 4, SH / 10, "Freeplay", olc::BLACK, SW / 180);

    olcfunc->DrawString(poolBox.x, poolBox.y, "Pool", olc::BLACK, SW / 250);
    olcfunc->DrawString(snkrBox.x, snkrBox.y, "Snooker", olc::BLACK, SW / 250);

    olcfunc->DrawString(backBox.x, backBox.y, "Back", olc::BLACK, SW / 325);

    return 'm';
}


int DisplayHelpMenu(olc::PixelGameEngine* olcfunc, char* menuScreen, int SW, int SH, std::vector<std::string> helpText, int menuScroller)
{
    int mouseX = olcfunc->GetMouseX();
    int mouseY = olcfunc->GetMouseY();

    // Scroll Page
    int upperBound = 1800;
    menuScroller = Scroll(olcfunc, mouseX, mouseY, SW, SH, menuScroller, upperBound);

    // Back button
    M_Rect backBox = MakeRectangle(SW / 40, SH / 20 * 19, SW / 20, SH / 40, olc::RED);

    bool leftClicked = olcfunc->GetMouse(0).bReleased;
    bool inXRange = (mouseX >= backBox.x) && (mouseX <= backBox.x + backBox.w);
    bool inYRange = (mouseY > backBox.y) && (mouseY < backBox.y + backBox.h);

    if (leftClicked && inXRange && inYRange)
    {
        // Scroll to the top of the page and return to main menu
        menuScroller = 0;
        *menuScreen = 'm';
    }

    DrawBackground(olcfunc, SW, SH);

    // Scroll Bar Visuals
    olcfunc->DrawLine(SW * 0.95f, SH * 0.1f, SW * 0.95f, SH * 0.9f, olc::Pixel(50, 50, 50));
    olcfunc->FillRect(SW * 0.95f - 2, SH * 0.1f + (SH * 0.8f) * ((float)menuScroller / upperBound) - (SW * 0.02f), 5, SW * 0.03f, olc::BLACK);


    // Text
    olcfunc->DrawString(SW / 4, (SH / 10) - menuScroller, "Help", olc::BLACK, SW / 160);

    olcfunc->DrawString(SW / 4, (SH / 10 * 3) - menuScroller, "What is Billiards?", olc::BLACK, SW / 400);
    olcfunc->DrawString(SW / 4, (SH / 10 * 4) - menuScroller, helpText[0], olc::BLACK, SW / 480);

    olcfunc->DrawString(SW / 4, (SH / 10 * 7.75) - menuScroller, "Pool (8-Ball)", olc::BLACK, SW / 400);
    olcfunc->DrawString(SW / 4, (SH / 10 * 8.5) - menuScroller, helpText[1], olc::BLACK, SW / 480);

    olcfunc->DrawString(SW / 4, (SH * 3) - menuScroller, "Snooker", olc::BLACK, SW / 400);
    olcfunc->DrawString(SW / 4, (SH * 3.075) - menuScroller, helpText[2], olc::BLACK, SW / 480);

    olcfunc->DrawString(backBox.x, backBox.y, "Back", olc::BLACK, SW / 325);

    return menuScroller;
}


int DisplayCreditsMenu(olc::PixelGameEngine* olcfunc, char* menuScreen, int SW, int SH, std::vector<std::string> creditsText, int menuScroller)
{
    int mouseX = olcfunc->GetMouseX();
    int mouseY = olcfunc->GetMouseY();

    // Scroll Page
    int upperBound = 60;
    menuScroller = Scroll(olcfunc, mouseX, mouseY, SW, SH, menuScroller, upperBound);

    // Back button
    M_Rect backBox = MakeRectangle(SW / 40, SH / 20 * 19, SW / 20, SH / 40, olc::RED);

    bool leftClicked = olcfunc->GetMouse(0).bReleased;
    bool inXRange = (mouseX >= backBox.x) && (mouseX <= backBox.x + backBox.w);
    bool inYRange = (mouseY > backBox.y) && (mouseY < backBox.y + backBox.h);

    if (leftClicked && inXRange && inYRange)
    {
        menuScroller = 0;
        *menuScreen = 'm';
    }

    DrawBackground(olcfunc, SW, SH);

    // Scroll Bar Visuals
    olcfunc->DrawLine(SW * 0.95f, SH * 0.1f, SW * 0.95f, SH * 0.9f, olc::Pixel(50, 50, 50));
    olcfunc->FillRect(SW * 0.95f - 2, SH * 0.1f + (SH * 0.8f) * ((float)menuScroller / upperBound) - (SW * 0.02f), 5, SW * 0.03f, olc::BLACK);


    // Text
    olcfunc->DrawString(SW / 4, (SH / 10) - menuScroller, "Credits", olc::BLACK, SW / 160);

    olcfunc->DrawString(SW / 4, (SH / 4) - menuScroller, "David 'One Lone Coder' Barr", olc::BLACK, SW / 400);
    olcfunc->DrawString(SW / 4, (SH / 3) - menuScroller, creditsText[0], olc::BLACK, SW / 480);

    olcfunc->DrawString(SW / 4, (SH * 0.65f) - menuScroller, "Kate Upson-Smith", olc::BLACK, SW / 400);
    olcfunc->DrawString(SW / 4, (SH * 0.725f) - menuScroller, creditsText[1], olc::BLACK, SW / 480);

    olcfunc->DrawString(backBox.x, backBox.y, "Back", olc::BLACK, SW / 325);

    return menuScroller;
}

