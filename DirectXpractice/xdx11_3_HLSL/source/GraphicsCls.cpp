
#include "header/GraphicsCls.h"

GraphicsCls::GraphicsCls()
{
    mDirect3D = 0;
}

GraphicsCls::GraphicsCls(const GraphicsCls & other)
{
}

GraphicsCls::~GraphicsCls()
{
}

bool GraphicsCls::fInitialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool rst;

    mDirect3D = new D3dCls;
    if(!mDirect3D) return false;
    
    rst = mDirect3D->fInitialize(
        screenWidth, screenHeight, 
        VSYNC_ENABLED, 
        hwnd, 
        FULL_SCREEN, 
        SCREEN_DEPTH, SCREEN_NEAR
        );
    if(!rst)
    {
        MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsCls::fShutdown()
{
    if(mDirect3D)
    {
        mDirect3D->fShutdown();
        delete mDirect3D;
        mDirect3D = 0;
    }
    return;
}

bool GraphicsCls::fFrame()
{
    bool rst;
    rst = fRender();
    if(!rst) return false;

    return true;
}

bool GraphicsCls::fRender()
{
    mDirect3D->fBeginScene(0.2f, 0.2f, 0.2f, 1.0f);
    mDirect3D->fEndScene();

    return true;
}