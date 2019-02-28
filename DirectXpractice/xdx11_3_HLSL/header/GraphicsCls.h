
#pragma once

#include "header/DDClsMacro.h"
#include "header/D3dCls.h"
#include "header/CameraCls.h"
#include "header/ModelCls.h"
#include "header/ColorShaderCls.h"

#include <windows.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsCls
{
public_fun:
    GraphicsCls();
    GraphicsCls(const GraphicsCls &);
    ~GraphicsCls();

    bool fInitialize(int, int, HWND);
    void fShutdown();
    bool fFrame();

private_fun:
    bool fRender();

private_mem:
    D3dCls * mDirect3D;
    CameraCls * mCamera;
    ModelCls * mModel;
    ColorShaderCls * mColorShader;
};
