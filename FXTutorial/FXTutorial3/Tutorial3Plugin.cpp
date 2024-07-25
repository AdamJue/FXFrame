#include "Tutorial3Plugin.h"
#include "HelloWorld3Module.h"

#ifdef FX_DYNAMIC_PLUGIN

FX_EXPORT void DllStartPlugin(FXIPluginManager* pm)
{
    CREATE_PLUGIN(pm, Tutorial3Plugin)
};

FX_EXPORT void DllStopPlugin(FXIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial3Plugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial3Plugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial3Plugin::GetPluginName()
{
	return GET_CLASS_NAME(Tutorial3Plugin);
}

void Tutorial3Plugin::Install()
{

    REGISTER_MODULE(pPluginManager, FXIHelloWorld3Module, FXCHelloWorld3Module)

}

void Tutorial3Plugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, FXIHelloWorld3Module, FXCHelloWorld3Module)
}
