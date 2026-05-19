#ifndef SELF_TEST_H
#define SELF_TEST_H

#include <QWidget>
#include <QTimer>
#include <QUdpSocket>
#include "qlog.h"
#include "mainwindow.h"

namespace Ui {
class Self_Test;
}

class Self_Test : public QWidget
{
    Q_OBJECT

public:
    explicit Self_Test(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~Self_Test();
    bool Shux_zijian;
private:
    Ui::Self_Test *ui;
    QTimer *timer;
    int step;
    int retryCount;  // 重新定义 retryCount 成员变量
    int maxRetries;  // 重新定义 maxRetries 成员变量
    QUdpSocket *udpSocket;
    MainWindow *mainWindow;  // 主窗口指针
    qlog *QlogObj;

    void startSelfTest();
    void updateSelfTestInfo();
    void UDP_Send(int index);

private slots:
//void onErrorCodeChanged(int newErrorCode);
    void handleMonitoringResult(bool success);  // 确保声明 handleMonitoringResult
};

#endif // SELF_TEST_H
