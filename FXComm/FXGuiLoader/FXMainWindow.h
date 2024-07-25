//
// Created by yuntianming on 2024/6/24.
//

#ifndef FXFRAMETEST_FXMAINWINDOW_H
#define FXFRAMETEST_FXMAINWINDOW_H
#include <QMainWindow>
#include "FXBackGroundThread.h"
#include "FXCPluginManager.h"
#include "FXIEventModule.h"
#include "FXDataList.hpp"

#include <QEvent>


class CustomEvent : public QEvent {
public:
    static const QEvent::Type Type = static_cast<QEvent::Type>(QEvent::User + 1);
    CustomEvent(const QString &data) : QEvent(Type), m_data(data) {}
    CustomEvent(const FXDataList &data) : QEvent(Type), m_datalist(data) {}

    QString getData() const { return m_data; }
    FXDataList getDataList() const { return m_datalist; }

private:
    QString m_data;
    FXDataList m_datalist;
};

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class FXMainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit FXMainWindow(QWidget *parent = nullptr);

    ~FXMainWindow() override;

protected:
    int OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg);

    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    FXBackgroundThread* m_backgroundTask ;
    FXIEventModule* m_pEventModule;


private slots:
    void OnStartGenerate();
};

#endif //FXFRAMETEST_FXMAINWINDOW_H
