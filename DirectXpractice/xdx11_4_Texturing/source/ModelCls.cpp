
#include "header/ModelCls.h"

ModelCls::ModelCls()
{
    mVertexBuffer = 0;
    mIndexBuffer = 0;
    mTexture = 0;
}

ModelCls::ModelCls(const ModelCls & other)
{
}

ModelCls::~ModelCls()
{
}

bool ModelCls::fInitialize(ID3D11Device * device, const char * textureFilename)
{
    bool rst;
    rst = fInitializeBuffers(device); // buffer init
    if(!rst)
    {
        return false;
    }

    rst = fLoadTexture(device, textureFilename);
    if(!rst)
    {
        return false;
    }

    return true;
}

void ModelCls::fShutdown()
{
    fReleaseTexture();
    fShutdownBuffers();
    
    return;
}

void ModelCls::fRender(ID3D11DeviceContext * deviceContext)
{
    fRenderBuffers(deviceContext);

    return;
}

int ModelCls::fGetIndexCount()
{
    return mIndexCount;
}

ID3D11ShaderResourceView * ModelCls::fGetTexture()
{
    return mTexture->fGetTexture();
}

bool ModelCls::fInitializeBuffers(ID3D11Device * device)
{
    HRESULT rst;

    mVertexCount = 3;
    mIndexCount = 3;

    VertexType * vertices;
    vertices = new VertexType[mVertexCount];
    if(!vertices) return false;
    vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
    //vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
    vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    //vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
    vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
    //vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

    unsigned long * indices;
    indices = new unsigned long[mVertexCount];
    if(!indices) return false;
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    rst = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
    if(FAILED(rst)) return false;

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;
    
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    rst = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
    if(FAILED(rst)) return false;

    delete [] vertices;
    vertices = 0;
    delete [] indices;
    indices = 0;

    return true;
}

void ModelCls::fShutdownBuffers()
{
    if(mIndexBuffer)
    {
        mIndexBuffer->Release();
        mIndexBuffer = 0;
    }
    if(mVertexBuffer)
    {
        mVertexBuffer->Release();
        mVertexBuffer = 0;
    }

    return;
}

void ModelCls::fRenderBuffers(ID3D11DeviceContext * deviceContext)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}

bool ModelCls::fLoadTexture(ID3D11Device * device, const char * filename)
{
    bool rst;

    mTexture = new TextureCls;
    if(!mTexture)
    {
        return false;
    }
    rst = mTexture->fInitialize(device, filename);
    if(!rst)
    {
        return false;
    }

    return true;
}

void ModelCls::fReleaseTexture()
{
    if(mTexture)
    {
        mTexture->fShutdown();
        delete mTexture;
        mTexture = 0;
    }
    return;
}