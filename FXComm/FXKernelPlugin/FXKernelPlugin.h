///
#include "FXIPlugin.h"
#include "FXIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class FXKernelPlugin : public FXIPlugin
{
public:
    FXKernelPlugin(FXIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
