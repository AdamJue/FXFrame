
#ifndef FXC_PLUGIN_MANAGER_H
#define FXC_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include "FXCDynLib.h"
#include "FXSingleton.hpp"
#include "FXIPluginManager.h"

class FXCPluginManager
    : public FXIPluginManager,
	public FXSingleton<FXCPluginManager>
{
public:
    FXCPluginManager();
    virtual ~FXCPluginManager();

	virtual bool Awake();

	virtual bool Init();

    virtual bool AfterInit();

    virtual bool CheckConfig();

    virtual bool ReadyExecute();

    virtual bool BeforeShut();

	virtual bool Shut();

	virtual bool Finalize();


    //////////////////////////////////////////////////////////////////////////

    virtual void Registered(FXIPlugin* pPlugin);

    virtual void UnRegistered(FXIPlugin* pPlugin);

    //////////////////////////////////////////////////////////////////////////

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName);

    virtual FXIPlugin* FindPlugin(const std::string& strPluginName);

    virtual void AddModule(const std::string& strModuleName, FXIModule* pModule);

    virtual void RemoveModule(const std::string& strModuleName);

    virtual FXIModule* FindModule(const std::string& strModuleName);

    virtual bool Execute();

	virtual int GetAppID() const;

    virtual void SetAppID(const int nAppID);

	virtual FXINT64 GetInitTime() const;

	virtual FXINT64 GetNowTime() const;

	virtual const std::string& GetConfigPath() const;

	virtual void SetConfigName(const std::string& strFileName);

	virtual const std::string& GetAppName() const;

	virtual void SetAppName(const std::string& strAppName);

	virtual const std::string& GetLogConfigName() const;

	virtual void SetLogConfigName(const std::string& strName);

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun);

	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent);

protected:
	bool LoadPluginConfig();

	bool LoadStaticPlugin(const std::string& strPluginDLLName);
    bool LoadPluginLibrary(const std::string& strPluginDLLName);
    bool UnLoadPluginLibrary(const std::string& strPluginDLLName);
	bool UnLoadStaticPlugin(const std::string& strPluginDLLName);

private:
    int mnAppID;
    FXINT64 mnInitTime;
    FXINT64 mnNowTime;
    std::string mstrConfigPath;
	std::string mstrConfigName;
	std::string mstrAppName;
	std::string mstrLogConfigName;

    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, FXCDynLib*> PluginLibMap;
    typedef std::map<std::string, FXIPlugin*> PluginInstanceMap;
    typedef std::map<std::string, FXIModule*> ModuleInstanceMap;

    typedef void(* DLL_START_PLUGIN_FUNC)(FXIPluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(FXIPluginManager* pm);

    PluginNameMap mPluginNameMap;
    PluginLibMap mPluginLibMap;
    PluginInstanceMap mPluginInstanceMap;
    ModuleInstanceMap mModuleInstanceMap;

	GET_FILECONTENT_FUNCTOR mGetFileContentFunctor;
};

#endif
