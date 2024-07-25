
#ifndef FXC_SCHEDULE_MODULE_H
#define FXC_SCHEDULE_MODULE_H

#include "FXMap.hpp"
#include "FXList.hpp"
#include "FXDataList.hpp"
#include "FXDateTime.hpp"
#include "FXIScheduleModule.h"

class  FXCScheduleElement
{
public:
	FXCScheduleElement()
	{
		mstrScheduleName = "";
		mfIntervalTime = 0.0f;
		mnNextTriggerTime = 0;
		mnStartTime = 0;
		mnRemainCount = 0;
		mnAllCount = 0;
		mbForever = false;
	};

	virtual ~FXCScheduleElement()
	{
	}

	void DoHeartBeatEvent();

	std::string mstrScheduleName;
	float mfIntervalTime;
	FXINT64 mnNextTriggerTime;
	FXINT64 mnStartTime;
	int mnRemainCount;
	int mnAllCount;
	bool mbForever;

	FXGUID self;

	FXList<OBJECT_SCHEDULE_FUNCTOR_PTR> mxObjectFunctor;
	FXList<MODULE_SCHEDULE_FUNCTOR_PTR> mxModuleFunctor;
};

class FXCScheduleModule : public FXIScheduleModule
{
public:
	FXCScheduleModule(FXIPluginManager* p);

	virtual ~FXCScheduleModule();

	virtual bool Init();
	virtual bool Execute();

	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const FXDateTime& date);
	virtual bool RemoveSchedule(const std::string& strScheduleName);
	virtual bool ExistSchedule(const std::string& strScheduleName);


	virtual bool AddSchedule(const FXGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const FXGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const FXDateTime& date);
	virtual bool RemoveSchedule(const FXGUID self);
	virtual bool RemoveSchedule(const FXGUID self, const std::string& strScheduleName);
	virtual bool ExistSchedule(const FXGUID self, const std::string& strScheduleName);



protected:
	FXMapEx<FXGUID, FXMapEx <std::string, FXCScheduleElement >> mObjectScheduleMap;//guid_scheduleName_element
	std::list<FXCScheduleElement> mObjectAddList;
	std::map<FXGUID, std::string> mObjectRemoveList;

	FXMapEx <std::string, FXCScheduleElement > mModuleScheduleMap;//guid_scheduleName_element
	std::list<FXCScheduleElement> mModuleAddList;
	std::list<std::string> mModuleRemoveList;

};

#endif
