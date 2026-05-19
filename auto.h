//#ifndef AUTO_H
//#define AUTO_H


//#include <QMessageBox>
//#include <QUdpSocket>
//#include <QtCharts>
//#include <QDateTime>
//#include <QFile>
//#include <QTextStream>
//#include <QThread>
//#include <QDebug>
//#include "receive.h"
//#include "qlog.h"
//#include "qcustomplot.h"
//// #include "autokeypress.h"
//#include <QtConcurrent>
//#include <QFileDialog>
//#include "excelreader.h"
//#include "rfm2g_api.h"
//#include"rfm2g_windows.h"
//#include<QVector>


//class Auto : public QObject
//{
//    Q_OBJECT

//public:
//    explicit Auto(QObject *parent = nullptr);
//    ~Auto();

//    void startAuto();
//    void stopAuto();

//    void readMemoryData();
//    void AbsoluteCommand(const QVariant &param1, const QVariant &param2);
//    void RelativeCommand(const QVariant &param1, const QVariant &param2);
//    void AutoSin(const QVariant &param);
//    void AutoRest();
//    void AutoStop();
//    void AutoSpeedM_F();
//    void AutoSpeedM_N();
//    void Auto_sport();

//    bool bPower = true;  // 假设设备默认开启

//signals:
//    void finished();
//    void AutoReadFinished();  // 发送定时器读取完成信号

//private:
//    float rfm_1, rfm_2, rfm_3, rfm_4, rfm_5;
//    uint32_t RFM_Com;  // 存储 RFM2G 读取到的 DWORD 值
//    bool Auto_Com[32];  // 存储 32 位命令的标志位

//    QTimer *autoReadTimer;
//    QThread *autoThread;

//    void SendCommand(const QByteArray &messageData); // 发送命令函数（需要根据实际实现调整）
//};

//#endif // AUTO_H
