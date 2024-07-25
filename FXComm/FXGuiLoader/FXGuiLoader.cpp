#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include "FXCPluginManager.h"
#include "FXPlatform.h"

#include <QApplication>
#include "FXMainWindow.h"


#if FX_PLATFORM == FX_PLATFORM_WIN
#include <Windows.h>
#include <codecvt>
#include <dbghelp.h>  // Required for the MiniDumpWriteDump function
#endif

#if FX_PLATFORM == FX_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/prctl.h>
#endif

bool bExitApp = false;
std::thread gThread;
std::string strArgvList;
std::string strPluginName;
std::string strAppName;
std::string strAppID;
std::string strTitleName;

#if FX_PLATFORM == FX_PLATFORM_WIN

#pragma comment( lib, "DbgHelp" )

void CreateDumpFile(const std::wstring& strDumpFilePathName, EXCEPTION_POINTERS* pException)
{
    //Dump
    HANDLE hDumpFile = CreateFileW(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

std::wstring getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    struct tm local_tm;
    localtime_s(&local_tm, &now_time_t);

    wchar_t timestamp[32];
    int result = swprintf(timestamp, 32, L"%.4d%.2d%.2d-%.2d%.2d%.2d",
                          local_tm.tm_year + 1900, local_tm.tm_mon + 1,
                          local_tm.tm_mday, local_tm.tm_hour,
                          local_tm.tm_min, local_tm.tm_sec);

    if (result < 0 || result >= 32) {
        throw std::runtime_error("Timestamp formatting failed or resulted in overflow");
    }
    return timestamp;
}

std::wstring generateDumpFileName() {
    std::wstring timestamp = getCurrentTimestamp();

    constexpr size_t MAX_PATH_LENGTH = 260; // MAX_PATH 的值为 260
    if (timestamp.length() + 9 >= MAX_PATH_LENGTH) {  // "dump-.dmp" 长度为9
        throw std::runtime_error("Generated dump file name would exceed MAX_PATH limit");
    }

    wchar_t dumpName[MAX_PATH_LENGTH];
    int result = swprintf(dumpName, MAX_PATH_LENGTH, L"dump-%ls.dmp", timestamp.c_str());
    if (result < 0 || static_cast<size_t>(result) >= MAX_PATH_LENGTH) {
        throw std::runtime_error("Dump file name formatting failed or resulted in overflow");
    }
    return dumpName;
}

long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    std::wstring dumpFileName = generateDumpFileName();
    CreateDumpFile(dumpFileName, pException);

    FatalAppExit(-1,  dumpFileName.c_str());
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void CloseXButton()
{
#if FX_PLATFORM == FX_PLATFORM_WIN
	HWND hWnd = GetConsoleWindow();
	if (hWnd)
	{
		HMENU hMenu = GetSystemMenu(hWnd, FALSE);
		EnableMenuItem(hMenu, SC_CLOSE, MF_DISABLED | MF_BYCOMMAND);
	}
#endif
}

void ThreadFunc()
{
    while (!bExitApp)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string s;
        std::cin >> s;
        // if ( 0 == _stricmp( s.c_str(), "exit" ) )
        if(s.compare("exit")==0)
        {
            bExitApp = true;
            gThread.detach();
        }
    }
}

void CreateBackThread()
{
    gThread = std::thread(std::bind(&ThreadFunc));
    auto f = std::async (std::launch::deferred, std::bind(ThreadFunc));
    std::cout << "CreateBackThread, thread ID = " << gThread.get_id() << std::endl;
}

void InitDaemon()
{
#if FX_PLATFORM == FX_PLATFORM_LINUX
    daemon(1, 0);

    // ignore signals
    signal(SIGINT,  SIG_IGN);
    signal(SIGHUP,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
#endif
}

void PrintfLogo()
{
#if FX_PLATFORM == FX_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

    std::cout << "\n" << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "**                 FXFrame                    **" << std::endl;
    std::cout << "**       Copyright (c) 2024-2094, Felix       **" << std::endl;
    std::cout << "**             All rights reserved.           **" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "\n" << std::endl;
	std::cout << "Instance: name.xml File's name to instead of \"Plugin.xml\" when programs be launched, all platform" << std::endl;
	std::cout << "Instance: \"ID=number\", \"Server=GUIServer\"  when programs be launched, all platform" << std::endl;
	std::cout << "\n" << std::endl;

#if FX_PLATFORM == FX_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void ProcessParameter(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
	{
		strArgvList += " ";
		strArgvList += argv[i];
	}

#if FX_PLATFORM == FX_PLATFORM_WIN
	if (strArgvList.find("-x") != string::npos)
	{
		CloseXButton();
	}
#elif FX_PLATFORM == FX_PLATFORM_LINUX
    //run it as a daemon process
	if (strArgvList.find("-d") != string::npos)
	{
		InitDaemon();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
#endif

	if (strArgvList.find(".xml") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strPluginName = argv[i];
			if (strPluginName.find(".xml") != string::npos)
			{
				break;
			}
		}

		FXCPluginManager::GetSingletonPtr()->SetConfigName(strPluginName);
	}

    if (strArgvList.find("Server=") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strAppName = argv[i];
			if (strAppName.find("Server=") != string::npos)
			{
                strAppName.erase(0, 7);
				break;
			}
		}

		FXCPluginManager::GetSingletonPtr()->SetAppName(strAppName);
	}

	if (strArgvList.find("ID=") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strAppID = argv[i];
			if (strAppID.find("ID=") != string::npos)
			{
                strAppID.erase(0, 3);
				break;
			}
		}

		int nAppID = 0;
        if(FX_StrTo(strAppID, nAppID))
        {
            FXCPluginManager::GetSingletonPtr()->SetAppID(nAppID);
        }
	}

	strTitleName = strAppName + strAppID;// +" PID" + FXGetPID();
//	strTitleName.replace(strTitleName.find("Server"), 6, "");
	strTitleName = "FX" + strTitleName;
#if FX_PLATFORM == FX_PLATFORM_WIN
//	SetConsoleTitle(strTitleName.c_str());
#else
	prctl(PR_SET_NAME, strTitleName.c_str());
	//setproctitle(strTitleName.c_str());
#endif
}

int main(int argc, char* argv[])
{
#if FX_PLATFORM == FX_PLATFORM_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#elif FX_PLATFORM == FX_PLATFORM_LINUX
    // TODO linux sigal
#endif
    PrintfLogo();
    ProcessParameter(argc, argv);

    FXCPluginManager::GetSingletonPtr()->Awake();
    FXCPluginManager::GetSingletonPtr()->Init();
    FXCPluginManager::GetSingletonPtr()->AfterInit();
    FXCPluginManager::GetSingletonPtr()->CheckConfig();
    FXCPluginManager::GetSingletonPtr()->ReadyExecute();

    QApplication app(argc, argv);

    app.setOrganizationName("XXXXXXXX");
    app.setApplicationName("xxxxxxx 3.0");
    // 创建主窗口
    FXMainWindow mainWindow;
    mainWindow.setWindowTitle("原型软件");
    // 显示主窗口
    mainWindow.show();

    return app.exec();
}