// Keep the dynamic aiming pivot within the maximum power
M_Vector Constrain_Power(M_Vector dynamicPivot, M_Vector aimPivot, int constraint)
{
    float width = (dynamicPivot.x - aimPivot.x);
    float height = (dynamicPivot.y - aimPivot.y);
    float dist = std::sqrt((width * width) + (height * height));

    if (dist > constraint)
    {
        float cosTheta = width / dist;
        float sinTheta = height / dist;

        dynamicPivot.x = aimPivot.x + (cosTheta * constraint);
        dynamicPivot.y = aimPivot.y + (sinTheta * constraint);
    }

    return dynamicPivot;
}

M_Vector MovePivot(olc::PixelGameEngine* olcfunc, M_Vector* aimPivot, int SW, int SH)
{
    M_Vector dynamicPivot;
    dynamicPivot.x = olcfunc->GetMouseX();
    dynamicPivot.y = olcfunc->GetMouseY();

    if (olcfunc->GetMouse(0).bHeld)
    {
        dynamicPivot = Constrain_Power(dynamicPivot, *aimPivot, SW / 10.667);
    }
    else
    {
        aimPivot->x = olcfunc->GetMouseX();
        aimPivot->y = olcfunc->GetMouseY();
    }

    return dynamicPivot;
}


// Pointer
float GetVectorDistance_P(M_Vector* Vector1, M_Vector* Vector2)
{
    float width = Vector1->x - Vector2->x;
    float height = Vector1->y - Vector2->y;
    float distance = std::sqrt((width * width) + (height * height));

    return distance;
}

// Vector
float GetVectorDistance_V(M_Vector Vector1, M_Vector Vector2)
{
    float width = Vector1.x - Vector2.x;
    float height = Vector1.y - Vector2.y;
    float distance = std::sqrt((width * width) + (height * height));

    return distance;
}


char CheckStrike(olc::PixelGameEngine* olcfunc, C_Ball* Balls, M_Vector* aimPivot, float pivotDist, int SW)
{
    if (olcfunc->GetMouse(0).bReleased && pivotDist > SW / 128)
    {
        float directionWidth = aimPivot->x - Balls[0].GetPos().x;
        float directionHeight = aimPivot->y - Balls[0].GetPos().y;

        // Get the angle of aimPivot from the cue ball (in radians)
        float theta = std::atan2(directionHeight, directionWidth);

        float power = (0.536f * pivotDist - 2.183f) / 7;

        float horizontalPower = std::cos(theta) * power;
        float verticalPower = std::sin(theta) * power;

        Balls[0].SetVel(horizontalPower, verticalPower);

        return 'w';
    }
    return 'a';
}



void ApplyAndDecayVelocity(C_Ball* Balls, int Num_of_Balls)
{
    for (int i = 0; i < Num_of_Balls; i++)
    {
        float newPosX = Balls[i].GetPos().x + Balls[i].GetVel().x;
        float newPosY = Balls[i].GetPos().y + Balls[i].GetVel().y;
        Balls[i].SetPos(newPosX, newPosY);

        float newVelX = Balls[i].GetVel().x * 0.995f;
        float newVelY = Balls[i].GetVel().y * 0.995f;
        if (std::abs(newVelX) + std::abs(newVelY) < 0.01)
        {
            newVelX = 0;
            newVelY = 0;
        }
        Balls[i].SetVel(newVelX, newVelY);
    }
}



std::vector<int> CheckPockets(C_Pocket* Pockets, C_Ball* Balls, int Num_of_Balls)
{
    std::vector<int> pocketedBalls;

    for (int p = 0; p < 6; p++)
    {
        for (int b = 0; b < Num_of_Balls; b++)
        {
            if (Balls[b].IsOnTable())
            {
                float Pocket_to_Ball = GetVectorDistance_V(Pockets[p].GetPos(), Balls[b].GetPos());

                if (Pocket_to_Ball < Pockets->GetRad())
                {
                    pocketedBalls.push_back(b);
                    Balls[b].SetIsOnTable(false);
                    Balls[b].SetVel(0, 0);
                }
            }
        }
    }

    return pocketedBalls;
}



void CollideWalls(C_Ball* Balls, int Num_of_Balls, M_Rect cloth, int SW, int SH)
{
    for (int b = 0; b < Num_of_Balls; b++)
    {
        // Coefficient of Restitution
        float e = 0.75f;

        float posX = Balls[b].GetPos().x;
        float posY = Balls[b].GetPos().y;
        float velX = Balls[b].GetVel().x;
        float velY = Balls[b].GetVel().y;
        float rad = Balls[b].GetRad();

        bool xVelPositive = velX > 0;
        bool yVelPositive = velY > 0;
        bool xPosTooLow = posX - rad <= cloth.x;
        bool xPosTooBig = posX + rad >= cloth.x + cloth.w;
        bool yPosTooLow = posY - rad <= cloth.y;
        bool yPosTooBig = posY + rad >= cloth.y + cloth.h;

        // If x position is out of range, and x velosity is pointing away from the table
        if ((xPosTooLow && not xVelPositive) || (xPosTooBig && xVelPositive))
        {
            // Reverse X direction
            Balls[b].SetVel(-velX * e, velY);
        }

        // If y position is out of range, and y velosity is pointing away from the table
        if ((yPosTooLow && not yVelPositive) || (yPosTooBig && yVelPositive))
        {
            // Reverse Y direction
            Balls[b].SetVel(velX, -velY * e);
        }
    }
}



bool IsBallMoving(M_Vector velosity)
{
    bool X_Vel_Moving = std::abs(velosity.x) > 0.01f;
    bool Y_Vel_Moving = std::abs(velosity.y) > 0.01f;

    return (X_Vel_Moving || Y_Vel_Moving);
}



bool AreCollisionsDone(C_Ball Ball, int ballToCheck)
{
    std::vector<int> collisions = Ball.GetOverlappingBalls();

    for (int i = 0; i < collisions.size(); i++)
    {
        if (collisions[i] == ballToCheck)
        {
            return true;
        }
    }
    return false;
}


M_Vector GetMidPoint(M_Vector pos1, M_Vector pos2)
{
    float difPosX = pos2.x - pos1.x;
    float difPosY = pos2.y - pos1.y;

    M_Vector midPos;
    midPos.x = pos1.x + (difPosX / 2);
    midPos.y = pos1.y + (difPosY / 2);

    return midPos;
}


float GetTangentAngle(M_Vector midPoint, M_Vector ballCentre)
{
    float xDist = ballCentre.x - midPoint.x;
    float yDist = ballCentre.y - midPoint.y;

    float theta = std::atan2(xDist, yDist);

    return theta;
}



void CollideBalls(C_Ball* Balls, int Num_of_Balls)
{
    // Compare every combination of balls
    for (int a = 0; a < Num_of_Balls - 1; a++)
    {
        for (int b = a + 1; b < Num_of_Balls; b++)
        {
            M_Vector a_pos = Balls[a].GetPos();
            M_Vector b_pos = Balls[b].GetPos();
            M_Vector a_vel = Balls[a].GetVel();
            M_Vector b_vel = Balls[b].GetVel();

            bool A_OnTable = Balls[a].IsOnTable();
            bool B_OnTable = Balls[b].IsOnTable();
            bool Both_OnTable = A_OnTable && B_OnTable;

            float dist = GetVectorDistance_V(Balls[a].GetPos(), Balls[b].GetPos());
            bool Balls_Are_Too_Close = dist <= 2.25 * Balls[a].GetRad();

            bool Ball_1_Moving = IsBallMoving(Balls[a].GetVel());
            bool Ball_2_Moving = IsBallMoving(Balls[b].GetVel());
            bool A_Ball_Is_Moving = Ball_1_Moving || Ball_2_Moving;

            bool Already_Done_Collision = AreCollisionsDone(Balls[a], b);


            if (Both_OnTable && Balls_Are_Too_Close && A_Ball_Is_Moving && not Already_Done_Collision)
            {
                // Coefficient of Restitution
                float e = 0.98f;

                float theta = atan2(b_pos.y - a_pos.y, b_pos.x - a_pos.x);

                // Rotate the Vectors
                M_Vector RotVelA;
                M_Vector RotVelB;
                RotVelA.x = cos(theta) * a_vel.x + sin(theta) * a_vel.y;
                RotVelA.y = -sin(theta) * a_vel.x + cos(theta) * a_vel.y;
                RotVelB.x = cos(theta) * b_vel.x + sin(theta) * b_vel.y;
                RotVelB.y = -sin(theta) * b_vel.x + cos(theta) * b_vel.y;


                // Solve 1D problem
                float buffer = RotVelA.x;
                RotVelA.x = RotVelB.x * e;
                RotVelB.x = buffer * e;


                // Rotate Back
                a_vel.x = cos(theta) * RotVelA.x - sin(theta) * RotVelA.y;
                a_vel.y = sin(theta) * RotVelA.x + cos(theta) * RotVelA.y;
                b_vel.x = cos(theta) * RotVelB.x - sin(theta) * RotVelB.y;
                b_vel.y = sin(theta) * RotVelB.x + cos(theta) * RotVelB.y;


                Balls[a].SetVel(a_vel);
                Balls[b].SetVel(b_vel);


                // add b to Balls[a]'s overlap
                Balls[a].AddOverlappingBall(b);
            }

            // Remove Collisions
            if (Already_Done_Collision && not Balls_Are_Too_Close)
            {
                Balls[a].RemoveOverlappingBall(b);
            }

        }
    }
}



// We need to wait until there are no balls moving to return to aiming mode
char CheckMovingBalls(C_Ball* Balls, int Num_of_Balls)
{
    for (int i = 0; i < Num_of_Balls; i++)
    {
        bool movingHorizontally = Balls[i].GetVel().x != 0;
        bool movingVertically = Balls[i].GetVel().y != 0;

        if (movingHorizontally || movingVertically)
        {
            return 'w';     // stay in watching mode
        }
    }

    return 'a';     // switch to aiming mode
}

