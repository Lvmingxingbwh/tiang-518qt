#include "autokeypressthread.h"
#include "mainwindow.h"

#include <QMetaObject>

AutoKeyPressThread::AutoKeyPressThread(MainWindow *mainWindow, RemoteUI *remoteUI, QObject *parent)
    : QThread(parent), mainWindow(mainWindow), remoteUI(remoteUI), running(true) {}

void AutoKeyPressThread::run()
{
    while (running)
    {

        int registerValue = readRegister(); // 从寄存器读取当前指令

        // 使用QMetaObject::invokeMethod在主线程中执行相应的功能
        switch (registerValue)
        {
        case 1:
            QMetaObject::invokeMethod(mainWindow, "on_btnPower_toggled", Qt::QueuedConnection, Q_ARG(bool, true));
            break;
        case 2:
            QMetaObject::invokeMethod(mainWindow, "on_btnPower_toggled", Qt::QueuedConnection, Q_ARG(bool, false));
            break;
        case 3:
            QMetaObject::invokeMethod(mainWindow, "on_btnRelativeMove_clicked", Qt::QueuedConnection);
            break;
        case 4:
            QMetaObject::invokeMethod(mainWindow, "on_btnAbsoluteMove_clicked", Qt::QueuedConnection);
            break;
        case 5:
            QMetaObject::invokeMethod(mainWindow, "on_btnStop_clicked", Qt::QueuedConnection);
            break;
        case 6:
            QMetaObject::invokeMethod(mainWindow, "on_btnReset_clicked", Qt::QueuedConnection);
            break;
        case 7:
            QMetaObject::invokeMethod(mainWindow, "on_btnJogForward_pressed", Qt::QueuedConnection);
            break;
        case 8:
            QMetaObject::invokeMethod(mainWindow, "on_btnJogForward_released", Qt::QueuedConnection);
            break;
        case 9:
            QMetaObject::invokeMethod(mainWindow, "on_JogBackward_pressed", Qt::QueuedConnection);
            break;
        case 10:
            QMetaObject::invokeMethod(mainWindow, "on_JogBackward_released", Qt::QueuedConnection);
            break;
        // 添加控制RemoteUI按钮的逻辑

            break;
        default:
            break;
        }

        QThread::sleep(1); // 每秒检查一次寄存器
    }
}

int AutoKeyPressThread::readRegister()
{
    int registerValue = 1;
    // 此处添加实际的寄存器读取逻辑
    // 示例：从寄存器地址0x1234读取数据
    // registerValue = ReadFromRegister(0x1234);
    return registerValue;
}

void AutoKeyPressThread::stop()
{
    running = false;
}
