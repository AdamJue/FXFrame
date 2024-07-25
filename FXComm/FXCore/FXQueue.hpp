
#ifndef FX_QUEUE_H
#define FX_QUEUE_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "FXPlatform.h"

class FXLock
{
public:
    explicit FXLock()
    {
        flag.clear();
    }

    ~FXLock()
    {
    }
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

protected:
    mutable std::atomic_flag flag;

private:
    FXLock& operator=(const FXLock& src);
};

template<typename T>
class FXQueue : public FXLock
{
public:
    FXQueue()
    {
    }

    virtual ~FXQueue()
    {
    }

    bool Push(const T& object)
    {
        lock();

        mList.push_back(object);

        unlock();

        return true;
    }

    bool Pop(T& object)
    {
        lock();

        if (mList.empty())
        {
            unlock();

            return false;
        }

        object = mList.front();
        mList.pop_front();

        unlock();

        return true;
    }

private:
    std::list<T> mList;
};

#endif
