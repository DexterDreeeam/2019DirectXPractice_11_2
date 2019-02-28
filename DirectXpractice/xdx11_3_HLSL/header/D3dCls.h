
#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "DDClsMacro.h"

#include <d3d11.h>
#include <d3dx10math.h>

class D3dCls
{
public_fun:
    D3dCls();
    D3dCls(const D3dCls &);
    ~D3dCls();

    bool fInitialize(int, int, bool, HWND, bool, float, float);
    void fShutdown();
    void fBeginScene(float, float, float, float);
    void fEndScene();

    ID3D11Device * fGetDevice();
    ID3D11DeviceContext * fGetDeviceContext();

    void fGetProjectionMatrix(D3DXMATRIX &);
    void fGetWorldMatrix(D3DXMATRIX &);
    void fGetOrthoMatrix(D3DXMATRIX &);

    void fGetVideoCardInfo(char *, int &);

private_mem:
    bool mVsyncEnabled;
    int mVideoCardMemory;
    char mVideoCardDescription[128];

    IDXGISwapChain * mSwapChain;
    ID3D11Device * mDevice;
    ID3D11DeviceContext * mDeviceContext;
    ID3D11RenderTargetView * mRenderTargetView;
    ID3D11Texture2D * mDepthStencilBuffer;
    ID3D11DepthStencilState * mDepthStencilState;
    ID3D11DepthStencilView * mDepthStencilView;
    ID3D11RasterizerState * mRasterState;

    D3DXMATRIX mProjectionMatrix;
    D3DXMATRIX mWorldMatrix;
    D3DXMATRIX mOrthoMatrix;
};