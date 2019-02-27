
#pragma once

#include "DDClsMacro.h"
#include <d3d11.h>
#include <d3dx10math.h>

class ModelCls
{
private_def:
    struct VertexType
    {
        D3DXVECTOR3 position;
        D3DXVECTOR4 color;
    };

public_fun:
    ModelCls();
    ModelCls(const ModelCls &);
    ~ModelCls();

    bool fInitialize(ID3D11Device *);
    void fShutdown();
    void fRender(ID3D11DeviceContext *);
    int fGetIndexCount();

private_fun:
    bool fInitializeBuffers(ID3D11Device *);
    void fShutdownBuffers();
    void fRenderBuffers(ID3D11DeviceContext *);

private_mem:
    ID3D11Buffer * mVertexBuffer;
    ID3D11Buffer * mIndexBuffer;
    int mVertexCount, mIndexCount;

};
