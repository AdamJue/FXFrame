//
// Created by yuntianming on 2024/6/25.
//

#include "GuiMeshPlugin.h"
#include "GuiMeshModule.h"

#ifdef FX_DYNAMIC_PLUGIN

FX_EXPORT void DllStartPlugin(FXIPluginManager* pm)
{
    CREATE_PLUGIN(pm, GuiMeshPlugin)
};

FX_EXPORT void DllStopPlugin(FXIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, GuiMeshPlugin)
};

#endif

const int GuiMeshPlugin::GetPluginVersion()
{
    return 0;
}

const std::string GuiMeshPlugin::GetPluginName()
{
    return GET_CLASS_NAME(GuiMeshPlugin);
}

void GuiMeshPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, IGuiMeshModule, GuiMeshModule);
}

void GuiMeshPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, IGuiMeshModule, GuiMeshModule);
}