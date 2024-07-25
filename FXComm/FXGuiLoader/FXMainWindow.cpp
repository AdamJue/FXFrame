//
// Created by yuntianming on 2024/6/24.
//
#include "FXMainWindow.h"
#include "ui_mainWindow.h"
#include "FXBackGroundThread.h"


FXMainWindow::FXMainWindow(QWidget *parent)
: QMainWindow(parent),
ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    m_backgroundTask = new FXBackgroundThread(this);
    m_backgroundTask->start();


    FXIPluginManager *pPluginManager = FXCPluginManager::GetSingletonPtr();
    m_pEventModule = pPluginManager->FindModule<FXIEventModule>();

    connect(ui->pBtn_start, &QPushButton::clicked, this,
            &FXMainWindow::OnStartGenerate);


    m_pEventModule->AddEventCallBack(FXGUID(0, 1), FXEventDefine::FXED_COMMON_GUI, this, &FXMainWindow::OnEvent);

//    qDebug() << "Main thread ID:" << QThread::currentThreadId();
//    std::cout << "Main this thread id:" << std::this_thread::get_id() << std::endl;

}

FXMainWindow::~FXMainWindow()
{
    m_backgroundTask->stop();
    m_backgroundTask->wait();
    delete ui;
}

void FXMainWindow::OnStartGenerate()
{
    m_pEventModule->DoAsynEvent(FXGUID(0,1), FXEventDefine::FXED_START_GENERATAGE_MESH, FXDataList() << int(100) << "200");
}

int FXMainWindow::OnEvent(const FXGUID& self, const FXEventDefine event, const FXDataList& arg)
{
    if (event != FXEventDefine::FXED_COMMON_GUI)
    {
        return 0;
    }

    QCoreApplication::postEvent(this, new CustomEvent(arg));

//    std::cout << "FXMainWindow::OnEvent this thread id:" << std::this_thread::get_id() << std::endl;
//    std::cout << "FXMainWindow EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0)  << std::endl;

    return 0;
}

bool FXMainWindow::event(QEvent *event)  {
    if (event->type() == CustomEvent::Type)
    {
        CustomEvent *customEvent = static_cast<CustomEvent *>(event);
       // QString data = customEvent->getData();
        FXDataList arg = customEvent->getDataList();
        int subEvent = arg.Int(0);
        QString msg;
        switch (subEvent) {
            case 1:
            {
                msg.append("login success ");
                msg.append("\n");
            }
                break;
            case 2:
            {
                int leftPercent = arg.Int(1);
                msg.append("generate Percent ");
                msg.append(QString::number(leftPercent));
                msg.append("\n");
                ui->progressBar->setValue(leftPercent);
            }
                break;
            case 3:
            {
                int result = arg.Int(1);
                msg.append("generate result ");
                msg.append(QString::number(result));
                msg.append("\n");
            }
                break;
            default:
            {
                // no event
                return true;
            }
                break;
        }

        QTextEdit *textEdit = ui->textEdit;
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);

        cursor.insertText(msg);

        textEdit->setTextCursor(cursor);
        textEdit->ensureCursorVisible();

        return true;
    }
    return QMainWindow::event(event);
}