
#ifndef FXC_DYNLIB_H
#define FXC_DYNLIB_H

#include <stdio.h>
#include <iostream>
#include "FXPlatform.h"

#if FX_PLATFORM == FX_PLATFORM_WIN
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryExA( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif FX_PLATFORM == FX_PLATFORM_LINUX || FX_PLATFORM == FX_PLATFORM_ANDROID
#include <dlfcn.h>
#define DYNLIB_HANDLE void*
#define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#define DYNLIB_UNLOAD( a ) dlclose( a )

#elif FX_PLATFORM == FX_PLATFORM_APPLE || FX_PLATFORM == FX_PLATFORM_APPLE_IOS
#include <dlfcn.h>
#define DYNLIB_HANDLE void*
#define DYNLIB_LOAD( a ) dlopen( a, RTLD_LOCAL|RTLD_LAZY)
#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#define DYNLIB_UNLOAD( a ) dlclose( a )

#endif

class FXCDynLib
{

public:

    FXCDynLib(const std::string& strName)
    {
        mbMain = false;
        mstrName = strName;
#ifdef FX_DEBUG_MODE
       // mstrName.append("_d");
#endif

#if FX_PLATFORM == FX_PLATFORM_WIN
        mstrName.append(".dll");
#elif FX_PLATFORM == FX_PLATFORM_LINUX || FX_PLATFORM == FX_PLATFORM_ANDROID
        mstrName.append(".so");
#elif FX_PLATFORM == FX_PLATFORM_APPLE || FX_PLATFORM == FX_PLATFORM_APPLE_IOS
        mstrName.append(".so");
#endif

//        printf("LoadPlugin:%s\n", mstrName.c_str());
    }

    ~FXCDynLib()
    {

    }

    bool Load();

    bool UnLoad();

    /// Get the name of the library
    const std::string& GetName(void) const
    {
        return mstrName;
    }

    const bool GetMain(void) const
    {
        return mbMain;
    }

    void* GetSymbol(const char* szProcName);

protected:

    std::string mstrName;
    bool mbMain;

    DYNLIB_HANDLE mInst;
};

#endif
