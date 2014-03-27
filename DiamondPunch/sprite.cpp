#include "sprite.h"
#include <cassert>

Sprite::Sprite(HINSTANCE hAppInst, int imageID, int maskID,
               const Circle& bc, const Vec2& p0, const Vec2& v0)
{
    mhAppInst = hAppInst;

    mhImage = LoadBitmap(hAppInst, MAKEINTRESOURCE(imageID));
    mhMask  = LoadBitmap(hAppInst, MAKEINTRESOURCE(maskID));

    GetObject(mhImage, sizeof(BITMAP), &mImageBM);
    GetObject(mhMask, sizeof(BITMAP), &mMaskBM);

    assert(mImageBM.bmWidth == mMaskBM.bmWidth);
    assert(mImageBM.bmHeight == mMaskBM.bmHeight);

    mBoundingCircle = bc;
    mPosition = p0;
    mVelocity = v0;
}

Sprite::~Sprite()
{
    DeleteObject(mhImage);
    DeleteObject(mhMask);
}

int Sprite::width()
{
    return mImageBM.bmWidth;
}
int Sprite::height()
{
    return mImageBM.bmHeight;
}

void Sprite::update(float dt)
{
    mPosition += mVelocity * dt;

    mBoundingCircle.c = mPosition;
}

void Sprite::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
    int w = width();
    int h = height();

    int x = (int)mPosition.x - (w/2);
    int y = (int)mPosition.y - (h/2);

    HGDIOBJ oldObj = SelectObject(hSpriteDC, mhMask);

    BitBlt(hBackBufferDC, x, y, w, h, hSpriteDC, 0, 0, SRCAND);

    SelectObject(hSpriteDC, mhImage);

    BitBlt(hBackBufferDC, x, y, w, h, hSpriteDC, 0, 0, SRCPAINT);

    SelectObject(hSpriteDC, oldObj);
}
