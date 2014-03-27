#ifndef DIAMONDPUNCHGAME_H
#define DIAMONDPUNCHGAME_H

#include <windows.h>
#include <vector>
#include "Sprite.h"
#include "Rect.h"
#include "Circle.h"
#include <cstdio>
#include <string>

using namespace std;

class DiamondPunchGame
{
    public:
        DiamondPunchGame(HINSTANCE hAppInst, HWND hMainWnd,
                         Vec2 wndCenterPt);
        ~DiamondPunchGame();

        void pause();
        void unpause();

        void update(float dt);
        void draw(HDC hBackBufferDC, HDC hSpriteDC);
        void diamondAttachedPlayer1();
        void diamondAttachedPlayer2();
        void moveDiamond1(int x);
        void throwDiamond1();
        void moveDiamond2(int x);
        void throwDiamond2();
        void movePlayer1(int x);
        void movePlayer2(int x);

    public:
        bool mGameOver;
        char mWinner[50];


    private:
        void updatePlayer1(float dt);
        void updatePlayer2(float dt);
        void updateDiamond(float dt);
        void updateBasket(float dt);
        void basketDiamondCollision();
        void increaseScore(bool player1);

    private:
        HINSTANCE mhAppInst;
        HWND      mhMainWnd;
        Vec2      mWndCenterPt;

        int mPlayer1Score;
        int mPlayer2Score;

        bool mPaused;

        float mRecoveryTime;

        Sprite* mGameBoard;
        Sprite* mPlayer1;
        Sprite* mPlayer2;
        Sprite* mBasket;
        Sprite* mDiamond;
        Sprite* mRuby;
        Sprite* mEmerald;
        Sprite* attached1;
        Sprite* attached2;

        Rect mPlayer1Bound;
        Rect mPlayer2Bound;
        Rect mBoardBound;

        RECT gMapRect = {0,0,900,600};

        vector<Sprite*> diamonds1;
        vector<Sprite*> diamonds2;


};

#endif // DIAMONDPUNCHGAME_H
