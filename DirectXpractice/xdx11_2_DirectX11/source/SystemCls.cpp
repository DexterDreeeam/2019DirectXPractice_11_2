
#include "header/SystemCls.h"

SystemCls::SystemCls()
{
    mInput = 0;
    mGraphics = 0;
}

SystemCls::SystemCls(const SystemCls & other)
{

}

SystemCls::~SystemCls()
{

}

bool SystemCls::fInitialize()
{
    int screenWidth, screenHeight;
    bool rst;

    screenWidth = 0;
    screenHeight = 0;
    fInitializeWindows(screenWidth, screenHeight);

    mInput = new InputCls;
    if(!mInput) return false;
    mInput->fInitialize();
    
    mGraphics = new GraphicsCls;
    if(!mGraphics) return false;
    rst = mGraphics->fInitialize(screenWidth, screenHeight, mHwnd);
    if(!rst) return false;

    return true;
}

void SystemCls::fShutdown()
{
    if(mGraphics)
    {
        mGraphics->fShutdown();
        delete mGraphics;
        mGraphics = 0;
    }

    if(mInput)
    {
        delete mInput;
        mInput = 0;
    }

    fShutdownWindows();

    return;
}

void SystemCls::fRun()
{
    MSG msg;
    bool done, rst;

    ZeroMemory(&msg, sizeof(MSG));

    done = false;
    while(!done)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if(msg.message == WM_QUIT)
        {
            done = true;
        }
        else
        {
            rst = fFrame();
            if(!rst)
            {
                done = true;
            }
        }
    }
    return;
}

bool SystemCls::fFrame()
{
    bool rst;
    if(mInput->fIsKeyDown(VK_ESCAPE))
    {
        return false;
    }
    rst = mGraphics->fFrame();
    if(!rst) return false;
    
    return true;
}

LRESULT CALLBACK SystemCls::fMessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
    {
    case WM_KEYDOWN:
        mInput->fKeyDown((unsigned int)wparam);
        return 0;
    case WM_KEYUP:
        mInput->fKeyUp((unsigned int)wparam);
        return 0;
    default:
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
}

void SystemCls::fInitializeWindows(int & screenWidth, int & screenHeight)
{
    ApplicationHandle = this;
    mHinstance = GetModuleHandle(NULL);
    mApplicationName = "Engine";
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = mHinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = mApplicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);

    int posX, posY;
    if(FULL_SCREEN)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        posX = posY = 0;
    }
    else
    {
        screenWidth = 800;
        screenHeight = 600;
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    mHwnd = CreateWindowEx(
        WS_EX_APPWINDOW, 
        mApplicationName, 
        mApplicationName, 
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY,
        screenWidth, screenHeight,
        NULL,
        NULL,
        mHinstance,
        NULL
        );

    ShowWindow(mHwnd, SW_SHOW);
    SetForegroundWindow(mHwnd);
    SetFocus(mHwnd);
    ShowCursor(false);

    return;
}

void SystemCls::fShutdownWindows()
{
    ShowCursor(true);

    if(FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    DestroyWindow(mHwnd);
    mHwnd = NULL;

    UnregisterClass(mApplicationName, mHinstance);

    ApplicationHandle = 0;

    return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch(umessage)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        return ApplicationHandle->fMessageHandler(hwnd, umessage, wparam, lparam);
    }
}