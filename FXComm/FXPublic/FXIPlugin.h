
#ifndef FXI_PLUGIN_H
#define FXI_PLUGIN_H

#include <iostream>
#include <assert.h>
#include "FXMap.hpp"
#include "FXIModule.h"
#include "FXPlatform.h"
#include "FXIPluginManager.h"

#define REGISTER_MODULE(pManager, classBaseName, className)  \
	assert((TIsDerived<classBaseName, FXIModule>::Result));	\
	assert((TIsDerived<className, classBaseName>::Result));	\
	FXIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );AddElement( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) FXIModule* pUnRegisterModule##className =  \
	dynamic_cast<FXIModule*>( pManager->FindModule( #classBaseName )); \
	pManager->RemoveModule( #classBaseName ); RemoveElement( #classBaseName ); delete pUnRegisterModule##className;


#define CREATE_PLUGIN(pManager, className)  FXIPlugin* pCreatePlugin##className = new className(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnRegistered( pManager->FindPlugin((#className)) );

/*
#define REGISTER_COMPONENT(pManager, className)  FXIComponent* pRegisterComponent##className= new className(pManager); \
    pRegisterComponent##className->strName = (#className); \
    pManager->AddComponent( (#className), pRegisterComponent##className );

#define UNREGISTER_COMPONENT(pManager, className) FXIComponent* pRegisterComponent##className =  \
        dynamic_cast<FXIComponent*>( pManager->FindComponent( (#className) ) ); pManager->RemoveComponent( (#className) ); delete pRegisterComponent##className;
*/

class FXIPluginManager;

class FXIPlugin : public FXIModule,
    public FXMap<std::string, FXIModule>
{

public:
	FXIPlugin()
	{
	}

    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

	virtual bool Awake()
	{
		FXIModule* pModule = First();
		while (pModule)
		{
			bool bRet = pModule->Awake();
			if (!bRet)
			{
				std::cout << pModule->strName << std::endl;
				assert(0);
			}

			pModule = Next();
		}
		return true;
	}

    virtual bool Init()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->Init();
            if (!bRet)
            {
				std::cout << pModule->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
				std::cout << pModule ->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool CheckConfig()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            pModule->CheckConfig();

            pModule = Next();
        }

        return true;
    }

	virtual bool ReadyExecute()
	{
		FXIModule* pModule = First();
		while (pModule)
		{
			pModule->ReadyExecute();

			pModule = Next();
		}

		return true;
	}

    virtual bool Execute()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            pModule->Execute();

            pModule = Next();
        }

        return true;
    }

    virtual bool BeforeShut()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            pModule->BeforeShut();

            pModule = Next();
        }
        return true;
    }

    virtual bool Shut()
    {
        FXIModule* pModule = First();
        while (pModule)
        {
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

	virtual bool OnReloadPlugin()
	{
		FXIModule* pModule = First();
		while (pModule)
		{
			pModule->OnReloadPlugin();

			pModule = Next();
		}

		return true;
	}
    virtual void Uninstall() = 0;
};

#endif
