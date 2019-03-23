
#include "header/SystemCls.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR pScmdline,
    int iCmdshow
    )
{
    SystemCls * sys;
    bool rst;

    sys = new SystemCls;
    if(!sys)
        return 0;

    rst = sys->fInitialize();
    if(rst)
    {
        sys->fRun();
    }

    sys->fShutdown();
    delete sys;
    sys = 0;

    return 0;
}