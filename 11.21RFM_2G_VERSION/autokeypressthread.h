#ifndef AUTOKEYPRESSTHREAD_H
#define AUTOKEYPRESSTHREAD_H

#include <QThread>

class MainWindow;  // 前向声明
class RemoteUI;    // 前向声明

class AutoKeyPressThread : public QThread
{
    Q_OBJECT
public:
    explicit AutoKeyPressThread(MainWindow *mainWindow, RemoteUI *remoteUI, QObject *parent = nullptr);
    void run() override;  // 重写run函数以启动线程

private:
    MainWindow *mainWindow;  // 指向主窗口的指针
    RemoteUI *remoteUI;      // 指向RemoteUI窗口的指针
    bool running;            // 线程运行标志

    int readRegister();      // 读取寄存器的函数

public slots:
    void stop();             // 停止线程的槽函数
};

#endif // AUTOKEYPRESSTHREAD_H
