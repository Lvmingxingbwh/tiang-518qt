#ifndef WORK_H
#define WORK_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QUdpSocket>
#include <QtCharts>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QDebug>
 // 引入 MainWindow 头文件

class Worker : public QObject
{
    Q_OBJECT

public:
    quint32  RFM_Com;
    float rfm_1, rfm_2, rfm_3, rfm_4, rfm_5, rfm_6;
    float rfm_7, rfm_8, rfm_9, rfm_10, rfm_11, rfm_12;
    float rfm_13, rfm_14, rfm_15, rfm_16, rfm_17, rfm_18;
    float rfm_19;
    float rfm_20;
    float rfm_21;
    float rfm_22;
    float rfm_23;
    float rfm_24;

    bool Auto_Com[32];

    explicit Worker( QObject *parent = nullptr);
    // 设置电源状态的公共方法
       void setPowerState(bool powered);
       bool getPowerState() const { return bPower; }
    ~Worker();

    void startTimer();
    void readMemoryData();
    void AbsoluteCommand(const QVariant &param1, const QVariant &param2);
    void RelativeCommand(const QVariant &param1, const QVariant &param2);
    void AutoSin(const QVariant &param);
    void AutoRest();
    void AutoStop();
    void AutoSpeedM_F();
    void AutoSpeedM_N();
    void Auto();
    void clearRfm2gData();
private slots:
    void onTimeout();
//    void SendCommand(const QByteArray &command);

private:
//    QUdpSocket *udpSocket = nullptr;
//      // 用于存储传入的 MainWindow 指针
    QTimer *timer;  // 定时器
    bool bPower;


signals:
    void sendCommandResquest(const QByteArray &command);
    void absoluteMoveRequest(float position, float speed);
    void abreluteMoveRequest(float position, float speed);
    void powerStateChanged(bool powered);

};

#endif // WORK_H
