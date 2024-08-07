//
// Created by yuntianming on 2024/7/24.
//

#ifndef FXFRAMETEST_FXCKERNELMODULE_H
#define FXFRAMETEST_FXCKERNELMODULE_H
#include <iostream>

#include <string>
#include <random>
#include <chrono>
#include "FXIKernelModule.h"
#include "FXIPluginManager.h"

class FXCKernelModule : public FXIKernelModule{

public:
    FXCKernelModule(FXIPluginManager * p);
    virtual ~FXCKernelModule();

    virtual bool Init() override;
    virtual bool Shut() override;

    virtual bool BeforeShut() override;
    virtual bool AfterInit() override;

    virtual bool Execute() override;

    //////////////////////////////////////////
    virtual FXGUID CreateGUID() override;

    virtual float Random() override;
    virtual int Random(int nStart, int nEnd) override;

protected:
    void InitRandom();

private:
    std::vector<float> mvRandom;
    int nGUIDIndex;
    decltype(mvRandom.cbegin()) mxRandomItor;
};


#endif //FXFRAMETEST_FXCKERNELMODULE_H
