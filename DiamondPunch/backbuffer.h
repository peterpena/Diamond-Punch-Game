#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <windows.h>


class BackBuffer
{
    public:
        BackBuffer(HWND hWnd, int width, int height);
        ~BackBuffer();

        HDC getDC();

        int width();
        int height();

        void present();

    private:
        BackBuffer(const BackBuffer& rhs);
        BackBuffer& operator=(const BackBuffer& rhs);

    private:
        HWND mhWnd;
        HDC mhDC;
        HBITMAP mhSurface;
        HBITMAP mhOldObject;
        int mWidth;
        int mHeight;
};

#endif // BACKBUFFER_H
