
#ifndef FX_MAPEX_H
#define FX_MAPEX_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "FXCConsistentHash.hpp"
#include "FXPlatform.h"

template <typename T , typename TD>
class FXMapEx
{
public:
    typedef std::map<T, FX_SHARE_PTR<TD> > FXMapOBJECT;

    FXMapEx()
	{
	};
    virtual ~FXMapEx()
    {

    };

	virtual bool ExistElement(const T& name)
	{
		typename FXMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr != mObjectList.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
/*
	virtual FX_SHARE_PTR<TD> AddElement(const T& name)
	{
		typename FXMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr == mObjectList.end())
		{
			FX_SHARE_PTR<TD> data(FX_NEW TD());
			mObjectList.insert(typename FXMapOBJECT::value_type(name, data));
			return data;
		}

		return FX_SHARE_PTR<TD>();
	}
	*/
    virtual bool AddElement(const T& name, const FX_SHARE_PTR<TD> data)
    {
		if (data == nullptr)
		{
			return false;
		}

        typename FXMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename FXMapOBJECT::value_type(name, data));

            return true;
        }

        return false;
    }

    virtual bool RemoveElement(const T& name)
    {
        typename FXMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            mObjectList.erase(itr);

            return true;
        }

        return false;
    }

    virtual TD* GetElementNude(const T& name)
    {
        typename FXMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual FX_SHARE_PTR<TD> GetElement(const T& name)
    {
        typename FXMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual TD* FirstNude(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* NextNude(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* FirstNude()
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* NextNude()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual FX_SHARE_PTR<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual FX_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual FX_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual FX_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    int Count()
    {
        return (int)mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }

protected:
    FXMapOBJECT     mObjectList;
    typename FXMapOBJECT::iterator mObjectCurIter;
};

template <typename T, typename TD>
class FXCConsistentHashMapEx : public FXMapEx<T, TD>
{
public:
	virtual void InitHashNodeWeith(const int nWeigh = 500)
	{
		mnWeigh = nWeigh;
	}

	virtual FX_SHARE_PTR<TD> GetElementBySuitRandom()
	{
		FXCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeRandom(vNode))
		{
			typename FXMapEx<T, TD>::FXMapOBJECT::iterator itr = FXMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != FXMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual FX_SHARE_PTR<TD> GetElementBySuitConsistent()
	{
		FXCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeConsistent(vNode))
		{
			typename FXMapEx<T, TD>::FXMapOBJECT::iterator itr = FXMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != FXMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual FX_SHARE_PTR<TD> GetElementBySuit(const T& name)
	{
		FXCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNode(name, vNode))
		{
			typename FXMapEx<T, TD>::FXMapOBJECT::iterator itr = FXMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != FXMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual bool AddElement(const T& name, const FX_SHARE_PTR<TD> data)
	{
		if (data == nullptr)
		{
			return false;
		}

		typename FXMapEx<T, TD>::FXMapOBJECT::iterator itr = FXMapEx<T, TD>::mObjectList.find(name);
		if (itr == FXMapEx<T, TD>::mObjectList.end())
		{
			FXMapEx<T, TD>::mObjectList.insert(typename FXMapEx<T, TD>::FXMapOBJECT::value_type(name, data));

			mxConsistentHash.Insert(name);

			return true;
		}

		return false;
	}

	virtual bool RemoveElement(const T& name)
	{
		typename FXMapEx<T, TD>::FXMapOBJECT::iterator itr = FXMapEx<T, TD>::mObjectList.find(name);
		if (itr != FXMapEx<T, TD>::mObjectList.end())
		{
			FXMapEx<T, TD>::mObjectList.erase(itr);
			mxConsistentHash.Erase(name);

			return true;
		}

		return false;
	}

	bool ClearAll()
	{
		FXMapEx<T, TD>::mObjectList.clear();
		mxConsistentHash.ClearAll();
		return true;
	}

private:
	int mnWeigh = 0;
	FXCConsistentHash<T> mxConsistentHash;
};
#endif
