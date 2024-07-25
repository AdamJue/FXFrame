
#ifndef FXC_EVENT_MODULE_H
#define FXC_EVENT_MODULE_H

#include <iostream>
#include "FXGUID.h"
#include "FXIEventModule.h"

class FXCEventModule
    : public FXIEventModule
{
public:
    FXCEventModule(FXIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~FXCEventModule()
    {
    }

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool DoEvent(const FXEventDefine nEventID, const FXDataList& valueList);
    virtual bool DoAsynEvent(const FXEventDefine nEventID, const FXDataList& valueList);

    virtual bool ExistEventCallBack(const FXEventDefine nEventID);

	virtual bool RemoveEventCallBack(const FXEventDefine nEventID);

	//////////////////////////////////////////////////////////
	virtual bool DoEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList& valueList);
	virtual bool DoAsynEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList& valueList);

	virtual bool ExistEventCallBack(const FXGUID self, const FXEventDefine nEventID);

	virtual bool RemoveEventCallBack(const FXGUID self, const FXEventDefine nEventID);
	virtual bool RemoveEventCallBack(const FXGUID self);

protected:

	virtual bool AddEventCallBack(const FXEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);
	virtual bool AddEventCallBack(const FXGUID self, const FXEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb);

private:


private:
	// for module
    typedef std::pair<FXEventDefine, FXDataList> EVENT_CACHE;

    FXList<EVENT_CACHE> mModuleWaitListEx;
	FXList<FXEventDefine> mModuleRemoveListEx;
	FXMapEx<FXEventDefine, FXList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;

	//for object
    FXMapEx<FXGUID,FXList<EVENT_CACHE> > mObjectWaitListEx;
	FXList<FXGUID> mObjectRemoveListEx;
	FXMapEx<FXGUID, FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>> mObjectEventInfoMapEx;
};

#endif
