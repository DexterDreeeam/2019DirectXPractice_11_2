
#include "header/GraphicsCls.h"

GraphicsCls::GraphicsCls()
{
}

GraphicsCls::GraphicsCls(const GraphicsCls & other)
{
}

GraphicsCls::~GraphicsCls()
{
}

bool GraphicsCls::fInitialize(int screenWidth, int screenHeight, HWND hwnd)
{
    return true;
}

void GraphicsCls::fShutdown()
{
    return;
}

bool GraphicsCls::fFrame()
{
    return true;
}

bool GraphicsCls::fRender()
{
    return true;
}