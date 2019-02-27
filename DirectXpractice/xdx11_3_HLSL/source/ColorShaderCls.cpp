
#include "header/ColorShaderCls.h"

ColorShaderCls::ColorShaderCls()
{
    mVertexShader = 0;
    mPixelShader = 0;
    mLayout = 0;
    mMatrixBuffer = 0;
}

ColorShaderCls::ColorShaderCls(const ColorShaderCls & other)
{
}

ColorShaderCls::~ColorShaderCls()
{
}

bool ColorShaderCls::fInitialize(ID3D11Device * device, HWND hwnd)
{
    bool rst;

    rst = fInitializeShader(device, hwnd, "./shader/color.vs", "./shader/color.ps");
    if(!rst) return false;

    return true;
}

void ColorShaderCls::fShutdown()
{
    fShutdownShader();

    return;
}

bool ColorShaderCls::fRender(
    ID3D11DeviceContext * deviceContext,
    int indexCount,
    D3DXMATRIX worldMatrix,
    D3DXMATRIX viewMatrix,
    D3DXMATRIX projectionMatrix
    )
{
    bool rst;
    rst = fSetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    if(!rst) return rst;

    fRenderShader(deviceContext, indexCount);

    return true;
}

bool ColorShaderCls::fInitializeShader(ID3D11Device * device, HWND hwnd, const char * vsFilename, const char * psFilename)
{
    HRESULT rst;
    ID3D10Blob * errorMessage = 0;
    ID3D10Blob * vertexShaderBuffer = 0;
    ID3D10Blob * pixelShaderBuffer = 0;

    rst = D3DX11CompileFromFile(
        vsFilename,
        NULL,
        NULL,
        "ColorVertexShader",
        "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        NULL,
        &vertexShaderBuffer,
        &errorMessage,
        NULL
        );
    if(FAILED(rst))
    {
        if(errorMessage)
        {
            fOutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        else
        {
            MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
        }
        return false;
    }
    rst = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);

    rst = D3DX11CompileFromFile(
        psFilename,
        NULL,
        NULL,
        "ColorPixelShader",
        "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        NULL,
        &pixelShaderBuffer,
        &errorMessage,
        NULL
        );
    if(FAILED(rst))
    {
        if(errorMessage)
        {
            fOutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        }
        else
        {
            MessageBox(hwnd, psFilename, "Missing Shader File", MB_OK);
        }
        return false;
    }
    rst = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;
    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    unsigned int numElements;
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
    rst = device->CreateInputLayout(
        polygonLayout,
        numElements,
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &mLayout
        );
    if(FAILED(rst)) return false;

    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;
    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;
    rst = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
    if(FAILED(rst)) return false;

    return true;
}

void ColorShaderCls::fShutdownShader()
{
    if(mMatrixBuffer)
    {
        mMatrixBuffer->Release();
        mMatrixBuffer = 0;
    }

    if (mLayout)
    {
        mLayout->Release();
        mLayout = 0;
    }

    if (mVertexShader)
    {
        mVertexShader->Release();
        mVertexShader = 0;
    }

    if (mPixelShader)
    {
        mPixelShader->Release();
        mPixelShader = 0;
    }

    return;
}

void ColorShaderCls::fOutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, const char * shaderFilename)
{
    char * compileErrors;
    compileErrors = (char *)(errorMessage->GetBufferPointer());

    unsigned long bufferSize;
    bufferSize = errorMessage->GetBufferSize();

    std::ofstream fout;
    fout.open("sahder_error.txt");
    for(int i = 0; i != bufferSize; ++i)
    {
        fout << compileErrors[i];
    }
    fout.close();

    errorMessage->Release();
    errorMessage = 0;

    MessageBox(hwnd, "Error compiling shader. Check shader_error.txt for message.", shaderFilename, MB_OK);

    return;
}

bool ColorShaderCls::fSetShaderParameters(
    ID3D11DeviceContext * deviceContext,
    D3DXMATRIX worldMatrix,
    D3DXMATRIX viewMatrix,
    D3DXMATRIX projectionMatrix
    )
{
    HRESULT rst;

    D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
    D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
    D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    rst = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(rst)) return false;

    MatrixBufferType * dataPtr;
    dataPtr = (MatrixBufferType *)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    unsigned int bufferNumber = 0;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

    return true;
}

void ColorShaderCls::fRenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(mLayout);
    deviceContext->VSSetShader(mVertexShader, NULL, 0);
    deviceContext->PSSetShader(mPixelShader, NULL, 0);
    
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}