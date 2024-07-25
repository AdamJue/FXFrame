#ifndef FX_PLATFORM_H
#define FX_PLATFORM_H

#define FX_PLATFORM_WIN 1
#define FX_PLATFORM_LINUX 2
#define FX_PLATFORM_APPLE 3
#define FX_PLATFORM_SYMBIAN 4
#define FX_PLATFORM_APPLE_IOS 5
#define FX_PLATFORM_ANDROID 6

#define FX_COMPILER_MSVC 1
#define FX_COMPILER_GNUC 2
#define FX_COMPILER_BORL 3
#define FX_COMPILER_WINSCW 4
#define FX_COMPILER_GCCE 5

#define FX_ENDIAN_LITTLE 1
#define FX_ENDIAN_BIG 2

#define FX_ARCHITECTURE_32 1
#define FX_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if defined( __GCCE__ )
#   define FX_COMPILER FX_COMPILER_GCCE
#   define FX_COMP_VER _MSC_VER
//# include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE
#elif defined( __WINSCW__ )
#   define FX_COMPILER FX_COMPILER_WINSCW
#   define FX_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define FX_COMPILER FX_COMPILER_MSVC
#   define FX_COMP_VER _MSC_VER
#elif defined( __GNUC__ )
#   define FX_COMPILER FX_COMPILER_GNUC
#   define FX_COMP_VER (((__GNUC__)*100) + \
                        (__GNUC_MINOR__*10) + \
                        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define FX_COMPILER FX_COMPILER_BORL
#   define FX_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if FX_COMPILER == FX_COMPILER_MSVC
#   if FX_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* Finds the current platform */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined( __SYMBIAN32__ )
#   define FX_PLATFORM FX_PLATFORM_SYMBIAN
//////////////////////////////////////////////////////////////////////////
#elif defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
#   define FX_PLATFORM FX_PLATFORM_WIN
//////////////////////////////////////////////////////////////////////////
#elif defined( __APPLE_CC__) || defined(__APPLE__) || defined(__OSX__)
// Device                                                     Simulator
// Both requiring OS version 4.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define FX_PLATFORM FX_PLATFORM_APPLE_IOS
#   else
#       define FX_PLATFORM FX_PLATFORM_APPLE
#   endif
//////////////////////////////////////////////////////////////////////////
#elif defined(linux) && defined(__arm__)
// TODO: This is NOT the correct way to detect the Tegra 2 platform but it works for now.
// It doesn't appear that GCC defines any platform specific macros.
#   define FX_PLATFORM FX_PLATFORM_TEGRA2
#elif defined(__ANDROID__)
#   define FX_PLATFORM FX_PLATFORM_ANDROID
//////////////////////////////////////////////////////////////////////////
#elif defined( __native_client__ )
#   define FX_PLATFORM FX_PLATFORM_NACL
#   ifndef FX_STATIC_LIB
#       error FX must be built as static for NaCl (FX_STATIC=true in cmake)
#   endif
#   ifdef FX_BUILD_RENDERSYSTEM_D3D9
#       error d3d9 is nor supported on NaCl (OFX_BUILD_RENDERSYSTEM_D3D9 false in cmake)
#   endif
#   ifdef FX_BUILD_RENDERSYSTEM_GL
#       error gl is nor supported on NaCl (OFX_BUILD_RENDERSYSTEM_GL=false in cmake)
#   endif
#   ifndef FX_BUILD_RENDERSYSTEM_GLES2
#       error GLES2 render system is needed for NaCl (OFX_BUILD_RENDERSYSTEM_GLES2=false in cmake)
#   endif
#else
#   define FX_PLATFORM FX_PLATFORM_LINUX
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define FX_ARCH_TYPE FX_ARCHITECTURE_64
#else
#   define FX_ARCH_TYPE FX_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define FX_QUOTE_INPLACE(x) # x
#define FX_QUOTE(x) FX_QUOTE_INPLACE(x)
#define FX_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if FX_PLATFORM == FX_PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include <Windows.h>
#define FX_EXPORT extern "C"  __declspec(dllexport)

#include <Dbghelp.h>
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#   if defined( FX_STATIC_LIB )
// Linux compilers don't have symbol import/export directives.
#       define _FXExport
#       define _FXPrivate
#   else
#       if defined( FX_NONCLIENT_BUILD )
#           define _FXExport __declspec( dllexport )
#       else
#           if defined( __MINGW32__ )
#               define _FXExport
#           else
#               define _FXExport __declspec( dllimport )
#           endif
#       endif
#       define _FXPrivate
#   endif
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define FX_DEBUG_MODE 1
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if FX_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || FX_COMP_VER > 345
#    define FX_UNICODE_SUPPORT 1
#   else
#    define FX_UNICODE_SUPPORT 0
#   endif
#  else
#   define FX_UNICODE_SUPPORT 1
#  endif
# else
#  define FX_UNICODE_SUPPORT 1
# endif
#else
#  define FX_UNICODE_SUPPORT 1
#endif

#endif // FX_PLATFORM == FX_PLATFORM_WIN
//----------------------------------------------------------------------------
// Android Settings
/*
#if FX_PLATFORM == FX_PLATFORM_ANDROID
#   define _FXExport
#   define FX_UNICODE_SUPPORT 1
#   define FX_DEBUG_MODE 0
#   define _FXPrivate
#     define CLOCKS_PER_SEC  1000
//  pragma def were found here: http://www.iFX.pucrs.br/~eduardob/disciplinas/SistEmbarcados/Mobile/Nokia/Tools/Carbide_vs/WINSCW/Help/PDF/C_Compilers_Reference_3.2.pdf
#     pragma warn_unusedarg off
#     pragma warn_emptydecl off
#     pragma warn_possunwant off
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define FX_DEBUG_MODE 1
#   else
#       define FX_DEBUG_MODE 0
#   endif
#endif
*/
//----------------------------------------------------------------------------
// Linux/Apple/iOs/Android/Symbian/Tegra2/NaCl Settings
#if FX_PLATFORM == FX_PLATFORM_LINUX || FX_PLATFORM == FX_PLATFORM_APPLE || FX_PLATFORM == FX_PLATFORM_APPLE_IOS || FX_PLATFORM == FX_PLATFORM_ANDROID || FX_PLATFORM == FX_PLATFORM_TEGRA2 || FX_PLATFORM == FX_PLATFORM_NACL

//#include <syswait.h>

// Enable GCC symbol visibility
#   if defined( FX_GCC_VISIBILITY )
#       define _FXExport  __attribute__ ((visibility("default")))
#       define _FXPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _FXExport
#       define _FXPrivate
#   endif

// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   if defined(_DEBUG) || defined(DEBUG)
#       define FX_DEBUG_MODE 1
#   endif

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define FX_UNICODE_SUPPORT 1
#define MAX_PATH 255

#define FX_EXPORT extern "C" __attribute ((visibility("default")))

#endif


//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN Config flag, set FX_ENDIAN correctly
#ifdef FX_Config_BIG_ENDIAN
#    define FX_ENDIAN FX_ENDIAN_BIG
#else
#    define FX_ENDIAN FX_ENDIAN_LITTLE
#endif

#include <stdint.h>
#include <chrono>

// Integer formats of fixed bit width
typedef uint32_t    FXUINT32;
typedef uint16_t    FXUINT16;
typedef uint8_t     FXUINT8;
typedef int32_t     FXINT32;
typedef int16_t     FXINT16;
typedef int8_t      FXINT8;
typedef uint64_t    FXUINT64;
typedef int64_t     FXINT64;


#if FX_PLATFORM == FX_PLATFORM_WIN
#include <crtdbg.h>
#include <assert.h>
#define FXASSERT(exp_, msg_, file_, func_)\
std::string strInfo("Message:");\
strInfo += msg_ + std::string(" don't exist or some warning") + std::string("\n File:") + std::string(file_) + std::string("\n Function:") + func_;\
MessageBoxA(0, strInfo.c_str(), ("Error_"#exp_), MB_RETRYCANCEL | MB_ICONERROR);\
assert(0);
#else
#define FXASSERT(exp_, msg_, file_, func_)
#endif


//#define GOOGLE_GLOG_DLL_DECL=

///////////////////////////////////////////////////////////////
#include <string>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <lexical_cast.hpp>

#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#define EPOCHFILETIME 11644473600000000ULL
#else
#include <windows.h>
#include <time.h>
#include <process.h>
#define EPOCHFILETIME 11644473600000000Ui64
#endif


#if FX_PLATFORM == FX_PLATFORM_WIN
#define FXSPRINTF sprintf_s
#define FXSTRICMP stricmp
#define FXSLEEP(s) Sleep(s)
#define FXGetPID() lexical_cast<std::string>(getpid())
#else
#define FXSPRINTF snprintf
#define FXSTRICMP strcasecmp
#define FXSLEEP(s) usleep(s)
#define FXGetPID() lexical_cast<std::string>(getpid())
#endif

#ifndef FX_DYNAMIC_PLUGIN
#define FX_DYNAMIC_PLUGIN
#endif

//use actor mode--begin
#define FX_ACTOR_THREAD_COUNT 1

#ifndef FX_USE_ACTOR
#define FX_USE_ACTOR
#endif

#ifdef FX_USE_ACTOR

#ifdef FX_DEBUG_MODE
#define THERON_DEBUG 1
#else
#define THERON_DEBUG 0
#endif

#ifndef THERON_CPP11
#define THERON_CPP11 1
#endif

#endif
//use actor mode--end

#define GET_CLASS_NAME(className) (#className)

#define FX_SHARE_PTR std::shared_ptr
#define FX_WEAK_PTR std::weak_ptr
#define FX_NEW new


template<typename DTYPE>
bool FX_StrTo(const std::string& strValue, DTYPE& nValue)
{
    try
    {
        nValue = lexical_cast<DTYPE>(strValue);
        return true;
    }
    catch (...)
    {
        return false;
    }

    return false;
}

inline bool IsZeroFloat(const float fValue, float epsilon = 1e-6)
{
    return std::abs(fValue) <= epsilon;
}

inline bool IsZeroDouble(const double dValue, double epsilon = 1e-15)
{
    return std::abs(dValue) <= epsilon;
}

inline int64_t FXGetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
//Protobuf Using Dlls
#if FX_PLATFORM == FX_PLATFORM_WIN
#ifndef PROTOBUF_SRC
#ifndef PROTOBUF_USE_DLLS
#define PROTOBUF_USE_DLLS
#endif
#endif
#endif

#endif
