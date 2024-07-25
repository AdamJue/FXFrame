
#include "FXCEventModule.h"

bool FXCEventModule::Init()
{

    return true;
}

bool FXCEventModule::AfterInit()
{
    return true;
}

bool FXCEventModule::BeforeShut()
{
	mModuleEventInfoMapEx.ClearAll();
	mObjectEventInfoMapEx.ClearAll();

    return true;
}

bool FXCEventModule::Shut()
{
    return true;
}

bool FXCEventModule::Execute()
{
	//remove
	if (mModuleRemoveListEx.Count() > 0)
	{
		FXEventDefine nEventID = FXEventDefine::FXED_UNKNOW;;
		bool bRet = mModuleRemoveListEx.First(nEventID);
		while (bRet)
		{
			mModuleEventInfoMapEx.RemoveElement(nEventID);

			bRet = mModuleRemoveListEx.Next(nEventID);
		}
	}

	// remove object
	if (mObjectRemoveListEx.Count() > 0)
	{
		FXGUID xObject;
		bool bRet = mObjectRemoveListEx.First(xObject);
		while (bRet)
		{
			mObjectEventInfoMapEx.RemoveElement(xObject);

			bRet = mObjectRemoveListEx.Next(xObject);
		}
	}

    // execute async event
    if (mModuleWaitListEx.Count() > 0)
    {
        // first we do all async event, if event to many event, we will do it in next frame
        EVENT_CACHE xEventCache;
        bool bRet = mModuleWaitListEx.First(xEventCache);
        while (bRet)
        {
            this->DoEvent(xEventCache.first, xEventCache.second);
            bRet = mModuleWaitListEx.Next(xEventCache);
        }

        mModuleWaitListEx.ClearAll();
    }

    if (mObjectWaitListEx.Count() > 0)
    {
        FXGUID xObject;
        auto xEventListPtr = mObjectWaitListEx.First(xObject);
        while (xEventListPtr != nullptr)
        {
            EVENT_CACHE xEventCache;
            bool bRet = xEventListPtr->First(xEventCache);
            while (bRet)
            {
                this->DoEvent(xObject,xEventCache.first, xEventCache.second);
                bRet = xEventListPtr->Next(xEventCache);
            }
            xEventListPtr->ClearAll();
            xEventListPtr = mObjectWaitListEx.Next(xObject);
        }
        mObjectWaitListEx.ClearAll();
    }

    return true;

}

bool FXCEventModule::DoEvent(const FXEventDefine nEventID, const FXDataList & valueList)
{
	bool bRet = false;

	FX_SHARE_PTR<FXList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			MODULE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool FXCEventModule::DoAsynEvent(const FXEventDefine nEventID, const FXDataList& valueList)
{
    mModuleWaitListEx.Add(EVENT_CACHE(nEventID, valueList));

    return true;
}

bool FXCEventModule::ExistEventCallBack(const FXEventDefine nEventID)
{
	return mModuleEventInfoMapEx.ExistElement(nEventID);
}

bool FXCEventModule::RemoveEventCallBack(const FXEventDefine nEventID)
{
	return mModuleEventInfoMapEx.RemoveElement(nEventID);
}

bool FXCEventModule::DoEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList & valueList)
{
	bool bRet = false;

	//if (!m_pKernelodule->ExistObject(self))
	//{
	//	return bRet;
	//}

	FX_SHARE_PTR<FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return bRet;
	}

	FX_SHARE_PTR<FXList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr = xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		return bRet;
	}

	OBJECT_EVENT_FUNCTOR_PTR pFunPtr;
	bool bFunRet = xEventListPtr->First(pFunPtr);
	while (bFunRet)
	{
		OBJECT_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nEventID, valueList);

		bFunRet = xEventListPtr->Next(pFunPtr);
	}

	return bRet;
}

bool FXCEventModule::DoAsynEvent(const FXGUID self, const FXEventDefine nEventID, const FXDataList& valueList)
{
    if (!mObjectWaitListEx.ExistElement(self))
    {
        auto eventCacheList = std::make_shared<FXList<EVENT_CACHE>>();
        eventCacheList->Add(EVENT_CACHE(nEventID, valueList));

        mObjectWaitListEx.AddElement(self, eventCacheList);
    }
    else
    {
        mObjectWaitListEx.GetElement(self)->Add(EVENT_CACHE(nEventID, valueList));
    }

    return true;
}

bool FXCEventModule::ExistEventCallBack(const FXGUID self, const FXEventDefine nEventID)
{
	FX_SHARE_PTR<FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->ExistElement(nEventID);
}

bool FXCEventModule::RemoveEventCallBack(const FXGUID self, const FXEventDefine nEventID)
{
	FX_SHARE_PTR<FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->RemoveElement(nEventID);
}

bool FXCEventModule::RemoveEventCallBack(const FXGUID self)
{
	return mObjectEventInfoMapEx.RemoveElement(self);
}

bool FXCEventModule::AddEventCallBack(const FXEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb)
{
	FX_SHARE_PTR<FXList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = FX_SHARE_PTR<FXList<MODULE_EVENT_FUNCTOR_PTR>>(FX_NEW FXList<MODULE_EVENT_FUNCTOR_PTR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool FXCEventModule::AddEventCallBack(const FXGUID self, const FXEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb)
{
	//if (!m_pKernelodule->ExistObject(self))
	//{
	//	return false;
	//}
	//
	FX_SHARE_PTR<FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		xEventMapPtr = FX_SHARE_PTR<FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>>(FX_NEW FXMapEx<FXEventDefine, FXList<OBJECT_EVENT_FUNCTOR_PTR>>());
		mObjectEventInfoMapEx.AddElement(self, xEventMapPtr);
	}

	FX_SHARE_PTR<FXList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr =  xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = FX_SHARE_PTR<FXList<OBJECT_EVENT_FUNCTOR_PTR>>(FX_NEW FXList<OBJECT_EVENT_FUNCTOR_PTR>());
		xEventMapPtr->AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return true;
}
