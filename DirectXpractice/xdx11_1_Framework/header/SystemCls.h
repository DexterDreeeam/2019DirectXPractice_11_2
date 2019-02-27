
#pragma once

#define WIN32_LEAN_AND_MEAN //reduce the size of Win32 header files

#include <windows.h>

#include "header/InputCls.h"
#include "header/GraphicsCls.h"
#include "header/DDClsMacro.h"

class SystemCls
{
public_fun:
    SystemCls();
    SystemCls(const SystemCls &);
    ~SystemCls();

    bool fInitialize();
    void fShutdown();
    void fRun();

    LRESULT CALLBACK fMessageHandler(HWND, UINT, WPARAM, LPARAM);

public_mem:

private_fun:
    bool fFrame();
    void fInitializeWindows(int&, int&);
    void fShutdownWindows();

private_mem:
    LPCSTR mApplicationName;
    HINSTANCE mHinstance;
    HWND mHwnd;

    InputCls * mInput;
    GraphicsCls * mGraphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemCls * ApplicationHandle = 0;