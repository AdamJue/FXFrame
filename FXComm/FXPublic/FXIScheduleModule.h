#ifndef FXI_SCHEDULE_MODULE_H
#define FXI_SCHEDULE_MODULE_H

#include <string>
#include "FXMap.hpp"
#include "FXList.hpp"
#include "FXDataList.hpp"
#include "FXDateTime.hpp"
#include "FXIModule.h"

typedef std::function<int(const FXGUID&, const std::string&, const float, const int)> OBJECT_SCHEDULE_FUNCTOR;
typedef std::function<int(const std::string&, const float, const int)> MODULE_SCHEDULE_FUNCTOR;

typedef FX_SHARE_PTR<OBJECT_SCHEDULE_FUNCTOR> OBJECT_SCHEDULE_FUNCTOR_PTR;//HEART
typedef FX_SHARE_PTR<MODULE_SCHEDULE_FUNCTOR> MODULE_SCHEDULE_FUNCTOR_PTR;//HEART

class FXIScheduleModule
	:public  FXIModule
{
public:
	virtual ~FXIScheduleModule() {}

	///for module
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const FXDateTime& date) = 0;
	virtual bool RemoveSchedule(const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		MODULE_SCHEDULE_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		MODULE_SCHEDULE_FUNCTOR_PTR functorPtr(FX_NEW MODULE_SCHEDULE_FUNCTOR(functor));
		return AddSchedule(strScheduleName, functorPtr, fIntervalTime, nCount);
	}

	///for object
	virtual bool AddSchedule(const FXGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool AddSchedule(const FXGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const FXDateTime& date) = 0;
	virtual bool RemoveSchedule(const FXGUID self) = 0;
	virtual bool RemoveSchedule(const FXGUID self, const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const FXGUID self, const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const FXGUID self, const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const FXGUID&, const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		OBJECT_SCHEDULE_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		OBJECT_SCHEDULE_FUNCTOR_PTR functorPtr(FX_NEW OBJECT_SCHEDULE_FUNCTOR(functor));
		return AddSchedule(self, strScheduleName, functorPtr, fIntervalTime, nCount);
	}
};

#endif
