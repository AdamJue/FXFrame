#include "FXPlatform.h"
#include <iostream>
#if FX_PLATFORM == FX_PLATFORM_WIN
#include <Windows.h>
#endif

#if FX_PLATFORM == FX_PLATFORM_WIN
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
//            std::cout << "GUIMeshPlugin DLL_PROCESS_ATTACH" << std::endl;
            break;

        case DLL_THREAD_ATTACH:

            break;

        case DLL_THREAD_DETACH:

            break;

        case DLL_PROCESS_DETACH:
//            std::cout << "GUIMeshPlugin DLL_PROCESS_DETACH" << std::endl;
            break;
    }


    return TRUE;
}
#endif

