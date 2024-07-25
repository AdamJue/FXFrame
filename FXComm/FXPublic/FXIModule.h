
#ifndef FXI_LOGIC_MODULE_H
#define FXI_LOGIC_MODULE_H

#include <string>
#include "FXMap.hpp"
#include "FXList.hpp"
#include "FXGUID.h"


template<typename DerivedType, typename BaseType>
class TIsDerived
{
public:
    static int AnyFunction(BaseType* base)
    {
        return 1;
    }

    static  char AnyFunction(void* t2)
    {
        return 0;
    }

    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)NULL))),
    };
};

class FXIPluginManager;

class FXIModule
{

public:
    FXIModule()
    {
    }

    virtual ~FXIModule() {}

    virtual bool Awake()
    {
        return true;
    }

    virtual bool Init()
    {

        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool ReadyExecute()
    {
        return true;
    }

    virtual bool Execute()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Finalize()
    {
        return true;
    }

	virtual bool OnReloadPlugin()
	{
		return true;
	}

    virtual FXIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

    std::string strName;

protected:
	FXIPluginManager* pPluginManager = NULL;
};
#endif
