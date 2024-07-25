//
// Created by yuntianming on 2024/6/24.
//
#include "FXBackGroundThread.h"
#include <chrono>
#include <thread>
#include "FXCPluginManager.h"

FXBackgroundThread::FXBackgroundThread(QObject *parent): QThread(parent),
                                                         m_bExitApp(false)
{

}

void FXBackgroundThread::stop()
{
    m_bExitApp = true;
}

void FXBackgroundThread::run()
{
    while (!m_bExitApp) {

        if (m_bExitApp) {
            break;
        }

        auto frameStartTime = std::chrono::steady_clock::now();

        FXCPluginManager::GetSingletonPtr()->Execute();

        auto frameEndTime = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);

        auto targetFrameTime = std::chrono::milliseconds(1);
        auto sleepTime = targetFrameTime - frameDuration;

        if (sleepTime > std::chrono::milliseconds::zero()) {
            std::this_thread::sleep_for(sleepTime);
        }

    }
    FXCPluginManager::GetSingletonPtr()->BeforeShut();
    FXCPluginManager::GetSingletonPtr()->Shut();
    FXCPluginManager::GetSingletonPtr()->Finalize();
    FXCPluginManager::GetSingletonPtr()->ReleaseInstance();

}