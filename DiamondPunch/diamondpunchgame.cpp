#include "diamondpunchgame.h"
#include <cstdio>
#include "resource.h"
#include <iostream>
#include <cmath>
#include "Vec2.h"

using namespace std;

DiamondPunchGame::DiamondPunchGame(HINSTANCE hAppInst, HWND hMainWnd,Vec2 wndCenterPt)
{
    mhAppInst = hAppInst;
    mhMainWnd = hMainWnd;
    mWndCenterPt = wndCenterPt;

    mPlayer1Score = 0;
    mPlayer2Score = 0;

    diamonds1.resize(12);
    diamonds2.resize(12);

    mPaused = false;
    mGameOver = false;

    mRecoveryTime = 2.0f;

    attached1 = 0;
    attached2 = 0;

    Circle bc;
    Vec2 p0 = wndCenterPt;
    Vec2 v0(0.0f, 0.0f);
    mGameBoard = new Sprite(mhAppInst, IDB_GAMEBOARD,
                            IDB_GAMEBOARDMASK, bc, p0, v0);

    p0.x = 50;
    p0.y = 50;

    bc.c = p0;
    bc.r = 25.0f;
    mPlayer1 = new Sprite(mhAppInst, IDB_PLAYER,
                            IDB_PLAYERMASK, bc, p0, v0);

    p0.x = 795;
    p0.y = 50;

    bc.c = p0;
    bc.r = 25.0f;
    mPlayer2 = new Sprite(mhAppInst, IDB_PLAYER,
                             IDB_PLAYERMASK, bc, p0, v0);

    p0 = wndCenterPt;

    v0.y = 200.0f;

    bc.c = p0;
    bc.r = 40.0f;
    mBasket = new Sprite(mhAppInst, IDB_BASKET,
                         IDB_BASKETMASK, bc, p0, v0);

    for(int i = 0; i < diamonds1.size(); i++)
    {
        Vec2 v0(0.0f, 0.0f);
        p0.x = 1 + rand() % 96;
        p0.y =  1 + rand() % 478;
        bc.c = p0;
        bc.r = 20.0f;
        diamonds1[i] = new Sprite(mhAppInst, IDB_DIAMOND,
                              IDB_DIAMONDMASK, bc, p0, v0);


    }
    for(int i = 0; i < diamonds2.size(); i++)
    {
        Vec2 v0(0.0f, 0.0f);
        p0.x = 790;
        p0.y = 1 + rand() % 478;
        bc.c = p0;
        bc.r = 20.0f;
        diamonds2[i] = new Sprite(mhAppInst, IDB_DIAMOND,
                              IDB_DIAMONDMASK, bc, p0, v0);

    }



    mPlayer1Bound  = Rect(18, 21, 96, 478);
    mPlayer2Bound  = Rect(786, 21, 874, 478);
    mBoardBound    = Rect(0, 0, 894, 500);
}

DiamondPunchGame::~DiamondPunchGame()
{
    delete mGameBoard;
    delete mPlayer1;
    delete mPlayer2;
    delete mBasket;

    for(int i = 0; i<diamonds1.size(); i++)
    {
        delete diamonds1[i];

    }
    for(int i = 0; i<diamonds2.size(); i++)
    {
        delete diamonds2[i];
    }


}

void DiamondPunchGame::pause()
{
    mPaused = true;

    ShowCursor(true);
}

void DiamondPunchGame::unpause()
{
    mPaused = false;

    ShowCursor(false);
}

void DiamondPunchGame::update(float dt)
{
    if(!mPaused)
    {
        updatePlayer1(dt);
        updatePlayer2(dt);
        updateDiamond(dt);
        updateBasket(dt);

        if(mRecoveryTime > 0.0f)
            mRecoveryTime -= dt;
    }
}

void DiamondPunchGame::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
    mGameBoard->draw(hBackBufferDC, hSpriteDC);
    mPlayer1->draw(hBackBufferDC, hSpriteDC);
    mPlayer2->draw(hBackBufferDC, hSpriteDC);

    for(int i = 0; i < diamonds1.size(); i++)
    {
        diamonds1[i]->draw(hBackBufferDC, hSpriteDC);

    }

    for(int i = 0; i < diamonds2.size(); i++)
    {
        diamonds2[i]->draw(hBackBufferDC, hSpriteDC);

    }

    mBasket->draw(hBackBufferDC, hSpriteDC);

    char score[32];
    sprintf(score, "Player1's score = %d", mPlayer1Score);

    SetBkMode(hBackBufferDC, TRANSPARENT);
    TextOut(hBackBufferDC, 15, 45, score, (int)strlen(score));

    sprintf(score, "Player2's Score = %d", mPlayer2Score );
    TextOut(hBackBufferDC, 600, 45, score, (int)strlen(score));

    if(mGameOver)
    {
        char score[32];
        SetTextColor(hBackBufferDC,RGB(255, 0, 0));
        HFONT hFont=CreateFont(40,0,0,0,FW_BOLD,0,0,0,0,0,0,2,0,"SYSTEM_FIXED_FONT");
        HFONT hTmp = (HFONT)SelectObject(hBackBufferDC,hFont);
        sprintf(score, "%s wins!", mWinner);
        SetBkMode(hBackBufferDC, TRANSPARENT);
        TextOut(hBackBufferDC, mWndCenterPt.x - 100.0f, mWndCenterPt.y, score, (int)strlen(score));
        DeleteObject(SelectObject(hBackBufferDC,hTmp));
    }

}

void DiamondPunchGame::diamondAttachedPlayer1()
{
    int i = 0;
    float minimumDistance = abs(diamonds1[i]->mPosition.x - mPlayer1->mPosition.x);
    attached1 = diamonds1[i];

    for(i = 1; i < diamonds1.size(); i++)
    {
        if(abs(diamonds1[i]->mPosition.x - mPlayer1->mPosition.x) < minimumDistance)
        {
            minimumDistance = abs(diamonds1[i]->mPosition.x - mPlayer1->mPosition.x);
            attached1 = diamonds1[i];
        }

    }

    attached1->mPosition = mPlayer1->mPosition;
    attached1->mBoundingCircle.c = mPlayer1->mBoundingCircle.c;

}

void DiamondPunchGame::diamondAttachedPlayer2()
{
    int i = 0;
    float minimumDistance = abs(diamonds2[i]->mPosition.x - mPlayer2->mPosition.x);
    attached2 = diamonds2[i];

    for(i = 1; i < diamonds2.size(); i++)
    {
        if(abs(diamonds2[i]->mPosition.x - mPlayer2->mPosition.x) < minimumDistance)
        {
            minimumDistance = abs(diamonds2[i]->mPosition.x - mPlayer2->mPosition.x);
            attached2 = diamonds2[i];
        }

    }

    attached2->mPosition = mPlayer2->mPosition;
    attached2->mBoundingCircle.c = mPlayer2->mBoundingCircle.c;

}

void DiamondPunchGame::moveDiamond1(int x)
{
    if(x==1)
    {
        attached1->mPosition.y += 5;
    }
    if(x==2)
    {
        attached1->mPosition.y -= 5;
    }
    if(x==3)
    {
        attached1->mPosition.x += 5;
    }
    if(x==4)
    {
        attached1->mPosition.x -= 5;
    }

}
void DiamondPunchGame::moveDiamond2(int x)
{
    if(x==1)
    {
        attached2->mPosition.y += 5;
    }
    if(x==2)
    {
        attached2->mPosition.y -= 5;
    }
    if(x==3)
    {
        attached2->mPosition.x += 5;
    }
    if(x==4)
    {
        attached2->mPosition.x -= 5;
    }

}

void DiamondPunchGame::throwDiamond1()
{
        attached1->mVelocity.x = 300.0f;
}

void DiamondPunchGame::throwDiamond2()
{
        attached2->mVelocity.x = -300.0f;

}

void DiamondPunchGame::movePlayer1(int x)
{
    if(x==1)
    {
        mPlayer1->mPosition.y += 5.0f;
    }
    else if(x==2)
    {
        mPlayer1->mPosition.y -= 5.0f;
    }
    else if(x==3)
    {
        mPlayer1->mPosition.x += 5.0f;
    }
    else if(x==4)
    {
        mPlayer1->mPosition.x -= 5.0f;
    }

}

void DiamondPunchGame::movePlayer2(int x)
{
    if(x==1)
    {
        mPlayer2->mPosition.y += 5;
    }
    else if(x==2)
    {
        mPlayer2->mPosition.y -= 5;
    }
    else if(x==3)
    {
        mPlayer2->mPosition.x += 5;
    }
    else if(x==4)
    {
        mPlayer2->mPosition.x -= 5;
    }

}

void DiamondPunchGame::updatePlayer1(float dt)
{
   mPlayer1->update(dt);

   mPlayer1Bound.forceInside(mPlayer1->mBoundingCircle);
   mPlayer1->mPosition = mPlayer1->mBoundingCircle.c;

}

void DiamondPunchGame::updatePlayer2(float dt)
{
    mPlayer2->update(dt);

    mPlayer2Bound.forceInside(mPlayer2->mBoundingCircle);
    mPlayer2->mPosition = mPlayer2->mBoundingCircle.c;
}

void DiamondPunchGame::updateDiamond(float dt)
{
    /*
    Vec2 v1(0.0f, 0.0f);
    Vec2 p1(0.0f, 0.0f);

    if(mRecoveryTime <=0.0f)
    {
        for(int i = 0; i < 3; i++)
        {
            p1.x = 1 + rand() % 96;
            p1.y = 1 + rand() % 478;

            Circle bc;
            bc.c = p1;
            bc.r = 20.0f;
            diamonds1[i] = new Sprite(mhAppInst, IDB_DIAMOND,
                              IDB_DIAMONDMASK, bc, p1, v1);



        }
        for(int i = 0; i < 3; i++)
        {
            p1.x = 850 + i;
            p1.y = 1 + rand() % 478;

            Circle bc;
            bc.c = p1;
            bc.r = 20.0f;
            diamonds2[i] = new Sprite(mhAppInst, IDB_DIAMOND,
                              IDB_DIAMONDMASK, bc, p1, v1);
        }
        mRecoveryTime = 2.0f;
    }
    */

    for(int i = 0; i < diamonds1.size(); i++)
    {
        POINT p;
        diamonds1[i]->update(dt);

        p.x = diamonds1[i]->mPosition.x;
        p.y = diamonds1[i]->mPosition.y;

        if(!PtInRect(&gMapRect, p))
            diamonds1.erase(diamonds1.begin() + i);
    }

    for(int i = 0; i < diamonds2.size(); i++)
    {
        POINT p;

        diamonds2[i]->update(dt);

        p.x = diamonds2[i]->mPosition.x;
        p.y = diamonds2[i]->mPosition.y;

        if(!PtInRect(&gMapRect, p))
            diamonds2.erase(diamonds2.begin() + i);


    }
}

void DiamondPunchGame::updateBasket(float dt)
{
    basketDiamondCollision();

    Circle basketCircle = mBasket->mBoundingCircle;
    if(basketCircle.c.y - basketCircle.r < mBoardBound.minPt.y)
        mBasket->mVelocity.y *= -1.0f;
    if(basketCircle.c.y + basketCircle.r > mBoardBound.maxPt.y)
        mBasket->mVelocity.y *= -1.0f;

    mBasket->update(dt);

}

void DiamondPunchGame::basketDiamondCollision()
{
    Circle basketCircle = mBasket->mBoundingCircle;



    for(int i = 0; i < diamonds1.size(); i++)
    {
        Circle diamond = diamonds1[i]->mBoundingCircle;
        if((diamond.c.x + diamond.r > basketCircle.c.x - basketCircle.r) && abs(diamond.c.y - basketCircle.c.y) < 1)
        {
            increaseScore(true);
            diamonds1.erase(diamonds1.begin() + i);

        }

    }

    for(int i = 0; i < diamonds2.size(); i++)
    {
        Circle diamond = diamonds2[i]->mBoundingCircle;
        if((diamond.c.x - diamond.r < basketCircle.c.x + basketCircle.r) && abs(diamond.c.y - basketCircle.c.y) < 1)
        {
           increaseScore(false);
           diamonds2.erase(diamonds2.begin() + i);
        }


    }


}

void DiamondPunchGame::increaseScore(bool player1)
{
    if(player1)
        mPlayer1Score++;
    else
        mPlayer2Score++;

    if(mPlayer1Score == 5)
    {
        mGameOver = true;
        mPaused = true;
        strcpy(mWinner, "Player 1");
    }

    else if(mPlayer2Score == 5)
    {
        mGameOver = true;
        mPaused = true;
        strcpy(mWinner, "Player 2");
    }

}

