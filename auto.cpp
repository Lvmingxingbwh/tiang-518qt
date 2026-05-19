//#include "Auto.h"

//Auto::Auto(QObject *parent) : QObject(parent), autoThread(nullptr), autoReadTimer(nullptr)
//{
//    // 构造函数
//}

//Auto::~Auto()
//{
//    // 清理资源
//    if (autoThread && autoThread->isRunning()) {
//        autoThread->quit();
//        autoThread->wait();
//    }
//    delete autoThread;
//    delete autoReadTimer;
//}

//void Auto::startAuto()
//{
//    if (autoThread == nullptr || !autoThread->isRunning()) {
//        autoThread = new QThread(this);
//        connect(autoThread, &QThread::started, this, &Auto::Auto_sport);
//        connect(autoThread, &QThread::finished, autoThread, &QObject::deleteLater);
//        autoThread->start();

//        // 启动定时器
//        if (autoReadTimer == nullptr) {
//            autoReadTimer = new QTimer(this);
//            connect(autoReadTimer, &QTimer::timeout, this, &Auto::Auto_sport);
//        }
//        autoReadTimer->start(1000); // 每秒调用一次 Auto
//    }
//}

//void Auto::stopAuto()
//{
//    if (autoThread && autoThread->isRunning()) {
//        autoThread->quit();
//        autoThread->wait();
//    }

//    if (autoReadTimer) {
//        autoReadTimer->stop(); // 停止定时器
//    }
//}

//void Auto::readMemoryData()
//{
//    RFM2GHANDLE handle;
//    RFM2G_STATUS status;
//    RFM2G_UINT32 offset = 0x4008000;

//    // 打开 RFM2G 设备
//    status = RFM2gOpen(0, &handle); // 假设设备号为0
//    if (status != RFM2G_SUCCESS) {
//        qCritical() << "无法打开 RFM2G 设备, 错误码:" << status;
//        return;
//    }

//    // 读取 DWORD 值
//    status = RFM2gRead(handle, offset, &RFM_Com, sizeof(RFM2G_UINT32));
//    if (status != RFM2G_SUCCESS) {
//        qCritical() << "无法读取 DWORD 值, 错误码:" << status;
//        return;
//    }

//    // 更新偏移量以读取浮点值
//    offset += sizeof(RFM2G_UINT32);

//    // 读取 5 个 float 值
//    status = RFM2gRead(handle, offset, &rfm_1, sizeof(float));
//    if (status == RFM2G_SUCCESS) offset += sizeof(float);
//    status = RFM2gRead(handle, offset, &rfm_2, sizeof(float));
//    if (status == RFM2G_SUCCESS) offset += sizeof(float);
//    status = RFM2gRead(handle, offset, &rfm_3, sizeof(float));
//    if (status == RFM2G_SUCCESS) offset += sizeof(float);
//    status = RFM2gRead(handle, offset, &rfm_4, sizeof(float));
//    if (status == RFM2G_SUCCESS) offset += sizeof(float);
//    status = RFM2gRead(handle, offset, &rfm_5, sizeof(float));

//    if (status != RFM2G_SUCCESS) {
//        qCritical() << "无法读取 float 值, 错误码:" << status;
//        return;
//    }

//    // 打印读取到的值
//    qDebug() << "RFM_Com:" << RFM_Com;
//    qDebug() << "rfm_1:" << rfm_1;
//    qDebug() << "rfm_2:" << rfm_2;
//    qDebug() << "rfm_3:" << rfm_3;
//    qDebug() << "rfm_4:" << rfm_4;
//    qDebug() << "rfm_5:" << rfm_5;

//    // 关闭 RFM2G 设备
////    RFM2gClose(handle);
//}

//void Auto::Auto_sport()
//{
//    readMemoryData();
//    for (int i = 0; i < 32; ++i) {
//        Auto_Com[i] = (RFM_Com >> i) & 1;
//    }

//    // 遍历每一位并进行处理
//    for (int i = 0; i < 32; ++i) {
//        if (Auto_Com[i]) {  // 如果该位是 1
//            switch (i) {
//                case 0:
//            {
//                    qDebug() << "Auto_Com[0] is 1: Performing action for bit 0";
//                    QByteArray messageData0 = QByteArray::fromHex("00");
//                    SendCommand(messageData0);
//                    break;
//            }

//            case 1:
//            {
//                    qDebug() << "Auto_Com[1] is 1: Performing action for bit 1";
//                    QByteArray messageData1 = QByteArray::fromHex("01");
//                    SendCommand(messageData1);
//                    break;
//}
//                case 2:
//            {
//                    qDebug() << "Auto_Com[2] is 1: Performing action for bit 2";
//                   // AutoStop();
//                    break;
//            }

//                case 3:
//                    qDebug() << "Auto_Com[3] is 1: Performing action for bit 3";
//                    //AutoRest();
//                    break;

//                case 7:
//                    qDebug() << "Auto_Com[7] is 1: Performing action for bit 7";
//                    //RelativeCommand(QVariant::fromValue(rfm_1), QVariant::fromValue(rfm_2));
//                    break;

//                case 8:
//                    qDebug() << "Auto_Com[8] is 1: Performing action for bit 8";
//                    //AbsoluteCommand(QVariant::fromValue(rfm_1), QVariant::fromValue(rfm_2));
//                    break;

//                case 9:
//                    qDebug() << "Auto_Com[9] is 1: Performing action for bit 9";
//                    //AutoSpeedM_F();
//                    break;

//                case 10:
//                    qDebug() << "Auto_Com[10] is 1: Performing action for bit 10";
//                    //AutoSpeedM_N();
//                    break;

//                case 11:
//                    qDebug() << "Auto_Com[11] is 1: Performing action for bit 11";
//                    //AutoSin(QVariant::fromValue(rfm_2));
//                    break;

//                default:
//                    break;  // 处理其他未覆盖的位
//            }
//        }
//    }
//}

//void Auto::SendCommand(const QByteArray &messageData)
//{
//    // 发送命令的实现（根据实际需求修改）
//    qDebug() << "Sending command:" << messageData.toHex();
//}
