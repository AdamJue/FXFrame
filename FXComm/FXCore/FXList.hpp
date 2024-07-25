

#ifndef FX_LIST_H
#define FX_LIST_H

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include "FXPlatform.h"

template < typename T >
class FXList
{
public:
    virtual ~FXList()
    {
    }
    bool                    Add(const T& id);
    bool                    Find(const T& id);
    bool                    Remove(const T& id);
    bool                    ClearAll();

    bool                    First(T& id);
    bool                    Next(T& id);
    bool                    Get(const int32_t index, T& id);
    int                     Count();

protected:
    //idType normal;
    typedef     std::list<T>   TLISTOBJConfigLIST;
    TLISTOBJConfigLIST          mtObjConfigList;
    typename std::list<T>::iterator mCurIter;

private:
};

template < typename T >
bool FXList<T>::Add(const T& id)
{
    //if (!Find(id))
    {
        mtObjConfigList.push_back(id);
        return true;
    }

    return false;
}

template < typename T >
bool FXList<T>::Remove(const T& id)
{
    if (Find(id))
    {
        mtObjConfigList.remove(id);
        return true;
    }

    return false;
}

template < typename T >
bool FXList<T>::ClearAll()
{
    mtObjConfigList.clear();
    return true;
}

template < typename T >
bool FXList<T>::First(T& id)
{
    if (mtObjConfigList.size() <= 0)
    {
        return false;
    }

    mCurIter = mtObjConfigList.begin();
    if (mCurIter != mtObjConfigList.end())
    {
        id = *mCurIter;
        return true;
    }

    return false;
}

template < typename T >
bool FXList<T>::Next(T& id)
{
    if (mCurIter == mtObjConfigList.end())
    {
        return false;
    }

    ++mCurIter;
    if (mCurIter != mtObjConfigList.end())
    {
        id = *mCurIter;
        return true;
    }

    return false;
}

template < typename T >
bool FXList<T>::Find(const T& id)
{
    typename TLISTOBJConfigLIST::iterator it = std::find(mtObjConfigList.begin(), mtObjConfigList.end(), id);
    if (it != mtObjConfigList.end())
    {
        return true;
    }

    return false;
}

template < typename T >
bool FXList<T>::Get(const int32_t index, T& id)
{
    if (index >= mtObjConfigList.size())
    {
        return false;
    }

    typename std::list<T>::iterator it = this->mtObjConfigList.begin();
    std::advance(it, index);

    id = *it;

    return true;
}

template < typename T >
int FXList<T>::Count()
{
    return (int)(mtObjConfigList.size());
}

#endif
