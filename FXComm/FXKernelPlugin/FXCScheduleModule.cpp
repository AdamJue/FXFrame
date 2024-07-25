
#include "FXCScheduleModule.h"

void FXCScheduleElement::DoHeartBeatEvent()
{
	if (self.IsNull())
	{
		MODULE_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxModuleFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxModuleFunctor.Next(cb);
		}
	}
	else
	{
		OBJECT_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxObjectFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxObjectFunctor.Next(cb);
		}
	}
}

FXCScheduleModule::FXCScheduleModule(FXIPluginManager* p)
{
	pPluginManager = p;
}

FXCScheduleModule::~FXCScheduleModule()
{
	mObjectScheduleMap.ClearAll();
    mModuleScheduleMap.ClearAll();
}

bool FXCScheduleModule::Init()
{
	return true;
}
bool FXCScheduleModule::Execute()
{
	//execute every schedule
	FX_SHARE_PTR<FXMapEx <std::string, FXCScheduleElement >> xObjectSchedule = mObjectScheduleMap.First();
	while (xObjectSchedule)
	{
		std::string str;
		FX_SHARE_PTR<FXCScheduleElement> pSchedule = xObjectSchedule->First();
		while (pSchedule)
		{
			FXINT64 nNow = FXGetTime();
			if (nNow > pSchedule->mnNextTriggerTime)
			{
				std::map<FXGUID, std::string>::iterator itRet = mObjectRemoveList.find(pSchedule->self);
				if (itRet != mObjectRemoveList.end() && itRet->second == pSchedule->mstrScheduleName)continue;

				if (pSchedule->mnRemainCount > 0 || pSchedule->mbForever == true)
				{
					pSchedule->mnRemainCount--;
					pSchedule->DoHeartBeatEvent();

					if (pSchedule->mnRemainCount <= 0 && pSchedule->mbForever == false)
					{
						mObjectRemoveList.insert(std::map<FXGUID, std::string>::value_type(pSchedule->self, pSchedule->mstrScheduleName));
//                        cout << "pSchedule->mstrScheduleName : " << pSchedule->mstrScheduleName << endl;
					}
					else
					{
						FXINT64 nNextCostTime = FXINT64(pSchedule->mfIntervalTime * 1000) * (pSchedule->mnAllCount - pSchedule->mnRemainCount + 1);
						pSchedule->mnNextTriggerTime = pSchedule->mnStartTime + nNextCostTime;
//						cout << " Next Time:" << pSchedule->mnNextTriggerTime << " Start Time:" << pSchedule->mnStartTime << " Next Cost Time:" << nNextCostTime << " Now Time:" << nNow << endl;
					}
				}

			}

			pSchedule = xObjectSchedule->Next();
		}

		xObjectSchedule = mObjectScheduleMap.Next();
	}

	//remove schedule
	for (std::map<FXGUID, std::string>::iterator it = mObjectRemoveList.begin(); it != mObjectRemoveList.end(); ++it)
	{
		FXGUID self = it->first;
		std::string scheduleName = it->second;
		auto findIter = mObjectScheduleMap.GetElement(self);
		if (NULL != findIter)
		{
			findIter->RemoveElement(scheduleName);
			if (findIter->Count() == 0)
			{
				mObjectScheduleMap.RemoveElement(self);
			}
		}
	}
	mObjectRemoveList.clear();

	//add schedule
	for (std::list<FXCScheduleElement>::iterator iter = mObjectAddList.begin(); iter != mObjectAddList.end(); ++iter)
	{
		FX_SHARE_PTR< FXMapEx <std::string, FXCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(iter->self);
		if (NULL == xObjectScheduleMap)
		{
			xObjectScheduleMap = FX_SHARE_PTR< FXMapEx <std::string, FXCScheduleElement >>(FX_NEW FXMapEx <std::string, FXCScheduleElement >());
			mObjectScheduleMap.AddElement(iter->self, xObjectScheduleMap);
		}

		FX_SHARE_PTR<FXCScheduleElement> xScheduleElement = xObjectScheduleMap->GetElement(iter->mstrScheduleName);
		if (NULL == xScheduleElement)
		{
			xScheduleElement = FX_SHARE_PTR<FXCScheduleElement>(FX_NEW FXCScheduleElement());
			*xScheduleElement = *iter;

			xObjectScheduleMap->AddElement(iter->mstrScheduleName, xScheduleElement);
		}
	}

	mObjectAddList.clear();

	////////////////////////////////////////////
	//execute every schedule
	FX_SHARE_PTR< FXCScheduleElement > xModuleSchedule = mModuleScheduleMap.First();
	while (xModuleSchedule)
	{
		FXINT64 nNow = FXGetTime();
		if (nNow > xModuleSchedule->mnNextTriggerTime)
		{
			if (xModuleSchedule->mnRemainCount > 0 || xModuleSchedule->mbForever == true)
			{
				xModuleSchedule->mnRemainCount--;
				xModuleSchedule->DoHeartBeatEvent();

				if (xModuleSchedule->mnRemainCount <= 0 && xModuleSchedule->mbForever == false)
				{
					mModuleRemoveList.push_back(xModuleSchedule->mstrScheduleName);
				}
				else
				{
					FXINT64 nNextCostTime = FXINT64(xModuleSchedule->mfIntervalTime * 1000) * (xModuleSchedule->mnAllCount - xModuleSchedule->mnRemainCount + 1);
					xModuleSchedule->mnNextTriggerTime = xModuleSchedule->mnStartTime + nNextCostTime;
				}
			}
		}

		xModuleSchedule = mModuleScheduleMap.Next();
	}

	//remove schedule
	for (std::list<std::string>::iterator it = mModuleRemoveList.begin(); it != mModuleRemoveList.end(); ++it)
	{
		const std::string& strSheduleName = *it;;
		auto findIter = mModuleScheduleMap.GetElement(strSheduleName);
		if (NULL != findIter)
		{
			mModuleScheduleMap.RemoveElement(strSheduleName);
		}
	}
	mModuleRemoveList.clear();

	//add schedule
	for (std::list<FXCScheduleElement>::iterator iter = mModuleAddList.begin(); iter != mModuleAddList.end(); ++iter)
	{
		FX_SHARE_PTR< FXCScheduleElement > xModuleScheduleMap = mModuleScheduleMap.GetElement(iter->mstrScheduleName);
		if (NULL == xModuleScheduleMap)
		{
			xModuleScheduleMap = FX_SHARE_PTR< FXCScheduleElement >(FX_NEW FXCScheduleElement());
			mModuleScheduleMap.AddElement(iter->mstrScheduleName, xModuleScheduleMap);
		}

		*xModuleScheduleMap = *iter;
	}

	mModuleAddList.clear();
	return true;
}

bool FXCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const float fTime, const int nCount)
{
	FXCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = FXGetTime() + (FXINT64)(fTime * 1000);
	xSchedule.mnStartTime = FXGetTime();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = FXGUID();
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxModuleFunctor.Add(cb);

	mModuleAddList.push_back(xSchedule);

	return true;
}

bool FXCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const FXDateTime & date)
{
	return false;
}

bool FXCScheduleModule::RemoveSchedule(const std::string & strScheduleName)
{
	mModuleRemoveList.push_back(strScheduleName);

	return true;
}

bool FXCScheduleModule::ExistSchedule(const std::string & strScheduleName)
{
	return mModuleScheduleMap.ExistElement(strScheduleName);
}

bool FXCScheduleModule::AddSchedule(const FXGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	FXCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = FXGetTime() + (FXINT64)(fTime * 1000);
	xSchedule.mnStartTime = FXGetTime();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = self;
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxObjectFunctor.Add(cb);

	mObjectAddList.push_back(xSchedule);

	return true;
}

bool FXCScheduleModule::AddSchedule(const FXGUID self, const std::string & strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const FXDateTime & date)
{
	return false;
}

bool FXCScheduleModule::RemoveSchedule(const FXGUID self)
{
	return mObjectScheduleMap.RemoveElement(self);
}

bool FXCScheduleModule::RemoveSchedule(const FXGUID self, const std::string& strScheduleName)
{
	mObjectRemoveList.insert(std::map<FXGUID, std::string>::value_type(self, strScheduleName));
	return true;
}

bool FXCScheduleModule::ExistSchedule(const FXGUID self, const std::string& strScheduleName)
{
	FX_SHARE_PTR< FXMapEx <std::string, FXCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return false;
	}

	return xObjectScheduleMap->ExistElement(strScheduleName);
}
