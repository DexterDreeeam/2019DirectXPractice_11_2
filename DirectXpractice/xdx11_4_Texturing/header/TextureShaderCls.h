
#pragma once

#include "DDClsMacro.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

class TextureShaderCls
{
private_def:
    struct MatrixBufferType
    {
        D3DXMATRIX world;
        D3DXMATRIX view;
        D3DXMATRIX projection;
    };

public_fun:
    TextureShaderCls();
    TextureShaderCls(const TextureShaderCls &);
    ~TextureShaderCls();

    bool fInitialize(ID3D11Device *, HWND);
    void fShutdown();
    bool fRender(ID3D11DeviceContext *, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *);

private_fun:
    bool fInitializeShader(ID3D11Device *, HWND, const char *, const char *);
    void fShutdownShader();
    void fOutputShaderErrorMessage(ID3D10Blob *, HWND, const char *);
    bool fSetShaderParameters(ID3D11DeviceContext *, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *);
    void fRenderShader(ID3D11DeviceContext *, int);

private_mem:
    ID3D11VertexShader * mVertexShader;
    ID3D11PixelShader * mPixelShader;
    ID3D11InputLayout * mLayout;
    ID3D11Buffer * mMatrixBuffer;
    ID3D11SamplerState * mSampleState;
};