
#include "FXKernelPlugin.h"
#include "FXCEventModule.h"
#include "FXCScheduleModule.h"
#include "FXCKernelModule.h"

//
//
#ifdef FX_DYNAMIC_PLUGIN

FX_EXPORT void DllStartPlugin(FXIPluginManager* pm)
{
    CREATE_PLUGIN(pm, FXKernelPlugin)

};

FX_EXPORT void DllStopPlugin(FXIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, FXKernelPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int FXKernelPlugin::GetPluginVersion()
{
    return 0;
}

const std::string FXKernelPlugin::GetPluginName()
{
	return GET_CLASS_NAME(FXKernelPlugin);
}

void FXKernelPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, FXIEventModule, FXCEventModule)
	REGISTER_MODULE(pPluginManager, FXIScheduleModule, FXCScheduleModule)
	REGISTER_MODULE(pPluginManager, FXIKernelModule, FXCKernelModule)
}

void FXKernelPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, FXIScheduleModule, FXCScheduleModule)
    UNREGISTER_MODULE(pPluginManager, FXIEventModule, FXCEventModule)
    UNREGISTER_MODULE(pPluginManager, FXIKernelModule, FXCKernelModule)
}
