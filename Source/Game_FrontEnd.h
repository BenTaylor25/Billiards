void DrawTable(olc::PixelGameEngine* olcfunc, C_Pocket* Pockets, bool Game_is_Snooker, M_Rect cloth, int SW, int SH)
{
    olcfunc->FillRect(0, 0, SW, SH, olc::Pixel(71, 96, 115));

    olcfunc->FillRect(SW * 0.078f, (SH * 0.153f) - (SW * 0.031), SW * 0.844f, (SH * 0.694f) + (SW * 0.063f), olc::Pixel(128, 47, 54));
    olcfunc->FillRect(cloth.x, cloth.y, cloth.w, cloth.h, olc::Pixel(48, 150, 47));

    if (Game_is_Snooker)
    {
        int width = (SH * 0.694f) - (SH * 0.153f);
        float DRadius = 0.135f * SH;
        olcfunc->DrawCircle(SW * 0.304f, SH * 0.5f, DRadius, olc::BLACK);
        olcfunc->FillRect(SW * 0.304f, SH * 0.5f - DRadius, DRadius + 1, 2 * DRadius + 1, olc::Pixel(48, 150, 47));
    }

    olcfunc->DrawLine(SW * 0.304f, SH * 0.153f, SW * 0.304f, SH * 0.847f, olc::BLACK);
    olcfunc->Draw(SW * 0.696f, SH * 0.5f, olc::BLACK);

    for (int i = 0; i < 6; i++)
    {
        olcfunc->FillCircle(Pockets[i].GetPos().x, Pockets[i].GetPos().y, Pockets[i].GetRad(), olc::BLACK);
    }
}


void DrawBalls(olc::PixelGameEngine* olcfunc, C_Ball* Balls, int Num_of_Balls)
{
    for (int i = 0; i < Num_of_Balls; i++)
    {
        if (Balls[i].IsOnTable())
        {
            olcfunc->FillCircle(Balls[i].GetPos().x, Balls[i].GetPos().y, Balls[i].GetRad(), Balls[i].GetCol());
        }
    }
}


void DrawAim(olc::PixelGameEngine* olcfunc, M_Vector* aimPivot, M_Vector* dynamicPivot, int SW, int SH)
{
    if (olcfunc->GetMouse(0).bHeld)
    {
        olcfunc->DrawCircle(aimPivot[0].x, aimPivot[0].y, SW / 128, olc::Pixel(184, 15, 10));
        olcfunc->DrawCircle(aimPivot[0].x, aimPivot[0].y, SW / 10.667f, olc::Pixel(255, 211, 0));
    }

    olcfunc->FillCircle(aimPivot->x, aimPivot->y, 2, olc::GREY);

    olcfunc->DrawLine(aimPivot->x, aimPivot->y, dynamicPivot->x, dynamicPivot->y, olc::GREY);
    olcfunc->FillCircle(dynamicPivot->x, dynamicPivot->y, 2, olc::GREY);

}


void DrawHUD(olc::PixelGameEngine* olcfunc, Both_Rules The_Game, int SW, int SH)
{
    olc::Pixel p1Col = olc::GREY;
    olc::Pixel p2Col = olc::Pixel(120, 200, 150);
    olc::Pixel p1Target = The_Game.GetP1Target();
    olc::Pixel p2Target = The_Game.GetP2Target();

    if (The_Game.GetIsPlayerOneMove())
    {
        p1Col = olc::Pixel(120, 200, 150);
        p2Col = olc::GREY;
    }

    olcfunc->FillRect(0, SH * 0.925f, SW * 0.2f, SH * 0.075f, olc::Pixel(155, 21, 21));
    olcfunc->FillRect(SW * 0.8f, SH * 0.925f, SW * 0.2f, SH * 0.075f, olc::Pixel(155, 21, 21));

    olcfunc->DrawString(SW * 0.025f, SH * 0.95f, "Player 1", p1Col, SW / 325);
    olcfunc->DrawString(SW * 0.825f, SH * 0.95f, "Player 2", p2Col, SW / 325);

    olcfunc->FillCircle(SW * 0.175f, SH * 0.965f, SW / 100, p1Target);
    olcfunc->FillCircle(SW * 0.975f, SH * 0.965f, SW / 100, p2Target);

    if (The_Game.GetGameIsSnooker())
    {
        olcfunc->DrawString(SW * 0.125f, SH * 0.975f, std::to_string(The_Game.GetP1Points()), p1Col, SW / 375);
        olcfunc->DrawString(SW * 0.925f, SH * 0.975f, std::to_string(The_Game.GetP2Points()), p2Col, SW / 375);
    }
}


char DrawResults(olc::PixelGameEngine* olcfunc, bool playerOneWon, char* mode, int SW, int SH)
{
    std::string prompt = playerOneWon ? "Player 1 Wins!" : "Player 2 Wins!"; // Conditional Operator

    M_Rect backToMenuBox = MakeRectangle(SW * 0.45f, SH * 0.95f, SW * 0.15f, SH * 0.05f, olc::RED);

    int mouseX = olcfunc->GetMouseX();
    int mouseY = olcfunc->GetMouseY();

    bool leftClicked = olcfunc->GetMouse(0).bReleased;
    bool inXRange = (mouseX >= backToMenuBox.x) && (mouseX <= backToMenuBox.x + backToMenuBox.w);
    bool inYRange = (mouseY > backToMenuBox.y) && (mouseY < backToMenuBox.y + backToMenuBox.h);

    if (leftClicked && inXRange && inYRange)
    {
        *mode = 'm';
        return 'a';
    }

    olcfunc->DrawString(SW * 0.25f, SH * 0.333f, prompt, olc::BLUE, SW / 200);
    olcfunc->DrawString(SW * 0.45f, SH * 0.95f, "Back to Menu", olc::BLACK, SW / 325);

    return 'r';
}
