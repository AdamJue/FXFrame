#include "HelloWorld3Module.h"
#include "Tutorial3Plugin.h"
#include "FXIEventModule.h"

bool FXCHelloWorld3Module::Init()
{

	std::cout << "Hello, world3, Init" << std::endl;

	return true;
}

int FXCHelloWorld3Module::OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg)
{

	std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;


	return 0;
}

int FXCHelloWorld3Module::OnHeartBeat(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{

	unsigned long unNowTime = FXGetTime();

	std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

	mLastTime = unNowTime;

	return 0;
}



bool FXCHelloWorld3Module::AfterInit()
{

	std::cout << "Hello, world3, AfterInit" << std::endl;

	m_pEventModule = pPluginManager->FindModule<FXIEventModule>();
	m_pScheduleModule = pPluginManager->FindModule<FXIScheduleModule>();

	m_pEventModule->AddEventCallBack(FXGUID(0, 1), FXEventDefine(1), this, &FXCHelloWorld3Module::OnEvent);

	m_pScheduleModule->AddSchedule(FXGUID(0, 1), "OnHeartBeat", this, &FXCHelloWorld3Module::OnHeartBeat, 5.0f, 10);
//	m_pScheduleModule->AddSchedule(FXGUID(0, 1), "OnHeartBeat2", this, &FXCHelloWorld3Module::OnHeartBeat, 5.0f, 10);

	mLastTime = FXGetTime();


	m_pEventModule->DoEvent(FXGUID(0,1), FXEventDefine(1), FXDataList() << int(100) << "200");

	return true;
}

bool FXCHelloWorld3Module::Execute()
{

	//std::cout << "Hello, world3, Execute" << std::endl;

	return true;
}

bool FXCHelloWorld3Module::BeforeShut()
{

	std::cout << "Hello, world3, BeforeShut" << std::endl;

//	m_pKernelModule->DestroyAll();

	return true;
}

bool FXCHelloWorld3Module::Shut()
{

	std::cout << "Hello, world3, Shut" << std::endl;

	return true;
}
