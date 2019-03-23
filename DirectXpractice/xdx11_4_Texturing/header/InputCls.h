
#pragma once

#include "header/DDClsMacro.h"

class InputCls
{
public_fun:
    InputCls();
    InputCls(const InputCls &);
    ~InputCls();

    void fInitialize();
    void fKeyDown(unsigned int);
    void fKeyUp(unsigned int);
    bool fIsKeyDown(unsigned int);

private_mem:
    bool mKeys[256];
};