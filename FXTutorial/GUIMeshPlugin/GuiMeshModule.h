//
// Created by yuntianming on 2024/6/25.
//

#ifndef FXFRAMETEST_GUIMESHMODULE_H
#define FXFRAMETEST_GUIMESHMODULE_H

#include "FXIModule.h"
#include "FXIScheduleModule.h"
#include "FXIEventModule.h"

class IGuiMeshModule : public FXIModule {
};

class GuiMeshModule : public IGuiMeshModule {

public:
    GuiMeshModule(FXIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init() override;
    virtual bool AfterInit() override;

    virtual bool Execute() override;

    virtual bool BeforeShut() override;
    virtual bool Shut() override;

public:

    void LoginToMesher(const std::string &strAccount, const std::string &strPwd, const std::string &strKey);

    void ReqGenerateMeshWithCompression();

protected:
    int OnHeartBeat(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);
    int OnHeartBeatTestAsyn(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);
    int OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg);


protected:

    FXIScheduleModule* m_pScheduleModule;
    FXIEventModule* m_pEventModule;

};


#endif //FXFRAMETEST_GUIMESHMODULE_H
