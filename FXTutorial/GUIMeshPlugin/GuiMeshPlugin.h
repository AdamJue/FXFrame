//
// Created by yuntianming on 2024/6/25.
//

#ifndef FXFRAMETEST_GUIMESHPLUGIN_H
#define FXFRAMETEST_GUIMESHPLUGIN_H
#include "FXIPlugin.h"
#include "FXIPluginManager.h"

class GuiMeshPlugin : public FXIPlugin
{
public:
    GuiMeshPlugin(FXIPluginManager *p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};


#endif //FXFRAMETEST_GUIMESHPLUGIN_H
