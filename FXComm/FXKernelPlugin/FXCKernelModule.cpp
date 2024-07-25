//
// Created by yuntianming on 2024/7/24.
//

#include "FXCKernelModule.h"

FXCKernelModule::FXCKernelModule(FXIPluginManager *p)
{
    nGUIDIndex = 0;
    InitRandom();
}

FXCKernelModule::~FXCKernelModule()
{

}

void FXCKernelModule::InitRandom()
{
    mvRandom.clear();

    constexpr int nRandomMax = 100000;
    mvRandom.reserve(nRandomMax);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0f);

    std::generate(mvRandom.begin(), mvRandom.end(), [&]() { return (float)dis(gen); });

    mxRandomItor = mvRandom.cbegin();
}

bool FXCKernelModule::Init()
{
    return true;
}

bool FXCKernelModule::AfterInit()
{
    return true;
}

bool FXCKernelModule::BeforeShut()
{
    return true;
}

bool FXCKernelModule::Shut()
{
    return true;
}

bool FXCKernelModule::Execute()
{
    return true;
}

FXGUID FXCKernelModule::CreateGUID()
{
    int64_t value = 0;
    uint64_t time = FXGetTime();

    value = time * 1000000;

    value += nGUIDIndex++;

    if (nGUIDIndex == 999999)
    {
        nGUIDIndex = 0;
    }

    FXGUID xID;
    xID.nHead64 = pPluginManager->GetAppID();
    xID.nData64 = value;

    return xID;
}

int FXCKernelModule::Random(int nStart, int nEnd)
{
    if (++mxRandomItor == mvRandom.cend())
    {
        mxRandomItor = mvRandom.cbegin();
    }

    return static_cast<int>((nEnd - nStart) * *mxRandomItor) + nStart;
}

float FXCKernelModule::Random()
{
    if (++mxRandomItor == mvRandom.cend())
    {
        mxRandomItor = mvRandom.cbegin();
    }

    return *mxRandomItor;
}