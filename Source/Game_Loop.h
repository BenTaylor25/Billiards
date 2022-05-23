void Loop_Pool(olc::PixelGameEngine* olcfunc, Pool_Rules* Pool_Game, C_Ball* PBalls, C_Pocket* Pockets, char* mode, char* loopState, M_Vector* aimPivot, M_Vector* dynamicPivot, M_Rect cloth, int SW, int SH)
{
    switch (*loopState)
    {

        // Aiming
    case 'a':
    {
        float pivotDist = GetVectorDistance_P(aimPivot, dynamicPivot);
        *loopState = CheckStrike(olcfunc, PBalls, aimPivot, pivotDist, SW);
        *dynamicPivot = MovePivot(olcfunc, aimPivot, SW, SH);

        DrawTable(olcfunc, Pockets, false, cloth, SW, SH);
        DrawBalls(olcfunc, PBalls, 16);
        DrawAim(olcfunc, aimPivot, dynamicPivot, SW, SH);
        DrawHUD(olcfunc, *Pool_Game, SW, SH);
    }
    break;


    // Watching
    case 'w':
    {
        ApplyAndDecayVelocity(PBalls, 16);
        std::vector<int> pocketedBalls = CheckPockets(Pockets, PBalls, 16);
        if (pocketedBalls.size() > 0) { Pool_Game->AddPocketedBalls(pocketedBalls); }
        CollideWalls(PBalls, 16, cloth, SW, SH);
        CollideBalls(PBalls, 16);
        *loopState = CheckMovingBalls(PBalls, 16);
        if (*loopState == 'a')
        {
            if (not Pool_Game->GetIsTableOpen()) { Pool_Game->CheckTargets(PBalls); }
            Pool_Game->CheckPocketedBalls(PBalls, SW, SH);
            *loopState = Pool_Game->CheckEndGame();
            Pool_Game->ResetTurnAttributes();
        }

        DrawTable(olcfunc, Pockets, false, cloth, SW, SH);
        DrawBalls(olcfunc, PBalls, 16);
        DrawHUD(olcfunc, *Pool_Game, SW, SH);

    }
    break;


    // Results
    case 'r':
    {
        bool playerOneWon = Pool_Game->GetPlayerOneWin();
        *loopState = DrawResults(olcfunc, playerOneWon, mode, SW, SH);
        if (*loopState == 'a')
        {
            Pool_Game->ResetAllAttributes();
            Pool_BallSetup(PBalls, SW, SH);
        }
    }
    break;

    }

}



void Loop_Snooker(olc::PixelGameEngine* olcfunc, Snooker_Rules* Snooker_Game, C_Ball* SBalls, C_Pocket* Pockets, char* mode, char* loopState, M_Vector* aimPivot, M_Vector* dynamicPivot, M_Rect cloth, int SW, int SH)
{
    switch (*loopState)
    {
        // Aiming
    case 'a':
    {
        float pivotDist = GetVectorDistance_P(aimPivot, dynamicPivot);
        *loopState = CheckStrike(olcfunc, SBalls, aimPivot, pivotDist, SW);
        *dynamicPivot = MovePivot(olcfunc, aimPivot, SW, SH);

        DrawTable(olcfunc, Pockets, true, cloth, SW, SH);
        DrawBalls(olcfunc, SBalls, 22);
        DrawAim(olcfunc, aimPivot, dynamicPivot, SW, SH);
        DrawHUD(olcfunc, *Snooker_Game, SW, SH);
    }
    break;


    case 'w':
    {
        ApplyAndDecayVelocity(SBalls, 22);
        std::vector<int> pocketedBalls = CheckPockets(Pockets, SBalls, 22);
        if (pocketedBalls.size() > 0) { Snooker_Game->AddPocketedBalls(pocketedBalls); }
        CollideWalls(SBalls, 22, cloth, SW, SH);
        CollideBalls(SBalls, 22);
        *loopState = CheckMovingBalls(SBalls, 22);
        if (*loopState == 'a')
        {
            Snooker_Game->CheckPhaseChange(SBalls);
            Snooker_Game->CheckPocketedBalls(SBalls, SW, SH);
            *loopState = Snooker_Game->CheckEndGame(SBalls);
            Snooker_Game->ResetTurnAttributes();
        }

        DrawTable(olcfunc, Pockets, true, cloth, SW, SH);
        DrawBalls(olcfunc, SBalls, 22);
        DrawHUD(olcfunc, *Snooker_Game, SW, SH);
    }
    break;


    case 'r':
    {
        bool playerOneWon = Snooker_Game->GetPlayerOneWin();
        *loopState = DrawResults(olcfunc, playerOneWon, mode, SW, SH);
        if (*loopState == 'a')
        {
            Snooker_Game->ResetAllAttributes();
            Snooker_BallSetup(SBalls, SW, SH);
        }
    }
    break;

    }

}

