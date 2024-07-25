
#ifndef FXI_PLUGIN_MANAGER_H
#define FXI_PLUGIN_MANAGER_H

#include "FXIModule.h"
#include <assert.h>

typedef std::function<bool (const std::string &strFileName, std::string &strContent)> GET_FILECONTENT_FUNCTOR;

#define FIND_MODULE(classBaseName, className)  \
	assert((TIsDerived<classBaseName, FXIModule>::Result));

class FXIPlugin;

class FXIPluginManager : public FXIModule
{
public:
    FXIPluginManager()
    {

    }

	template <typename T>
	T* FindModule()
	{
		FXIModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, FXIModule>::Result)
			{
				return NULL;
			}

			T* pT = dynamic_cast<T*>(pLogicModule);
			assert(NULL != pT);

			return pT;
		}
		assert(NULL);
		return NULL;
	}

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName) = 0;

    virtual void Registered(FXIPlugin* plugin) = 0;

    virtual void UnRegistered(FXIPlugin* plugin) = 0;

    virtual FXIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, FXIModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual FXIModule* FindModule(const std::string& strModuleName) = 0;

    virtual int GetAppID() const = 0;
    virtual void SetAppID(const int nAppID) = 0;

    virtual FXINT64 GetInitTime() const = 0;
    virtual FXINT64 GetNowTime() const = 0;

	virtual const std::string& GetConfigPath() const = 0;
	virtual void SetConfigName(const std::string& strFileName) = 0;

	virtual const std::string& GetAppName() const = 0;
	virtual void SetAppName(const std::string& strAppName) = 0;

	virtual const std::string& GetLogConfigName() const = 0;
	virtual void SetLogConfigName(const std::string& strName) = 0;

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun) = 0;
	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent) = 0;
};

#endif
