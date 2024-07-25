//
// Created by yuntianming on 2024/6/24.
//

#ifndef FXFRAMETEST_FXBACKGROUNDTHREAD_H
#define FXFRAMETEST_FXBACKGROUNDTHREAD_H

#include <QThread>

class FXBackgroundThread : public QThread {
Q_OBJECT

public:
    explicit FXBackgroundThread(QObject *parent = nullptr);
    void stop();

protected:
    void run() override;

private:
    bool m_bExitApp;
};

#endif //FXFRAMETEST_FXBACKGROUNDTHREAD_H
