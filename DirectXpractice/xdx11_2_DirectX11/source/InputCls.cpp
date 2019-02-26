
#include "header/InputCls.h"

InputCls::InputCls()
{
}

InputCls::InputCls(const InputCls & other)
{
}

InputCls::~InputCls()
{
}

void InputCls::fInitialize()
{
    for(int i = 0; i != 256; ++i)
    {
        mKeys[i] = false;
    }
    return;
}

void InputCls::fKeyDown(unsigned int input)
{
    mKeys[input] = true;
    return;
}

void InputCls::fKeyUp(unsigned int input)
{
    mKeys[input] = false;
    return;
}

bool InputCls::fIsKeyDown(unsigned int key)
{
    return mKeys[key];
}