#ifndef FXC_CONSISTENT_HASH_H
#define FXC_CONSISTENT_HASH_H

#include <map>
#include <string>
#include <list>
#include <functional>
#include <algorithm>
#include <chrono>
#include "FXPlatform.h"
#include "crc32.hpp"

//template <typename T>
class FXIVirtualNode
{
public:

    FXIVirtualNode(const int nVirID)
        :nVirtualIndex(nVirID)
    {

    }

	FXIVirtualNode()
	{
		nVirtualIndex = 0;
	}

	virtual ~FXIVirtualNode()
	{
		nVirtualIndex = 0;
	}

	virtual std::string GetDataStr() const
	{
		return "";
	}

    std::string ToStr() const
    {
        std::ostringstream strInfo;
        strInfo << GetDataStr() << "-" << nVirtualIndex;
        return strInfo.str();
    }

private:
    int nVirtualIndex;
};

template <typename T>
class FXCVirtualNode : public FXIVirtualNode
{
public:
	FXCVirtualNode(const T tData, const int nVirID ) : FXIVirtualNode(nVirID)
	{
		mxData = tData;
	}
	FXCVirtualNode()
	{
	}

	virtual std::string GetDataStr() const
	{
		return lexical_cast<std::string>(mxData);
	}

	T mxData;
};

class FXIHasher
{
public:
    virtual uint32_t GetHashValue(const FXIVirtualNode& vNode) = 0;
};

class FXCHasher : public FXIHasher
{
public:
    virtual uint32_t GetHashValue(const FXIVirtualNode& vNode)
    {
        std::string vnode = vNode.ToStr();
        return FFrame::CRC32(vnode);
    }
};

template <typename T>
class FXIConsistentHash
{
public:
	virtual std::size_t Size() const = 0;
	virtual bool Empty() const = 0;

	virtual void ClearAll() = 0;
	virtual void Insert(const T& name) = 0;
	virtual void Insert(const FXCVirtualNode<T>& xNode) = 0;

	virtual bool Exist(const FXCVirtualNode<T>& xInNode) = 0;
	virtual void Erase(const T& name) = 0;
	virtual std::size_t Erase(const FXCVirtualNode<T>& xNode)  = 0;

	virtual bool GetSuitNodeRandom(FXCVirtualNode<T>& node) = 0;
	virtual bool GetSuitNodeConsistent(FXCVirtualNode<T>& node) = 0;
	virtual bool GetSuitNode(const T& name, FXCVirtualNode<T>& node) = 0;
	//virtual bool GetSuitNode(const std::string& str, FXCVirtualNode<T>& node) = 0;
	virtual bool GetSuitNode(uint32_t hashValue, FXCVirtualNode<T>& node) = 0;

	virtual bool GetNodeList(std::list<FXCVirtualNode<T>>& nodeList) = 0;
};

template <typename T>
class FXCConsistentHash : public FXIConsistentHash<T>
{
public:
    FXCConsistentHash()
    {
        m_pHasher = new FXCHasher();
    }

    virtual ~FXCConsistentHash()
    {
        delete m_pHasher;
        m_pHasher = NULL;
    }

public:
	virtual std::size_t Size() const
    {
        return mxNodes.size();
    }

	virtual bool Empty() const
    {
        return mxNodes.empty();
    }

	virtual void ClearAll()
	{
		mxNodes.clear();
	}

	virtual void Insert(const T& name)
	{
		for (int i = 0; i < mnNodeCount; ++i)
		{
			FXCVirtualNode<T> vNode(name, i);
			Insert(vNode);
		}
	}

	virtual void Insert(const FXCVirtualNode<T>& xNode)
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        auto it = mxNodes.find(hash);
        if (it == mxNodes.end())
        {
            mxNodes.insert(typename std::map<uint32_t, FXCVirtualNode<T>>::value_type(hash, xNode));
        }
    }

	virtual bool Exist(const FXCVirtualNode<T>& xInNode)
	{
		uint32_t hash = m_pHasher->GetHashValue(xInNode);
		typename std::map<uint32_t, FXCVirtualNode<T>>::iterator it = mxNodes.find(hash);
		if (it != mxNodes.end())
		{
			return true;
		}

		return false;
	}

	virtual void Erase(const T& name)
	{
		for (int i = 0; i < mnNodeCount; ++i)
		{
			FXCVirtualNode<T> vNode(name, i);
			Erase(vNode);
		}
	}

	virtual std::size_t Erase(const FXCVirtualNode<T>& xNode)
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        return mxNodes.erase(hash);
    }

	virtual bool GetSuitNodeRandom(FXCVirtualNode<T>& node)
	{
		int nID = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return GetSuitNode(nID, node);
	}

	virtual bool GetSuitNodeConsistent(FXCVirtualNode<T>& node)
	{
		return GetSuitNode(0, node);
	}

	virtual bool GetSuitNode(const T& name, FXCVirtualNode<T>& node)
	{
		std::string str = lexical_cast<std::string>(name);
		uint32_t nCRC32 = FFrame::CRC32(str);
		return GetSuitNode(nCRC32, node);
	}
	/*
	virtual bool GetSuitNode(const std::string& str, FXCVirtualNode<T>& node)
	{
		uint32_t nCRC32 = FFrame::CRC32(str);
        return GetSuitNode(nCRC32, node);
	}
	*/
	virtual bool GetSuitNode(uint32_t hashValue, FXCVirtualNode<T>& node)
	{
		if(mxNodes.empty())
		{
			return false;
		}

		typename std::map<uint32_t, FXCVirtualNode<T>>::iterator it = mxNodes.lower_bound(hashValue);

		if (it == mxNodes.end())
		{
			it = mxNodes.begin();
		}

		node = it->second;

		return true;
	}

	virtual bool GetNodeList(std::list<FXCVirtualNode<T>>& nodeList)
	{
		for (typename std::map<uint32_t, FXCVirtualNode<T>>::iterator it = mxNodes.begin(); it != mxNodes.end(); ++it)
		{
			nodeList.push_back(it->second);
		}

		return true;
	}

private:
	int mnNodeCount = 500;
	typename std::map<uint32_t, FXCVirtualNode<T>> mxNodes;
    FXIHasher* m_pHasher;
};


#endif
