
#ifndef FX_MEMORY_COUNTER_H
#define FX_MEMORY_COUNTER_H

#include <iostream>
#include <string>
#include <map>
#include "FXPlatform.h"

template <class T>
class FXMemoryCounter
{
private:
	FXMemoryCounter() {}
public:
	FXMemoryCounter(const std::string& strClassName)
	{
		mstrClassName = strClassName;
		mxCounter[mstrClassName]++;
	}

	~FXMemoryCounter()
	{
		mxCounter[mstrClassName]--;
	}

	std::string mstrClassName;
	static std::map<std::string, int> mxCounter;
};

template <class T>
std::map<std::string, int> FXMemoryCounter<T>::mxCounter;

#endif
