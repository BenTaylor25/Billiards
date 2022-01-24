// Maths Vector
class M_Vector
{
public:
    float x = 0;
    float y = 0;
};


// Maths Rectangle
class M_Rect
{
public:
    float x;
    float y;
    float w;
    float h;
    olc::Pixel col;
};


M_Rect MakeRectangle(float newX, float newY, float newW, float newH, olc::Pixel newCol)
{
    M_Rect newRect;
    newRect.x = newX;
    newRect.y = newY;
    newRect.w = newW;
    newRect.h = newH;
    newRect.col = newCol;

    return newRect;
}


// Set the size of the playing area
M_Rect SizeCloth(M_Rect cloth, int SW, int SH)
{
    cloth = MakeRectangle(SW * 0.109f, SH * 0.153f, SW * 0.781f, SH * 0.694f, olc::WHITE);

    return cloth;
}


// Return the input vector without the item that should be excluded
std::vector<int> RemoveItem(std::vector<int> theVector, int item_to_remove)
{
    for (int i = 0; i < theVector.size(); i++)
    {
        if (theVector[i] == item_to_remove)
        {
            theVector.erase(theVector.begin() + i);
        }
    }

    return theVector;
}


// Interface
class PositionAndRadius
{
private:
    M_Vector pos;
    int rad;
public:
    M_Vector GetPos() { return pos; }
    void SetPos(M_Vector newPos) { pos = newPos; }
    void SetPos(float newX, float newY)
    {
        pos.x = newX;
        pos.y = newY;
    }

    int GetRad() { return rad; }
    void SetRad(int newRad) { rad = newRad; }
};


// Class Pocket
class C_Pocket : public PositionAndRadius {};


// Class Ball
class C_Ball : public PositionAndRadius
{
private:
    M_Vector vel;
    bool onTable = true;
    olc::Pixel col = olc::WHITE;
    std::vector<int> overlappingBalls;
public:
    M_Vector GetVel() { return vel; }
    void SetVel(M_Vector newVel) { vel = newVel; }
    void SetVel(float newX, float newY)
    {
        vel.x = newX;
        vel.y = newY;
    }

    bool IsOnTable() { return onTable; }
    void SetIsOnTable(bool newOnTable) { onTable = newOnTable; }

    olc::Pixel GetCol() { return col; }
    void SetCol(olc::Pixel newCol) { col = newCol; }

    std::vector<int> GetOverlappingBalls() { return overlappingBalls; }
    void AddOverlappingBall(int newOverlap) { overlappingBalls.push_back(newOverlap); }
    void RemoveOverlappingBall(int removeOverlap) { overlappingBalls = RemoveItem(overlappingBalls, removeOverlap); }
};


// Menu Text - String Tokeniser
std::vector<std::string> TextSplit(std::string text)
{
    std::vector<std::string> splitWords;

    std::string word = "";
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '¬')
        {
            if (text[i + 1] == '¬')
            {
                i++;
                word += "\n";
            }
            word += "\n\n";
        }
        else if (text[i] == ' ')
        {
            splitWords.push_back(word);
            word = "";
        }
        else
        {
            word += text[i];
        }
    }
    splitWords.push_back(word);

    return splitWords;
}

// Menu Text - Construct Output String
std::string LineBreak(std::string text, int lineLen)
{
    std::vector<std::string> splitWords = TextSplit(text);

    int lineCount = 0;
    std::string newText = "";

    for (std::string word : splitWords)
    {
        bool shouldBreak = (lineCount + word.length() + 1 > lineLen);

        if (word.find('\n') != std::string::npos)
        {
            newText += word;
            lineCount = 0;
        }
        else if (shouldBreak)
        {
            newText += word + "\n\n";
            lineCount = 0;
        }
        else
        {
            newText += word + " ";
            lineCount += word.length() + 1;
        }
    }

    return newText;
}

// Menu Text - Help Menu
std::vector<std::string> getHelpText()
{
    std::vector<std::string> helpText;

    std::ifstream myFile("HelpText.txt");
    std::string line;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            helpText.push_back(line);
        }
        myFile.close();
    }

    for (int i = 0; i < helpText.size(); i++)
    {
        helpText[i] = LineBreak(helpText[i], 36);
    }

    return helpText;
}

// Menu Text - Credits Menu
std::vector<std::string> getCreditsText()
{
    std::vector<std::string> creditsText;

    std::ifstream myFile("CreditsText.txt");
    std::string line;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            creditsText.push_back(line);
        }
        myFile.close();
    }

    for (int i = 0; i < creditsText.size(); i++)
    {
        creditsText[i] = LineBreak(creditsText[i], 36);
    }

    return creditsText;
}


// Pocket Sizes and Positions
void PocketSetup(C_Pocket* Pockets, int SW, int SH)
{
    for (int i = 0; i < 6; i++)
    {
        Pockets[i].SetRad(SW * 0.019f);
    }

    Pockets[0].SetPos(SW * 0.109f, SH * 0.153f);
    Pockets[1].SetPos(SW * 0.109f, SH * 0.847f);
    Pockets[2].SetPos(SW * 0.5f, SH * 0.153f);
    Pockets[3].SetPos(SW * 0.5f, SH * 0.847f);
    Pockets[4].SetPos(SW * 0.888f, SH * 0.153f);
    Pockets[5].SetPos(SW * 0.888f, SH * 0.847f);
}


// Ball Positions
M_Vector triangleAdj(C_Ball refBall, int YDir)
{
    M_Vector newPos;

    newPos.x = refBall.GetPos().x + (refBall.GetRad() * 1.732f);
    newPos.y = refBall.GetPos().y + (refBall.GetRad() * YDir);

    return newPos;
}

// Pool Ball Colours and Positions
void Pool_BallSetup(C_Ball* PBalls, int SW, int SH)
{
    for (int i = 0; i < 16; i++)
    {
        PBalls[i].SetIsOnTable(true);
        PBalls[i].SetRad(SW * 0.01);

        if (i > 0 && i < 8) { PBalls[i].SetCol(olc::RED); }
        if (i > 7 && i < 15) { PBalls[i].SetCol(olc::YELLOW); }
    }

    PBalls[15].SetCol(olc::BLACK);

    // Cue Ball position
    PBalls[0].SetPos(SW * 0.304f, SH * 0.5f);

    // Leftmost Red Ball position
    PBalls[1].SetPos(SW * 0.696f, SH * 0.5f);

    // The rest of the Balls' positions
    PBalls[2].SetPos(triangleAdj(PBalls[1], -1));
    PBalls[8].SetPos(triangleAdj(PBalls[1], 1));
    PBalls[9].SetPos(triangleAdj(PBalls[2], -1));
    PBalls[15].SetPos(triangleAdj(PBalls[2], 1));
    PBalls[3].SetPos(triangleAdj(PBalls[8], 1));
    PBalls[4].SetPos(triangleAdj(PBalls[9], -1));
    PBalls[10].SetPos(triangleAdj(PBalls[9], 1));
    PBalls[5].SetPos(triangleAdj(PBalls[15], 1));
    PBalls[11].SetPos(triangleAdj(PBalls[3], 1));
    PBalls[12].SetPos(triangleAdj(PBalls[4], -1));
    PBalls[13].SetPos(triangleAdj(PBalls[4], 1));
    PBalls[6].SetPos(triangleAdj(PBalls[10], 1));
    PBalls[14].SetPos(triangleAdj(PBalls[5], 1));
    PBalls[7].SetPos(triangleAdj(PBalls[11], 1));
}


// Snooker Ball Colours and Positions
void Snooker_BallSetup(C_Ball* SBalls, int SW, int SH)
{
    // Set Radii and Reds
    for (int i = 0; i < 22; i++)
    {
        SBalls[i].SetIsOnTable(true);
        SBalls[i].SetRad(SW * 0.01);
        if (i > 0 && i < 16) { SBalls[i].SetCol(olc::RED); }
    }

    // Other Colours
    SBalls[16].SetCol(olc::YELLOW);
    SBalls[17].SetCol(olc::GREEN);
    SBalls[18].SetCol(olc::Pixel(149, 85, 0));      // Brown
    SBalls[19].SetCol(olc::Pixel(15, 75, 225));     // Blue
    SBalls[20].SetCol(olc::Pixel(255, 105, 180));   // Pink
    SBalls[21].SetCol(olc::BLACK);


    // Positions
    float DRadius = 0.135 * SH;
    SBalls[17].SetPos(SW * 0.304f, SH * 0.5f - DRadius);   // Green
    SBalls[18].SetPos(SW * 0.304f, SH * 0.5f);             // Brown
    SBalls[0].SetPos(SW * 0.304f, 0.5f * (SH + DRadius));  // Cue Ball
    SBalls[16].SetPos(SW * 0.304f, SH * 0.5f + DRadius);   // Yellow

    SBalls[19].SetPos(SW * 0.5f, SH * 0.5f);     // Blue
    SBalls[20].SetPos(SW * 0.674f, SH * 0.5f);   // Pink

    // Leftmost Red Ball
    SBalls[1].SetPos(SW * 0.696f, SH * 0.5f);

    // Other Red Balls
    SBalls[2].SetPos(triangleAdj(SBalls[1], -1));
    SBalls[3].SetPos(triangleAdj(SBalls[1], 1));
    SBalls[4].SetPos(triangleAdj(SBalls[2], -1));
    SBalls[5].SetPos(triangleAdj(SBalls[2], 1));
    SBalls[6].SetPos(triangleAdj(SBalls[3], 1));
    SBalls[7].SetPos(triangleAdj(SBalls[4], -1));
    SBalls[8].SetPos(triangleAdj(SBalls[4], 1));
    SBalls[9].SetPos(triangleAdj(SBalls[5], 1));
    SBalls[10].SetPos(triangleAdj(SBalls[6], 1));
    SBalls[11].SetPos(triangleAdj(SBalls[7], -1));
    SBalls[12].SetPos(triangleAdj(SBalls[7], 1));
    SBalls[13].SetPos(triangleAdj(SBalls[8], 1));
    SBalls[14].SetPos(triangleAdj(SBalls[9], 1));
    SBalls[15].SetPos(triangleAdj(SBalls[10], 1));

    SBalls[21].SetPos(SW * 0.8f, SH * 0.5f);     // Black
}
