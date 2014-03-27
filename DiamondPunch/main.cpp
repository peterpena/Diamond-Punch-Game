#include <string>
#include "resource.h"
#include "DiamondPunchGame.h"
#include "BackBuffer.h"
#include "Sprite.h"

using namespace std;

HWND ghMainWnd  = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMainMenu = 0;
HDC ghSpriteDC  =  0;

bool diamondAttached1 = false;
bool diamondAttached2 = false;
const int MOVEDOWN  = 1,
          MOVEUP    = 2,
          MOVERIGHT = 3,
          MOVELEFT  = 4;

BackBuffer* gBackBuffer = 0;
DiamondPunchGame* gDiamondPunch = 0;

string gWndCaption = "Diamond Punch";

const int gClientWidth = 894;
const int gClientHeight = 500;

const POINT gClientCenter =
{
    gClientWidth / 2,
    gClientHeight / 2
};

const int gWindowWidth = gClientWidth + 6;
const int gWindowHeight = gClientHeight + 52;

bool InitMainWindow();
int Run();
void DrawFramesPerSecond(float deltaTime);

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK
AboutBoxProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstane,
        PSTR cmdLine, int showCmd)
{
    ghAppInst = hInstance;

    if(!InitMainWindow())
    {
        MessageBox(0, "Window Creation Failed", "Error", MB_OK);
        return 0;
    }

    return Run();
}

bool InitMainWindow()
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = ghAppInst;
    wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "MyWndClassName";

    RegisterClass(&wc);

    ghMainMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU));
    ghMainWnd = ::CreateWindow("MyWndClassName",
            gWndCaption.c_str(), WS_OVERLAPPED | WS_SYSMENU,
            200, 200, gWindowWidth, gWindowHeight, 0,
            ghMainMenu, ghAppInst, 0);

            if(ghMainWnd == 0)
            {
                ::MessageBox(0, "CreateWindow-Failed", 0, 0);
                return 0;
            }
            ShowWindow(ghMainWnd, SW_NORMAL);
            UpdateWindow(ghMainWnd);
            return true;
}

int Run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    __int64 cntsPerSec = 0;
    bool perfExists = QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
    if(!perfExists)
    {
        MessageBox(0, "Performance timer does not exist!", 0, 0);
        return 0;

    }

    double timeScale = 1.0 / (double)cntsPerSec;
    __int64 lastTime = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

    double timeElapsed = 0.0;

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            __int64 currTime = 0;
            QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
            double deltaTime = (double)(currTime - lastTime) * timeScale;

            timeElapsed += deltaTime;

            if(timeElapsed >= 0.01)
            {
                gDiamondPunch->update((float)timeElapsed);
                gDiamondPunch->draw(gBackBuffer->getDC(), ghSpriteDC);

                DrawFramesPerSecond(timeElapsed);

                gBackBuffer->present();

                if(gDiamondPunch->mGameOver)
                {
                    Sleep(5000);
                    PostQuitMessage(0);
                }


                timeElapsed = 0.0f;
            }

            lastTime = currTime;
        }

    }
    return (int)msg.wParam;
}

void DrawFramesPerSecond(float deltaTime)
{
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;
    static char buffer[256];

    ++frameCnt;

    timeElapsed += deltaTime;

    if(timeElapsed >= 1.0f)
    {
        sprintf(buffer, "--Frames Per Second = %d", frameCnt);

        string newCaption = gWndCaption + buffer;

        SetWindowText(ghMainWnd, newCaption.c_str());

        frameCnt = 0;
        timeElapsed = 0.0f;
    }
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
        gDiamondPunch = new DiamondPunchGame(ghAppInst, hWnd, gClientCenter);
        ghSpriteDC = CreateCompatibleDC(0);

        gBackBuffer = new BackBuffer(hWnd,
                                     gClientWidth,
                                     gClientHeight);

        return 0;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case ID_FILE_EXIT:
                DestroyWindow(ghMainWnd);
                break;

            case ID_HELP_ABOUT:
                DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ABOUTBOX),
                          ghMainWnd, AboutBoxProc);

                break;
        }
        return 0;

            case WM_LBUTTONDOWN:
                gDiamondPunch->unpause();
                return 0;

            case WM_RBUTTONDOWN:
                gDiamondPunch->pause();
                return 0;
            case WM_KEYDOWN:
                switch(wParam)
                {
                case 'W':
                gDiamondPunch->movePlayer1(MOVEUP);

                if(diamondAttached1)
                {
                    gDiamondPunch->moveDiamond1(MOVEUP);
                }
                break;

            case 'S':
                gDiamondPunch->movePlayer1(MOVEDOWN);

                if(diamondAttached1)
                {
                    gDiamondPunch->moveDiamond1(MOVEDOWN);
                }
               break;

            case 'D':
                gDiamondPunch->movePlayer1(MOVERIGHT);

                if(diamondAttached1)
                {
                    gDiamondPunch->moveDiamond1(MOVERIGHT);
                }
                break;

            case 'A':
                gDiamondPunch->movePlayer1(MOVELEFT);

                if(diamondAttached1)
                {
                    gDiamondPunch->moveDiamond1(MOVELEFT);
                }
                break;

            case 'E':
                if(diamondAttached1)
                {
                    gDiamondPunch->throwDiamond1();
                }

                diamondAttached1 = false;

                break;

            case 'Q':
                gDiamondPunch->diamondAttachedPlayer1();

                diamondAttached1 = true;

                break;

            case VK_UP:
                gDiamondPunch->movePlayer2(MOVEUP);

                if(diamondAttached2)
                {
                    gDiamondPunch->moveDiamond2(MOVEUP);
                }
                break;

            case VK_DOWN:
                gDiamondPunch->movePlayer2(MOVEDOWN);
                if(diamondAttached2)
                {
                    gDiamondPunch->moveDiamond2(MOVEDOWN);
                }
                 break;

            case VK_RIGHT:
                gDiamondPunch->movePlayer2(MOVERIGHT);
                if(diamondAttached2)
                {
                    gDiamondPunch->moveDiamond2(MOVERIGHT);
                }
                break;

            case VK_LEFT:
                gDiamondPunch->movePlayer2(MOVELEFT);
                if(diamondAttached2)
                {
                    gDiamondPunch->moveDiamond2(MOVELEFT);
                }
                break;

            case VK_RETURN:
                if(diamondAttached2)
                {
                    gDiamondPunch->throwDiamond2();
                }
                diamondAttached2 = false;

                break;

            case VK_SHIFT:
                gDiamondPunch->diamondAttachedPlayer2();
                diamondAttached2 = true;

                break;

                }

                return 0;

            case WM_DESTROY:
                delete gDiamondPunch;
                delete gBackBuffer;
                DeleteDC(ghSpriteDC);
                PostQuitMessage(0);
                return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK
AboutBoxProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return true;

    case WM_COMMAND:
        switch(LOWORD(wParam));
        {
        case IDOK:
            EndDialog(hDlg, 0);
            break;
        }
        return true;
    }
    return false;
}

