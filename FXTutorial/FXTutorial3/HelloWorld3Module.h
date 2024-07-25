
#ifndef FXC_HELLO_WORLD3_H
#define FXC_HELLO_WORLD3_H

#include "FXIEventModule.h"
#include "FXIScheduleModule.h"

class FXIHelloWorld3Module
	: public FXIModule
{

};

class FXCHelloWorld3Module
    : public FXIHelloWorld3Module
{
public:
    FXCHelloWorld3Module(FXIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg);
    int OnHeartBeat(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
    unsigned long mLastTime;
protected:
	FXIEventModule* m_pEventModule;
	FXIScheduleModule* m_pScheduleModule;
};

#endif
