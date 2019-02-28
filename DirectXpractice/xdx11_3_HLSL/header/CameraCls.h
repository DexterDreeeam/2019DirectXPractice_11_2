
#pragma once

#include "DDClsMacro.h"
#include <d3dx10math.h>

class CameraCls
{
public_fun:
    CameraCls();
    CameraCls(const CameraCls &);
    ~CameraCls();
    
    void gSetPosition(float, float, float);
    void gSetRotation(float, float, float);
    D3DXVECTOR3 fGetPosition();
    D3DXVECTOR3 fGetRotation();
    void fRender();
    void fGetViewMatrix(D3DXMATRIX &);

private_mem:
    float mPositionX, mPositionY, mPositionZ;
    float mRotationX, mRotationY, mRotationZ;
    D3DXMATRIX mViewMatrix;
};