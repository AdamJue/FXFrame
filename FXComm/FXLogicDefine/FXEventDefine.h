//
// Created by yuntianming on 2024/7/10.
//

#ifndef FXFRAMETEST_FXEVENTDEFINE_H
#define FXFRAMETEST_FXEVENTDEFINE_H

enum FXEventDefine
{
    FXED_UNKNOW = 0,
    ////////////////Has Self---logic//////////////////////////////////////////////////////////
    //arg[0]:resultID[EErrorCode]
    FXED_ON_GENERAL_MESSAGE,

    ////////////////core events for FX--start////////////////////////////////////////////////////////
    FXED_START_GENERATAGE_MESH = 2,
    FXED_COMMON_GUI = 3,
    //common property
    //common record

    ////////////////core events for FX--end//////////////////////////////////////////////////////////

    FXED_CUSTOM_EVENT_BEGIN = 10000,

    FXED_CUSTOM_EVENT_END = 30000,

};

#endif //FXFRAMETEST_FXEVENTDEFINE_H
