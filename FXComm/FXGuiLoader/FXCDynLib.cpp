
#include "FXCDynLib.h"

bool FXCDynLib::Load()
{
    std::string strLibPath = "./";
    strLibPath += mstrName;
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strLibPath.c_str());

    return mInst != NULL;
}

bool FXCDynLib::UnLoad()
{
    DYNLIB_UNLOAD(mInst);
    return true;
}

void* FXCDynLib::GetSymbol(const char* szProcName)
{
    return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}
