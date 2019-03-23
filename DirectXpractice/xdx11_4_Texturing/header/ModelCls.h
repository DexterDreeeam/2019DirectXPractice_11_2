
#pragma once

#include "DDClsMacro.h"
#include <d3d11.h>
#include <d3dx10math.h>

#include "header/textureCls.h"

class ModelCls
{
private_def:
    struct VertexType
    {
        D3DXVECTOR3 position;
        //D3DXVECTOR4 color;
        D3DXVECTOR2 texture;
    };

public_fun:
    ModelCls();
    ModelCls(const ModelCls &);
    ~ModelCls();

    bool fInitialize(ID3D11Device *, const char *);
    void fShutdown();
    void fRender(ID3D11DeviceContext *);
    int fGetIndexCount();
    ID3D11ShaderResourceView * fGetTexture();

private_fun:
    bool fInitializeBuffers(ID3D11Device *);
    void fShutdownBuffers();
    void fRenderBuffers(ID3D11DeviceContext *);
    bool fLoadTexture(ID3D11Device *, const char *);
    void fReleaseTexture();

private_mem:
    ID3D11Buffer * mVertexBuffer;
    ID3D11Buffer * mIndexBuffer;
    int mVertexCount, mIndexCount;
    TextureCls * mTexture;
};
