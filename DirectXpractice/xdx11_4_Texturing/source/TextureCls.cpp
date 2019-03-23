
#include "Header/TextureCls.h"

TextureCls::TextureCls()
{
    mTexture = 0;
}

TextureCls::TextureCls(const TextureCls & other)
{
}

TextureCls::~TextureCls()
{
}

bool TextureCls::fInitialize(ID3D11Device * device, const char * filename)
{
    HRESULT rst = D3DX11CreateShaderResourceViewFromFile(
        device,
        filename,
        NULL, NULL,
        &mTexture,
        NULL
        );
    if(FAILED(rst))
    {
        return false;
    }
    return true;
}

void TextureCls::fShutdown()
{
    if(mTexture)
    {
        mTexture->Release();
        mTexture = 0;
    }
    return;
}

ID3D11ShaderResourceView * TextureCls::fGetTexture()
{
    return mTexture;
}