
#ifndef FX_TUTORIAL3_H
#define FX_TUTORIAL3_H

///
#include "FXIPlugin.h"
#include "FXIPluginManager.h"

class Tutorial3Plugin : public FXIPlugin
{
public:
    Tutorial3Plugin(FXIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif
