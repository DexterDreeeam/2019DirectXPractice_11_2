
#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>

#include "header/DDClsMacro.h"

class TextureCls
{
public_fun:
    TextureCls();
    TextureCls(const TextureCls &);
    ~TextureCls();

    bool fInitialize(ID3D11Device *, const char *);
    void fShutdown();
    ID3D11ShaderResourceView * fGetTexture();

private_mem:
    ID3D11ShaderResourceView * mTexture;
};