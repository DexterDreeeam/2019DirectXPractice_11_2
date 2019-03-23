
#include "header/D3dCls.h"

D3dCls::D3dCls()
{
    mSwapChain = 0;
    mDevice = 0;
    mDeviceContext = 0;
    mRenderTargetView = 0;
    mDepthStencilBuffer = 0;
    mDepthStencilState = 0;
    mDepthStencilView = 0;
    mRasterState = 0;
}

D3dCls::D3dCls(const D3dCls & other)
{
}

D3dCls::~D3dCls()
{
}

bool D3dCls::fInitialize(
    int screenWidth, int screenHeight,
    bool vsync,
    HWND hwnd,
    bool fullScreen,
    float screenDepth, float screenNear
    )
{
    HRESULT rst;

    mVsyncEnabled = vsync;

    IDXGIFactory * factory;
    rst = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
    if(FAILED(rst)) return false;

    IDXGIAdapter * adapter; //choose graphics interface
    rst = factory->EnumAdapters(0, &adapter);
    if(FAILED(rst)) return false;

    IDXGIOutput * adapterOutput; //chose adapter monitor
    rst = adapter->EnumOutputs(0, &adapterOutput);
    if(FAILED(rst)) return false;

    unsigned int numModes;
    rst = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if(FAILED(rst)) return false;

    DXGI_MODE_DESC * displayModeList;
    displayModeList = new DXGI_MODE_DESC[numModes];
    if(!displayModeList) return false;

    rst = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if(FAILED(rst)) return false;

    unsigned int numerator, denominator;
    for(int i = 0; i != numModes; ++i)
    {
        if(displayModeList[i].Width == (unsigned int)screenWidth)
        {
            if(displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    DXGI_ADAPTER_DESC adapterDesc;
    rst = adapter->GetDesc(&adapterDesc);
    if(FAILED(rst)) return false;
    mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    int error;
    unsigned long long stringLength;
    error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
    if(error) return false;

    delete [] displayModeList;
    displayModeList = 0;

    adapterOutput->Release();
    adapterOutput = 0;

    adapter->Release();
    adapter = 0;

    factory->Release();
    factory = 0;
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    if(mVsyncEnabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    if(fullScreen)
    {
        swapChainDesc.Windowed = false;
    }
    else
    {
        swapChainDesc.Windowed = true;
    }
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    rst = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mSwapChain,
        &mDevice,
        NULL,
        &mDeviceContext
        );
    if(FAILED(rst)) return false;
    

    ID3D11Texture2D * backBufferPtr;
    rst = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backBufferPtr);
    if(FAILED(rst)) return false;

    rst = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
    if(FAILED(rst)) return false;

    backBufferPtr->Release();
    backBufferPtr = 0;

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = screenWidth;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;
    rst = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
    if(FAILED(rst)) return false;

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    rst = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
    if(FAILED(rst)) return false;

    mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    rst = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
    if(FAILED(rst)) return false;

    mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;
    rst = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
    if(FAILED(rst)) return false;

    mDeviceContext->RSSetState(mRasterState);

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    mDeviceContext->RSSetViewports(1, &viewport);

    float fieldOfView = 3.141592654f / 4.0f;
    float screenAspect = (float)screenWidth / (float)screenHeight;

    D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
    D3DXMatrixIdentity(&mWorldMatrix);
    D3DXMatrixOrthoLH(&mOrthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}

void D3dCls::fShutdown()
{
    if(mSwapChain)
        mSwapChain->SetFullscreenState(false, NULL);
    if(mRasterState)
    {
        mRasterState->Release();
        mRasterState = 0;
    }
    if(mDepthStencilView)
    {
        mDepthStencilView->Release();
        mDepthStencilView = 0;
    }
    if(mDepthStencilState)
    {
        mDepthStencilState->Release();
        mDepthStencilState = 0;
    }
    if(mDepthStencilBuffer)
    {
        mDepthStencilBuffer->Release();
        mDepthStencilBuffer = 0;
    }
    if(mRenderTargetView)
    {
        mRenderTargetView->Release();
        mRenderTargetView = 0;
    }
    if(mDeviceContext)
    {
        mDeviceContext->Release();
        mDeviceContext = 0;
    }
    if(mDevice)
    {
        mDevice->Release();
        mDevice = 0;
    }
    if(mSwapChain)
    {
        mSwapChain->Release();
        mSwapChain = 0;
    }
    return;
}

void D3dCls::fBeginScene(float red, float green, float blue, float alpha)
{
    float color[4] = { red, green, blue, alpha };
    mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
    mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    return;
}

void D3dCls::fEndScene()
{
    if(mVsyncEnabled)
    {
        mSwapChain->Present(1, 0);
    }
    else
    {
        mSwapChain->Present(0, 0);
    }
    return;
}

ID3D11Device * D3dCls::fGetDevice()
{
    return mDevice;
}

ID3D11DeviceContext * D3dCls::fGetDeviceContext()
{
    return mDeviceContext;
}

void D3dCls::fGetProjectionMatrix(D3DXMATRIX & projectionMatrix)
{
    projectionMatrix = mProjectionMatrix;
    return;
}

void D3dCls::fGetWorldMatrix(D3DXMATRIX & worldMatrix)
{
    worldMatrix = mWorldMatrix;
    return;
}

void D3dCls::fGetOrthoMatrix(D3DXMATRIX & orthoMatrix)
{
    orthoMatrix = mOrthoMatrix;
    return;
}

void D3dCls::fGetVideoCardInfo(char * cardName, int & memory)
{
    strcpy_s(cardName, 128, mVideoCardDescription);
    memory = mVideoCardMemory;
    return;
}