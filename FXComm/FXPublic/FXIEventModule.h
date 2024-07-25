
#ifndef FXI_EVENT_MODULE_H
#define FXI_EVENT_MODULE_H

#include <iostream>
#include "FXIModule.h"
#include "FXDataList.hpp"
#include "FXEventDefine.h"

class FXIEventModule
    : public FXIModule
{
public:
protected:

	typedef std::function<int(const FXGUID&, const FXEventDefine, const FXDataList&)> OBJECT_EVENT_FUNCTOR;
	typedef std::function<int(const FXEventDefine, const FXDataList&)> MODULE_EVENT_FUNCTOR;

	typedef FX_SHARE_PTR<OBJECT_EVENT_FUNCTOR> OBJECT_EVENT_FUNCTOR_PTR;//EVENT
	typedef FX_SHARE_PTR<MODULE_EVENT_FUNCTOR> MODULE_EVENT_FUNCTOR_PTR;//EVENT

public:
	// only be used in module
    virtual bool DoEvent(const FXEventDefine nEventID, const FXDataList& valueList) = 0;
    virtual bool DoAsynEvent(const FXEventDefine nEventID, const FXDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const FXEventDefine nEventID) = 0;

    virtual bool RemoveEventCallBack(const FXEventDefine nEventID) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const FXEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const FXEventDefine, const FXDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		MODULE_EVENT_FUNCTOR_PTR functorPtr(new MODULE_EVENT_FUNCTOR(functor));
		return AddEventCallBack(nEventID, functorPtr);
	}

    // TODO add module event call back with Priority
//    virtual bool AddEventCallBack(const FXEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb, int nPriority = 0) = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
	// can be used for object
    virtual bool DoEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList& valueList) = 0;
    virtual bool DoAsynEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const FXGUID self,const FXEventDefine nEventID) = 0;

    virtual bool RemoveEventCallBack(const FXGUID self,const FXEventDefine nEventID) = 0;
	virtual bool RemoveEventCallBack(const FXGUID self) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const FXGUID& self, const FXEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const FXGUID&, const FXEventDefine, const FXDataList&))
	{
		OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		OBJECT_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_EVENT_FUNCTOR(functor));
		return AddEventCallBack(self, nEventID, functorPtr);
	}

    // TODO add object event call back with Priority
//    virtual bool AddEventCallBack(const FXGUID self, const FXEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb, int nPriority = 0) = 0;

protected:

	virtual bool AddEventCallBack(const FXEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb) = 0;
	virtual bool AddEventCallBack(const FXGUID self, const FXEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb) = 0;

};

#endif
