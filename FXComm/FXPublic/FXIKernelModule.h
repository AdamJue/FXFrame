//
// Created by yuntianming on 2024/7/24.
//

#ifndef FXFRAMETEST_FXIKERNELMODULE_H
#define FXFRAMETEST_FXIKERNELMODULE_H

#include <iostream>
#include <string>
#include <functional>
#include "FXList.hpp"
#include "FXIModule.h"
#include "FXGUID.h"

class FXIKernelModule : public FXIModule {
public:
    virtual FXGUID CreateGUID() = 0;

    virtual int Random(int nStart, int nEnd) = 0;
    virtual float Random() = 0;
};


#endif //FXFRAMETEST_FXIKERNELMODULE_H
