#include "FXCPluginManager.h"
#include "rapidxml/rapidxml.hpp"
#include "FXIPlugin.h"
#include "FXPlatform.h"
#include <fstream>

#if FX_PLATFORM == FX_PLATFORM_WIN
#pragma comment( lib, "ws2_32.lib" )
#endif


FXCPluginManager::FXCPluginManager() : FXIPluginManager()
{
   mnAppID = 0;
   mnInitTime = time(NULL);
   mnNowTime = mnInitTime;

   mGetFileContentFunctor = nullptr;

   mstrConfigPath = "";
   mstrConfigName = "Plugin.xml";
}

FXCPluginManager::~FXCPluginManager()
{

}

bool FXCPluginManager::Awake()
{
	LoadPluginConfig();

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (it; it != mPluginNameMap.end(); ++it)
	{
#ifdef FX_DYNAMIC_PLUGIN
		LoadPluginLibrary(it->first);
#else
		LoadStaticPlugin(it->first);
#endif
	}


	PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
	for (itAfterInstance; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
	{
		itAfterInstance->second->Awake();
	}

	return true;
}

inline bool FXCPluginManager::Init()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		itInstance->second->Init();
	}

	return true;
}

bool FXCPluginManager::LoadPluginConfig()
{
	std::string strContent;
	bool bRead = GetFileContent(mstrConfigName, strContent);
    if (!bRead || strContent.empty())
    {
        return false;
    }

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)strContent.c_str());

    rapidxml::xml_node<>* pRoot = xDoc.first_node();
    if (!pRoot)
    {
        return false;
    }
    rapidxml::xml_node<>* pAppNameNode = pRoot->first_node(mstrAppName.c_str());
    if (!pAppNameNode)
    {
        FXASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    for (rapidxml::xml_node<>* pPluginNode = pAppNameNode->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin"))
    {
        const char* strPluginName = pPluginNode->first_attribute("Name")->value();

        mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, true));

    }

/*
    rapidxml::xml_node<>* pPluginAppNode = pAppNameNode->first_node("APPID");
    if (!pPluginAppNode)
    {
        FXASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    const char* strAppID = pPluginAppNode->first_attribute("Name")->value();
    if (!strAppID)
    {
        FXASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    if (!FX_StrTo(strAppID, mnAppID))
    {
        FXASSERT(0, "App ID Convert Error", __FILE__, __FUNCTION__);
        return false;
    }
*/
    rapidxml::xml_node<>* pPluginConfigPathNode = pAppNameNode->first_node("ConfigPath");
    if (!pPluginConfigPathNode)
    {
		FXASSERT(0, "There are no ConfigPath", __FILE__, __FUNCTION__);
        return false;
    }

    if (NULL == pPluginConfigPathNode->first_attribute("Name"))
    {
		FXASSERT(0, "There are no ConfigPath.Name", __FILE__, __FUNCTION__);
        return false;
    }

    mstrConfigPath = pPluginConfigPathNode->first_attribute("Name")->value();

    return true;
}

bool FXCPluginManager::LoadStaticPlugin(const std::string& strPluginDLLName)
{
	//     PluginNameList::iterator it = mPluginNameList.begin();
	//     for (it; it != mPluginNameList.end(); it++)
	//     {
	//         const std::string& strPluginName = *it;
	//         CREATE_PLUGIN( this, strPluginName );
	//     }

	//     CREATE_PLUGIN(this, FXKernelPlugin)
	//     CREATE_PLUGIN(this, FXEventProcessPlugin)
	//     CREATE_PLUGIN(this, FXConfigPlugin)

	return false;
}

void FXCPluginManager::Registered(FXIPlugin* plugin)
{
    std::string strPluginName = plugin->GetPluginName();
    if (!FindPlugin(strPluginName))
    {
		// dynamic add plugin no dlls
        //bool bFind = false;
        //PluginNameMap::iterator it = mPluginNameMap.begin();
        //for (it; it != mPluginNameMap.end(); ++it)
        //{
        //    if (strPluginName == it->first)
        //    {
        //        bFind = true;
        //        break;
        //    }
        //}

        //if (bFind)
        {
            mPluginInstanceMap.insert(PluginInstanceMap::value_type(strPluginName, plugin));
            plugin->Install();
        }
    }
}

void FXCPluginManager::UnRegistered(FXIPlugin* plugin)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(plugin->GetPluginName());
    if (it != mPluginInstanceMap.end())
    {
        it->second->Uninstall();
        delete it->second;
        it->second = NULL;
        mPluginInstanceMap.erase(it);
    }
}

bool FXCPluginManager::ReLoadPlugin(const std::string & strPluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who have been reloaded
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.find(strPluginDLLName);
	if (itInstance == mPluginInstanceMap.end())
	{
		return false;
	}
	//1
	FXIPlugin* pPlugin = itInstance->second;
	FXIModule* pModule = pPlugin->First();
	while (pModule)
	{
		pModule->BeforeShut();
		pModule->Shut();
		pModule->Finalize();

		pModule = pPlugin->Next();
	}

	//2
	PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
	if (it != mPluginLibMap.end())
	{
		FXCDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		mPluginLibMap.erase(it);
	}

	//3
	FXCDynLib* pLib = new FXCDynLib(strPluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

		DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
		if (!pFunc)
		{
			std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
			assert(0);
			return false;
		}

		pFunc(this);
	}
	else
	{
#if FX_PLATFORM == FX_PLATFORM_LINUX
		char* error = dlerror();
		if (error)
		{
			std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
			std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
		}
#elif FX_PLATFORM == FX_PLATFORM_WIN
		std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
		std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
		assert(0);
		return false;
#endif // FX_PLATFORM
	}

	//4
	PluginInstanceMap::iterator itReloadInstance = mPluginInstanceMap.begin();
	for (itReloadInstance; itReloadInstance != mPluginInstanceMap.end(); itReloadInstance++)
	{
		if (strPluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

FXIPlugin* FXCPluginManager::FindPlugin(const std::string& strPluginName)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(strPluginName);
    if (it != mPluginInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool FXCPluginManager::Execute()
{
    mnNowTime = time(NULL);

    bool bRet = true;

    PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
    for (; it != mPluginInstanceMap.end(); ++it)
    {
        bool tempRet = it->second->Execute();
        bRet = bRet && tempRet;
    }

    return bRet;
}

inline int FXCPluginManager::GetAppID() const
{
	return mnAppID;
}

inline void FXCPluginManager::SetAppID(const int nAppID)
{
    mnAppID = nAppID;
}

inline FXINT64 FXCPluginManager::GetInitTime() const
{
	return mnInitTime;
}

inline FXINT64 FXCPluginManager::GetNowTime() const
{
	return mnNowTime;
}

inline const std::string & FXCPluginManager::GetConfigPath() const
{
	return mstrConfigPath;
}

void FXCPluginManager::SetConfigName(const std::string & strFileName)
{
	if (strFileName.empty())
	{
		return;
	}

	if (strFileName.find(".xml") == string::npos)
	{
		return;
	}

	mstrConfigName = strFileName;
}

const std::string& FXCPluginManager::GetAppName() const
{
	return mstrAppName;
}

void FXCPluginManager::SetAppName(const std::string& strAppName)
{
	if (!mstrAppName.empty())
	{
		return;
	}

	mstrAppName = strAppName;
}

const std::string & FXCPluginManager::GetLogConfigName() const
{
	return mstrLogConfigName;
}

void FXCPluginManager::SetLogConfigName(const std::string & strName)
{
	mstrLogConfigName = strName;
}

void FXCPluginManager::SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun)
{
	mGetFileContentFunctor = fun;
}

bool FXCPluginManager::GetFileContent(const std::string &strFileName, std::string &strContent)
{
	if (mGetFileContentFunctor)
	{
		return mGetFileContentFunctor(strFileName, strContent);
	}

    std::ifstream file(strFileName.c_str(), std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);

        if (file.read(buffer.data(), size))
        {
            strContent.assign(buffer.data(),buffer.size());
        }
        file.close();
    }
    else
    {
        std::cout << "GetFileContent open file [" << strFileName << "] failed" << std::endl;
        return false;
    }

	return true;
}

void FXCPluginManager::AddModule(const std::string& strModuleName, FXIModule* pModule)
{
    if (!FindModule(strModuleName))
    {
        mModuleInstanceMap.insert(ModuleInstanceMap::value_type(strModuleName, pModule));
    }
}

void FXCPluginManager::RemoveModule(const std::string& strModuleName)
{
    ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strModuleName);
    if (it != mModuleInstanceMap.end())
    {
        mModuleInstanceMap.erase(it);
    }
}


FXIModule* FXCPluginManager::FindModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if FX_PLATFORM == FX_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(" ");
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if (strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strSubModuleName);
	if (it != mModuleInstanceMap.end())
	{
		return it->second;
	}

    return NULL;
}

bool FXCPluginManager::AfterInit()
{
    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for (itAfterInstance; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
    {
        itAfterInstance->second->AfterInit();
    }

    return true;
}

bool FXCPluginManager::CheckConfig()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->CheckConfig();
    }

    return true;
}

bool FXCPluginManager::ReadyExecute()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->ReadyExecute();
    }

    return true;
}

bool FXCPluginManager::BeforeShut()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for (itBeforeInstance; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++)
    {
        itBeforeInstance->second->BeforeShut();
    }

    return true;
}

bool FXCPluginManager::Shut()
{
    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (itInstance; itInstance != mPluginInstanceMap.end(); ++itInstance)
    {
        itInstance->second->Shut();
    }

    return true;
}

bool FXCPluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		itInstance->second->Finalize();
	}

	////////////////////////////////////////////////

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (it; it != mPluginNameMap.end(); it++)
	{
#ifdef FX_DYNAMIC_PLUGIN
		UnLoadPluginLibrary(it->first);
#else
		UnLoadStaticPlugin(it->first);
#endif
	}

	mPluginInstanceMap.clear();
	mPluginNameMap.clear();

	return true;
}

bool FXCPluginManager::LoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it == mPluginLibMap.end())
    {
        FXCDynLib* pLib = new FXCDynLib(strPluginDLLName);
        bool bLoad = pLib->Load();

        if (bLoad)
        {
            mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

            DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
            if (!pFunc)
            {
                std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
                assert(0);
                return false;
            }

            pFunc(this);

            return true;
        }
        else
        {
#if FX_PLATFORM == FX_PLATFORM_LINUX
            char* error = dlerror();
            if (error)
            {
                std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
                std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
                assert(0);
                return false;
            }
#elif FX_PLATFORM == FX_PLATFORM_WIN
            std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
            std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
            assert(0);
            return false;
#endif // FX_PLATFORM
        }
    }

    return false;
}

bool FXCPluginManager::UnLoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it != mPluginLibMap.end())
    {
        FXCDynLib* pLib = it->second;
        DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

        if (pFunc)
        {
            pFunc(this);
        }

        pLib->UnLoad();

        delete pLib;
        pLib = NULL;
        mPluginLibMap.erase(it);

        return true;
    }

    return false;
}

bool FXCPluginManager::UnLoadStaticPlugin(const std::string & strPluginDLLName)
{
	//     DESTROY_PLUGIN(this, FXKernelPlugin)
	return false;
}
