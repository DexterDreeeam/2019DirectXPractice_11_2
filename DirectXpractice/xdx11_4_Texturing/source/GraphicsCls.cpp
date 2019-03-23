
#include "header/GraphicsCls.h"

GraphicsCls::GraphicsCls()
{
    mDirect3D = 0;
    mCamera = 0;
    mModel = 0;
    //mColorShader = 0;
    mTextureShader = 0;
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

    mCamera = new CameraCls;
    if(!mCamera) return false;
    mCamera->gSetPosition(0.0f, 0.0f, -10.0f);

    mModel = new ModelCls;
    if(!mModel) return false;
    rst = mModel->fInitialize(mDirect3D->fGetDevice(), "./texture/seafloor.dds");
    if(!rst)
    {
        MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
        return false;
    }

    mTextureShader = new TextureShaderCls;
    if(!mTextureShader) return false;
    rst = mTextureShader->fInitialize(mDirect3D->fGetDevice(), hwnd);
    if(!rst)
    {
        MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsCls::fShutdown()
{
    if(mModel)
    {
        mModel->fShutdown();
        delete mModel;
        mModel = 0;
    }

    if(mCamera)
    {
        delete mCamera;
        mCamera = 0;
    }

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

    bool rst;
    D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

    mCamera->fRender();
    mCamera->fGetViewMatrix(viewMatrix);
    mDirect3D->fGetWorldMatrix(worldMatrix);
    mDirect3D->fGetProjectionMatrix(projectionMatrix);
    
    mModel->fRender(mDirect3D->fGetDeviceContext());
    rst = mTextureShader->fRender(
        mDirect3D->fGetDeviceContext(), 
        mModel->fGetIndexCount(), 
        worldMatrix, viewMatrix, projectionMatrix,
        mModel->fGetTexture()
        );
    if(!rst) return false;

    mDirect3D->fEndScene();

    return true;
}