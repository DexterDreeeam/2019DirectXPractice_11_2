
#include "header/CameraCls.h"

CameraCls::CameraCls()
{
    mPositionX = 0.0f;
    mPositionY = 0.0f;
    mPositionZ = 0.0f;

    mRotationX = 0.0f;
    mRotationY = 0.0f;
    mRotationZ = 0.0f;
}

CameraCls::CameraCls(const CameraCls & other)
{
}

CameraCls::~CameraCls()
{
}

void CameraCls::gSetPosition(float x, float y, float z)
{
    mPositionX = x;
    mPositionY = y;
    mPositionZ = z;
    return;
}

void CameraCls::gSetRotation(float x, float y, float z)
{
    mRotationX = x;
    mRotationY = y;
    mRotationZ = z;
    return;
}

D3DXVECTOR3 CameraCls::fGetPosition()
{
    return D3DXVECTOR3(mPositionX, mPositionY, mPositionZ);
}

D3DXVECTOR3 CameraCls::fGetRotation()
{
    return D3DXVECTOR3(mRotationX, mRotationY, mRotationZ);
}

void CameraCls::fRender()
{
    D3DXVECTOR3 up;
    up = { 0.0f, 1.0f, 0.0f };

    D3DXVECTOR3 position;
    position = { mPositionX, mPositionY, mPositionZ };

    D3DXVECTOR3 lookAt;
    lookAt = { 0.0f, 0.0f, 1.0f };

    float yaw, pitch, roll;
    float toRadians = 0.0174532925f;
    pitch = mRotationX * toRadians;
    yaw = mRotationY * toRadians;
    roll = mRotationZ * toRadians;

    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
    D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    lookAt = position + lookAt;
    D3DXMatrixLookAtLH(&mViewMatrix, &position, &lookAt, &up);

    return;
}

void CameraCls::fGetViewMatrix(D3DXMATRIX & viewMatrix)
{
    viewMatrix = mViewMatrix;
    return;
}
