// Interface
class Both_Rules
{
protected:
    bool gameIsSnooker;
    bool isGameOver;
    bool playerOneWin;
    bool isPlayerOneMove;
    olc::Pixel playerOneTarget;
    olc::Pixel playerTwoTarget;
    std::vector<int> pocketedBalls;

    // only used by Snooker
    int playerOnePoints;
    int playerTwoPoints;

public:
    bool GetGameIsSnooker()
    {
        return gameIsSnooker;
    }

    void AddPocketedBalls(std::vector<int> newIndices)
    {
        for (int i = 0; i < newIndices.size(); i++)
        {
            pocketedBalls.push_back(newIndices[i]);
        }
    }

    bool GetPlayerOneWin()
    {
        return playerOneWin;
    }

    bool GetIsPlayerOneMove()
    {
        return isPlayerOneMove;
    }

    olc::Pixel GetP1Target()
    {
        return playerOneTarget;
    }

    olc::Pixel GetP2Target()
    {
        return playerTwoTarget;
    }

    int GetP1Points()
    {
        return playerOnePoints;
    }

    int GetP2Points()
    {
        return playerTwoPoints;
    }

};

class Pool_Rules : public Both_Rules
{
private:
    bool isTableOpen;

public:
    void ResetTurnAttributes()
    {
        pocketedBalls.clear();
    }

    void ResetAllAttributes()
    {
        gameIsSnooker = false;
        isGameOver = false;
        playerOneWin = false;
        isPlayerOneMove = true;
        isTableOpen = true;
        playerOneTarget = olc::GREY;
        playerTwoTarget = olc::GREY;

        ResetTurnAttributes();
    }

    bool GetIsTableOpen()
    {
        return isTableOpen;
    }

    // Should the players start targeting the 8-Ball?
    void CheckTargets(C_Ball* Balls)
    {
        if (not (playerOneTarget == olc::BLACK && playerTwoTarget == olc::BLACK))
        {
            bool areRedsOnTable = false;
            for (int r = 1; r <= 7; r++) { areRedsOnTable = areRedsOnTable || Balls[r].IsOnTable(); }
            bool areYelsOnTable = false;
            for (int y = 8; y <= 14; y++) { areYelsOnTable = areYelsOnTable || Balls[y].IsOnTable(); }

            if (not areRedsOnTable)
            {
                if (playerOneTarget == olc::RED) { playerOneTarget = olc::BLACK; }
                else if (playerTwoTarget == olc::RED) { playerTwoTarget = olc::BLACK; }
            }
            if (not areYelsOnTable)
            {
                if (playerOneTarget == olc::YELLOW) { playerOneTarget = olc::BLACK; }
                else if (playerTwoTarget == olc::YELLOW) { playerTwoTarget = olc::BLACK; }
            }

        }
    }

    void CueBallPocketed(C_Ball* PBalls, int SW, int SH)
    {
        PBalls[0].SetPos(SW * 0.304f, SH * 0.5f);
        PBalls[0].SetVel(0, 0);
        PBalls[0].SetIsOnTable(true);
    }

    void EightBallPocketed()
    {
        if (isTableOpen)
        {
            playerOneWin = not isPlayerOneMove;
        }
        else
        {
            if (isPlayerOneMove)
            {
                playerOneWin = (playerOneTarget == olc::BLACK);
            }
            else
            {
                playerOneWin = not (playerTwoTarget == olc::BLACK);
            }
        }

        isGameOver = true;
    }

    void CloseTable(olc::Pixel BallCol)
    {
        isTableOpen = false;

        olc::Pixel OtherCol = olc::RED;
        if (BallCol == olc::RED) { OtherCol = olc::YELLOW; }

        if (isPlayerOneMove)
        {
            playerOneTarget = BallCol;
            playerTwoTarget = OtherCol;
        }
        else
        {
            playerOneTarget = OtherCol;
            playerTwoTarget = BallCol;
        }

    }

    void CheckPocketedBalls(C_Ball* PBalls, int SW, int SH)
    {
        bool wasPlayerOneMove = isPlayerOneMove;

        if (pocketedBalls.size() == 0)
        {
            isPlayerOneMove = not wasPlayerOneMove;
        }
        else
        {
            olc::Pixel strikerTarget = playerTwoTarget;
            if (isPlayerOneMove) { strikerTarget = playerOneTarget; }

            for (int i = 0; i < pocketedBalls.size(); i++)
            {
                int index = pocketedBalls[i];

                if (index == 0)
                {
                    // Cue Ball
                    CueBallPocketed(PBalls, SW, SH);
                    isPlayerOneMove = not wasPlayerOneMove;
                }
                else if (index == 15)
                {
                    // 8-Ball
                    EightBallPocketed();
                }
                else if (index >= 1 && index <= 7)
                {
                    // Red
                    if (strikerTarget == olc::GREY)
                    {
                        CloseTable(olc::RED);
                    }
                    else if (strikerTarget != olc::RED)
                    {
                        isPlayerOneMove = not wasPlayerOneMove;
                    }

                }
                else if (index >= 8 && index <= 14)
                {
                    // Yellow
                    if (strikerTarget == olc::GREY)
                    {
                        CloseTable(olc::YELLOW);
                    }
                    else if (strikerTarget != olc::YELLOW)
                    {
                        isPlayerOneMove = not wasPlayerOneMove;
                    }
                }

            }
        }
    }

    char CheckEndGame()
    {
        if (isGameOver)
        {
            return 'r';
        }
        return 'a';
    }

};



class Snooker_Rules : public Both_Rules
{
private:
    int phase;
public:
    void ResetTurnAttributes()
    {
        pocketedBalls.clear();
    }

    void ResetAllAttributes()
    {
        gameIsSnooker = true;
        isGameOver = false;
        playerOneWin = false;
        isPlayerOneMove = true;
        playerOneTarget = olc::RED;
        playerTwoTarget = olc::RED;
        playerOnePoints = 0;
        playerTwoPoints = 0;
        phase = 1;

        ResetTurnAttributes();
    }

    void GivePoints(bool foul, int numOfPoints)
    {
        // isPlayerOneMove XOR foul
        bool playerOneGetsPoints = (isPlayerOneMove != foul);

        if (playerOneGetsPoints) { playerOnePoints += numOfPoints; }
        else { playerTwoPoints += numOfPoints; }
    }

    void ReplaceBall(C_Ball* SBalls, int index, int SW, int SH)
    {
        float DRadius = 0.135 * SH;
        switch (index)
        {
        case 0:
            SBalls[0].SetPos(SW * 0.304f, 0.5f * (SH + DRadius));
            SBalls[0].SetIsOnTable(true);
            break;
        case 16:
            SBalls[16].SetPos(SW * 0.304f, SH * 0.5f + DRadius);
            SBalls[16].SetIsOnTable(true);
            break;
        case 17:
            SBalls[17].SetPos(SW * 0.304f, SH * 0.5f - DRadius);
            SBalls[17].SetIsOnTable(true);
            break;
        case 18:
            SBalls[18].SetPos(SW * 0.304f, SH * 0.5f);
            SBalls[18].SetIsOnTable(true);
            break;
        case 19:
            SBalls[19].SetPos(SW * 0.5f, SH * 0.5f);
            SBalls[19].SetIsOnTable(true);
            break;
        case 20:
            SBalls[20].SetPos(SW * 0.674f, SH * 0.5f);
            SBalls[20].SetIsOnTable(true);
            break;
        case 21:
            SBalls[21].SetPos(SW * 0.8f, SH * 0.5f);
            SBalls[21].SetIsOnTable(true);
            break;
        }
    }

    void PhaseOne_ChangeTarget(C_Ball* SBalls, olc::Pixel wasStrikerTarget)
    {
        olc::Pixel newTarget = olc::GREY;
        if (wasStrikerTarget == olc::GREY) { newTarget = olc::RED; }

        if (isPlayerOneMove)
        {
            playerOneTarget = newTarget;
        }
        else
        {
            playerTwoTarget = newTarget;
        }
    }

    void PhaseOne_CheckPocketedBalls(C_Ball* SBalls, int SW, int SH)
    {
        bool wasPlayerOneMove = isPlayerOneMove;
        olc::Pixel wasStrikerTarget = playerTwoTarget;
        if (isPlayerOneMove) { wasStrikerTarget = playerOneTarget; }

        if (pocketedBalls.size() == 0)
        {
            isPlayerOneMove = not wasPlayerOneMove;
        }
        else
        {
            for (int i = 0; i < pocketedBalls.size(); i++)
            {
                int index = pocketedBalls[i];

                if (index == 0)
                {
                    // Cue Ball
                    GivePoints(true, 4);
                    ReplaceBall(SBalls, index, SW, SH);
                    isPlayerOneMove = not wasPlayerOneMove;
                }
                else if (index >= 1 && index <= 15)
                {
                    // Red Ball
                    bool foul = (wasStrikerTarget != olc::RED);
                    GivePoints(foul, 1);
                    if (foul) { isPlayerOneMove = not wasPlayerOneMove; }
                    else { PhaseOne_ChangeTarget(SBalls, wasStrikerTarget); }
                }
                else
                {
                    // Yellow, Green, Brown, Blue, Pink, or Black Ball    
                    bool foul = (wasStrikerTarget != olc::GREY);
                    GivePoints(foul, index - 14);
                    if (foul) { isPlayerOneMove = not wasPlayerOneMove; }
                    else { PhaseOne_ChangeTarget(SBalls, wasStrikerTarget); }
                    ReplaceBall(SBalls, index, SW, SH);
                }

                if (wasPlayerOneMove) { playerTwoTarget = olc::RED; }
                else { playerOneTarget = olc::RED; }

            }
        }

        if (isPlayerOneMove) { playerTwoTarget = olc::RED; }
        else { playerOneTarget = olc::RED; }
    }

    int PhaseTwo_CalculateTarget(C_Ball* SBalls)
    {
        int targetInd = 21;
        for (int i = 20; i >= 16; i--)
        {
            if (SBalls[i].IsOnTable())
            {
                targetInd = i;
            }
        }

        return targetInd;
    }

    void PhaseTwo_CheckPocketedBalls(C_Ball* SBalls, int SW, int SH)
    {
        bool wasPlayerOneMove = isPlayerOneMove;

        olc::Pixel colours[6] = { olc::YELLOW, olc::GREEN, olc::Pixel(149, 85, 0), olc::Pixel(15, 75, 225), olc::Pixel(255, 105, 180), olc::BLACK };
        int wasStrikerTargetInd = PhaseTwo_CalculateTarget(SBalls);
        olc::Pixel wasStrikerTarget = colours[wasStrikerTargetInd - 16];
        playerOneTarget = wasStrikerTarget;
        playerTwoTarget = wasStrikerTarget;

        if (pocketedBalls.size() == 0)
        {
            isPlayerOneMove = not wasPlayerOneMove;
        }
        else
        {
            for (int i = 0; i < pocketedBalls.size(); i++)
            {
                int index = pocketedBalls[i];

                if (index == 0)
                {
                    // Cue Ball
                    GivePoints(true, 4);
                    ReplaceBall(SBalls, index, SW, SH);
                    isPlayerOneMove = not wasPlayerOneMove;
                }
                else
                {
                    // Yellow, Green, Brown, Blue, Pink, or Black Ball    
                    bool foul = (index > wasStrikerTargetInd);
                    if (index < 16) { GivePoints(foul, 1); }
                    else { GivePoints(foul, index - 14); }
                    if (foul)
                    {
                        isPlayerOneMove = not wasPlayerOneMove;
                        ReplaceBall(SBalls, index, SW, SH);
                    }
                }

            }
        }
    }

    void CheckPocketedBalls(C_Ball* SBalls, int SW, int SH)
    {
        if (phase == 1) { PhaseOne_CheckPocketedBalls(SBalls, SW, SH); }
        else { PhaseTwo_CheckPocketedBalls(SBalls, SW, SH); }
    }

    void CheckPhaseChange(C_Ball* SBalls)
    {
        olc::Pixel strikerTarget = playerTwoTarget;
        if (isPlayerOneMove) { strikerTarget = playerOneTarget; }

        if (strikerTarget == olc::GREY)
        {
            bool redsOnTable = false;
            for (int i = 1; i <= 15; i++) { redsOnTable = redsOnTable || SBalls[i].IsOnTable(); }

            if (not redsOnTable) { phase = 2; }
        }
    }

    char CheckEndGame(C_Ball* SBalls)
    {
        // If the Black ball is pocketed, but the game should not yet be over, it will already have been replaced onto the table
        if (not SBalls[21].IsOnTable())
        {
            isGameOver = true;
            if (playerOnePoints > playerTwoPoints) { playerOneWin = true; }

            return 'r';
        }
        return 'a';
    }

};

