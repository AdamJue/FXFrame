//
// Created by yuntianming on 2024/6/25.
//

#include "GuiMeshModule.h"
#include "FXIPluginManager.h"
#include "zlibhelper.h"
#include <fstream>
#include <thread>

using namespace std;

bool GuiMeshModule::Init() {

   // std::cout << "Hello, GuiMeshModule, Init" << std::endl;

    m_pScheduleModule = pPluginManager->FindModule<FXIScheduleModule>();
    m_pEventModule = pPluginManager->FindModule<FXIEventModule>();

    return true;
}

bool GuiMeshModule::AfterInit()  {

  //  std::cout << "Hello, GuiMeshModule, AfterInit" << std::endl;

    m_pEventModule->AddEventCallBack(FXGUID(0, 1), FXEventDefine::FXED_START_GENERATAGE_MESH, this, &GuiMeshModule::OnEvent);

    m_pScheduleModule->AddSchedule(FXGUID(0, 1), "GuiMeshModule::OnHeartBeat", this, &GuiMeshModule::OnHeartBeat, 1.0f, 1);

    return true;
}


bool GuiMeshModule::Execute() {
//    std::cout << "Hello, GuiMeshModule, Execute" << std::endl;
    return true;
}

bool GuiMeshModule::BeforeShut() {
  //  std::cout << "Hello, GuiMeshModule, BeforeShut" << std::endl;
    return true;
}

bool GuiMeshModule::Shut() {

  //  std::cout << "Hello, GuiMeshModule, Shut" << std::endl;

    return true;
}


void GuiMeshModule::LoginToMesher(const std::string &strAccount, const std::string &strPwd, const std::string &strKey)
{

    m_pEventModule->DoAsynEvent(FXGUID(0,1), FXEventDefine::FXED_COMMON_GUI, FXDataList() << int(1) << "200");

 //   std::cout << "LoginToMesher" << std::endl;
}


int GuiMeshModule::OnHeartBeat(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    std::string strAccount = "felix";
    std::string strKey = "123456";
    LoginToMesher(strAccount, strKey,"");

    return 0;
}

int GuiMeshModule::OnHeartBeatTestAsyn(const FXGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    int percent = 100 - nCount;

    m_pEventModule->DoEvent(FXGUID(0,1), FXEventDefine::FXED_COMMON_GUI, FXDataList() << int(2) << percent);
    return 0;
}

void GuiMeshModule::ReqGenerateMeshWithCompression()
{
    if (m_pScheduleModule->ExistSchedule(FXGUID(0, 1), "GuiMeshModule::OnHeartBeatTestAsyn"))
    {
        m_pScheduleModule->RemoveSchedule(FXGUID(0, 1), "GuiMeshModule::OnHeartBeatTestAsyn");
    }
    m_pScheduleModule->AddSchedule(FXGUID(0, 1), "GuiMeshModule::OnHeartBeatTestAsyn", this, &GuiMeshModule::OnHeartBeatTestAsyn, 2.0f, 100);
}

int GuiMeshModule::OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg)
{
 //   std::cout << "this thread id:" << std::this_thread::get_id() << std::endl;
//    std::cout << "GuiMeshModule OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;
    ReqGenerateMeshWithCompression();

    return 0;
}