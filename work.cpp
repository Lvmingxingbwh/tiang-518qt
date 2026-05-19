#include "work.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QMessageBox>   // 正确的头文件
#include "rfm2g_api.h"
#include "rfm2g_windows.h"
#include "mainwindow.h"
#include <QMessageBox>
#define DEVICE "\\\\.\\rfm2g1"


// Worker class definition
Worker::Worker(QObject *parent) : QObject(parent), timer(new QTimer(this)), bPower(false)
{
//    udpSocket = new QUdpSocket(this);
//        if (!udpSocket->bind(QHostAddress::Any, 5080)) {
//            qCritical() << "Failed to bind UDP socket:" << udpSocket->errorString();
//        }
    // Set timer interval to 1000 ms (1 second)
    timer->setInterval(50);

    // Connect the timer's timeout signal to the onTimeout slot
    connect(timer, &QTimer::timeout, this, &Worker::onTimeout);

}

Worker::~Worker()
{
    // Timer will stop automatically on destruction, but we'll delete it explicitly here
    delete timer;
}

//void Worker::SendCommand(const QByteArray &command)
//{
//    // 检查是否有命令要发送
//    if (command.isEmpty()) {
//        qDebug() << "Command is empty!";
//        return;
//    }

//    // 检查UDP套接字是否已经绑定到端口
//    if (udpSocket != nullptr && udpSocket->state() == QAbstractSocket::BoundState) {
//        QHostAddress address("169.254.237.68");  // 使用 QHostAddress 来处理 IP 地址
//        quint16 port = 5060;  // 端口号应为 quint16 类型

//        // 发送数据报到指定的IP地址和端口
//        qint64 bytesSent = udpSocket->writeDatagram(command.data(), command.size(), address, port);

//        if (bytesSent == -1) {
//            qDebug() << "Failed to send datagram:" << udpSocket->errorString();
//        } else {
//            qDebug() << "Datagram sent to" << address.toString() << "on port" << port;
//        }
//    } else {
//        qDebug() << "UDP socket is not bound or invalid!";
//    }
//}

void Worker::startTimer()
{
    qDebug() << "Worker::startTimer called - Starting timer with interval:" << timer->interval();
    // Start the timer
    timer->start();
    qDebug() << "Timer start Successfully" ;
}

void Worker::onTimeout()
{
//    static int count = 0;
//    qDebug() << "Timer triggered, count: " << ++count;
    Auto();
    clearRfm2gData();
}

// Read memory data from the RFM2G device
void Worker::readMemoryData()
{

    RFM2GHANDLE handle = nullptr;
    RFM2G_STATUS status;
    RFM2G_UINT32 offset = 0x4008000;

    // Open RFM2G device
    status = RFM2gOpen(const_cast<char*>(DEVICE), &handle);
    if (status != RFM2G_SUCCESS) {
        qCritical() << "Failed to open RFM2G device, error code:" << status;
        return;
    }

    // Use a scoped structure to ensure cleanup
    do {
        // Read DWORD value
        status = RFM2gRead(handle, offset, &RFM_Com, sizeof(RFM2G_UINT32));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read RFM_Com, error code:" << status;
            break;
        }

        // Update the offset to read float values
        offset += sizeof(RFM2G_UINT32);

        // Read the float values
        status = RFM2gRead(handle, offset, &rfm_1, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_1, error code:" << status;
            break;
        }
        offset += sizeof(float);

        status = RFM2gRead(handle, offset, &rfm_2, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_2, error code:" << status;
            break;
        }
        offset += sizeof(float);

        status = RFM2gRead(handle, offset, &rfm_3, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_3, error code:" << status;
            break;
        }
        offset += sizeof(float);

        status = RFM2gRead(handle, offset, &rfm_4, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_4, error code:" << status;
            break;
        }
        offset += sizeof(float);

        status = RFM2gRead(handle, offset, &rfm_5, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_6, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_7, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_8, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_9, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_10, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_11, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_12, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_13, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_14, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_15, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_16, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_17, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_12, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
        offset += sizeof(float);
        status = RFM2gRead(handle, offset, &rfm_18, sizeof(float));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read rfm_5, error code:" << status;
            break;
        }
    } while (false);

    // Close the RFM2G device
    if (handle != nullptr) {
        RFM2gClose(&handle);
    }

//    // Debug output

    if(RFM_Com!=0){
        qDebug() << "RFM_Com:" << RFM_Com;
        qDebug() << "rfm_1:" << rfm_1;
        qDebug() << "rfm_2:" << rfm_2;
        qDebug() << "rfm_3:" << rfm_3;
        qDebug() << "rfm_4:" << rfm_4;
        qDebug() << "rfm_5:" << rfm_5;
        qDebug() << "rfm_6:" << rfm_6;
        qDebug() << "rfm_7:" << rfm_7;
        qDebug() << "rfm_8:" << rfm_8;
        qDebug() << "rfm_9:" << rfm_9;
        qDebug() << "rfm_10:" << rfm_10;
        qDebug() << "rfm_11:" << rfm_11;
        qDebug() << "rfm_12:" << rfm_12;
        qDebug() << "rfm_13:" << rfm_13;
        qDebug() << "rfm_14:" <<  rfm_14;
        qDebug() << "rfm_15:" << rfm_15;
        qDebug() << "rfm_16:" << rfm_16;
        qDebug() << "rfm_17:" << rfm_17;
        qDebug() << "rfm_18:" << rfm_18;
    }

    RFM2gClose(&handle);  // Uncomment if necessary
}


//#pragma pack(push, 1)
//struct RFMData {
//    quint32 RFM_Com;  // DWORD (4 bytes)
//    float rfm_1;
//    float rfm_2;
//    float rfm_3;
//    float rfm_4;
//    float rfm_5;
//    float rfm_6;
//    float rfm_7;
//    float rfm_8;
//    float rfm_9;
//    float rfm_10;
//    float rfm_11;
//    float rfm_12;
//    float rfm_13;
//    float rfm_14;
//    float rfm_15;
//    float rfm_16;
//    float rfm_17;
//    float rfm_18;
//    float rfm_19;
//    float rfm_20;
//    float rfm_21;
//    float rfm_22;
//    float rfm_23;
//    float rfm_24;
//};
//#pragma pack(pop)
//void Worker::readMemoryData()
//{
//    RFM2GHANDLE handle = nullptr;
//    RFM2G_STATUS status;
//    RFM2G_UINT32 offset = 0x4008000;
//    RFMData data;  // 定义结构体实例

//    // Open RFM2G device
//    status = RFM2gOpen(const_cast<char*>(DEVICE), &handle);
//    if (status != RFM2G_SUCCESS) {
//        qCritical() << "Failed to open RFM2G device, error code:" << status;
//        return;
//    }

//    // 读取整个结构体数据
//    status = RFM2gRead(handle, offset, &data, sizeof(RFMData));
//    if (status != RFM2G_SUCCESS) {
//        qCritical() << "Failed to read RFMData, error code:" << status;
//    }

//    // Close the RFM2G device
//    if (handle != nullptr) {
//        RFM2gClose(&handle);
//    }

//    // Debug output
//    qDebug() << "RFM_Com:" << data.RFM_Com;
//    qDebug() << "rfm_1:" << data.rfm_1;
//    qDebug() << "rfm_2:" << data.rfm_2;
//    qDebug() << "rfm_3:" << data.rfm_3;
//    qDebug() << "rfm_4:" << data.rfm_4;
//    qDebug() << "rfm_5:" << data.rfm_5;
//    qDebug() << "rfm_6:" << data.rfm_6;
//    qDebug() << "rfm_7:" << data.rfm_7;
//    qDebug() << "rfm_8:" << data.rfm_8;
//    qDebug() << "rfm_9:" << data.rfm_9;
//    qDebug() << "rfm_10:" << data.rfm_10;
//    qDebug() << "rfm_11:" << data.rfm_11;
//    qDebug() << "rfm_12:" << data.rfm_12;
//    qDebug() << "rfm_13:" << data.rfm_13;
//    qDebug() << "rfm_14:" << data.rfm_14;
//    qDebug() << "rfm_15:" << data.rfm_15;
//    qDebug() << "rfm_16:" << data.rfm_16;
//    qDebug() << "rfm_17:" << data.rfm_17;
//    qDebug() << "rfm_18:" << data.rfm_18;
//    qDebug() << "rfm_19:" << data.rfm_19;
//    qDebug() << "rfm_20:" << data.rfm_20;
//    qDebug() << "rfm_21:" << data.rfm_21;
//    qDebug() << "rfm_22:" << data.rfm_22;
//    qDebug() << "rfm_23:" << data.rfm_23;
//    qDebug() << "rfm_24:" << data.rfm_24;

//}


// Absolute Command definition
void Worker::AbsoluteCommand(const QVariant &param1, const QVariant &param2)
{
    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = param1.toFloat(&okAbsolutePosition);
    float AbsoluteSpeed = param2.toFloat(&okAbsoluteSpeed);


    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(11)  // Message type (assumed as 11)
           << static_cast<quint8>(1)   // Subtype (assumed as 1)
           << AbsolutePosition         // Absolute position
           << AbsoluteSpeed;           // Absolute speed

    emit sendCommandResquest(messageData);

    // Send additional command
    QByteArray messageData1 = QByteArray::fromHex("0C");
    emit sendCommandResquest(messageData1);
}

// Relative Command definition
void Worker::RelativeCommand(const QVariant &param1, const QVariant &param2)
{


    bool okRelativePosition, okRelativeSpeed;
    float RelativePosition = param1.toFloat(&okRelativePosition);
    float RelativeSpeed = param2.toFloat(&okRelativeSpeed);


    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(40)
           << static_cast<quint8>(1)
           << RelativePosition << RelativeSpeed;

    emit sendCommandResquest(messageData);

    // Send additional command
    QByteArray messageData1 = QByteArray::fromHex("29");
    emit sendCommandResquest(messageData1);
}

// AutoSin command definition
void Worker::AutoSin(const QVariant &param)
{
    if (bPower) {
        float sinSpeed = param.toFloat();
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(16)
               << sinSpeed;
        emit sendCommandResquest(messageData);
    } else {

    }

    // Send additional command
    QByteArray messageData = QByteArray::fromHex("11");
    emit sendCommandResquest(messageData);
}

// AutoRest command definition
void Worker::AutoRest()
{
    if (bPower) {
        QByteArray messageData = QByteArray::fromHex("04");
        emit sendCommandResquest(messageData);
    }

    // Send additional command
    QByteArray messageData = QByteArray::fromHex("05");
    emit sendCommandResquest(messageData);
}

// AutoStop command definition
void Worker::AutoStop()
{
    if (bPower) {
        QByteArray messageData = QByteArray::fromHex("02");
        emit sendCommandResquest(messageData);

    }
    // Send additional command
    QByteArray messageData = QByteArray::fromHex("03");
    emit sendCommandResquest(messageData);

}

// AutoSpeedM_F (Forward) command definition
void Worker::AutoSpeedM_F()
{
    if (bPower) {
        QByteArray messageData1 = QByteArray::fromHex("0D");
        emit sendCommandResquest(messageData1);
    }

    // Send additional command
    QByteArray messageData = QByteArray::fromHex("0F");
    emit sendCommandResquest(messageData);
}

// AutoSpeedM_N (Reverse) command definition
void Worker::AutoSpeedM_N()
{
    if (bPower) {
        QByteArray messageData1 = QByteArray::fromHex("0E");
        emit sendCommandResquest(messageData1);
    }

    // Send additional command
    QByteArray messageData = QByteArray::fromHex("0F");
    emit sendCommandResquest(messageData);
}

// Auto command (based on RFM_Com bit values)
void Worker::Auto()
{
    MainWindow mw;
    memset(Auto_Com, 0, sizeof(Auto_Com));
    // Parse RFM_Com and process each bit
    readMemoryData();  // 注意：确保 readMemoryData 安全性
//    qDebug()<<"Auto_Com"<<Auto_Com;

    for (int i = 0; i < 32; ++i) {
        Auto_Com[i] = (RFM_Com >> i) & 1;
    }
//    qDebug() << "Parsed Auto_Com array:";
//       for (int i = 0; i < 32; ++i) {
//           qDebug() << "Auto_Com[" << i << "]:" << Auto_Com[i];
//       }

    for (int i = 0; i < 32; ++i) {
        if (Auto_Com[i])
        {
            switch (i) {
                case 0:
            {


                    qDebug() << "Auto_Com[0] is 1: Performing action for bit 0";

                    QByteArray messageData0 = QByteArray::fromHex("00");
                    emit sendCommandResquest(messageData0);
                    break;
            }

                case 1:
            {

                    mw.powerOn();
                    qDebug() << "Auto_Com[1] is 1: Performing action for bit 1";
                    // QByteArray messageData1 = QByteArray::fromHex("01");
                    // emit sendCommandResquest(messageData1);
                    break;
}
                case 2:
                    qDebug() << "Auto_Com[2] is 1: Performing action for bit 2";
                    AutoStop();
                    break;

                case 3:
                    qDebug() << "Auto_Com[3] is 1: Performing action for bit 3";
                    AutoRest();
                    break;
                case 4:
                    qDebug() << "Auto_Com[4] is 1: Performing action for bit 4";
                    AutoRest();
                    break;
                case 5:
                    qDebug() << "Auto_Com[5] is 1: Performing action for bit 5";
                    AutoRest();
                    break;
                case 6:
                    qDebug() << "Auto_Com[6] is 1: Performing action for bit 6";
                    AutoRest();
                    break;

                case 7:
                    qDebug() << "Auto_Com[7] is 1: Performing action for bit 7";
//                    RelativeCommand(QVariant::fromValue(rfm_1), QVariant::fromValue(rfm_2));
                    emit abreluteMoveRequest(rfm_1, rfm_2);
                    break;

                case 8:
                    qDebug() << "Auto_Com[8] is 1: Performing action for bit 8";
                    qDebug() <<"rfm1"<<rfm_1;
//                    AbsoluteCommand(QVariant::fromValue(rfm_1), QVariant::fromValue(rfm_2));
                    emit absoluteMoveRequest(rfm_1, rfm_2);
                    break;

                case 9:
                    qDebug() << "Auto_Com[9] is 1: Performing action for bit 9";
                    AutoSpeedM_F();
                    break;

                case 10:
            {
                    qDebug() << "Auto_Com[10] is 1: Performing action for bit 10";
                    AutoSpeedM_N();
                    break;
            }

                case 11:
            {
                    qDebug() << "Auto_Com[11] is 1: Performing action for bit 11";
                    AutoSin(QVariant::fromValue(rfm_2));
                    break;
            }
            case 12:
        {
                qDebug() << "Auto_Com[12] is 1: Performing action for bit 12";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
            case 13:
        {
                qDebug() << "Auto_Com[13] is 1: Performing action for bit 13";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
            case 14:
        {
                qDebug() << "Auto_Com[14] is 1: Performing action for bit 14";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
            case 15:
        {
                qDebug() << "Auto_Com[15] is 1: Performing action for bit 15";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
            case 16:
        {
                qDebug() << "Auto_Com[16] is 1: Performing action for bit 16";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
            case 17:
        {
                qDebug() << "Auto_Com[15] is 1: Performing action for bit 17";
                AutoSin(QVariant::fromValue(rfm_2));
                break;
        }
                default:
                    break;  // Default case, typically nothing happens here
            }
        }

    }
}


void Worker::clearRfm2gData()
{
    RFM2GHANDLE handle = nullptr;

    // 打开设备
    RFM2G_STATUS status = RFM2gOpen(const_cast<char*>(DEVICE), &handle);
    if (status != RFM2G_SUCCESS || handle == nullptr) {
        qCritical() << "Failed to open RFM2G device, error code:" << status;
        return;  // 设备打开失败，退出
    }

    const RFM2G_UINT32 startAddress = 0x4008000;
    const RFM2G_UINT32 endAddress = 0x4008280;
    const RFM2G_UINT32 dataSize = endAddress - startAddress + 1;

    // 创建缓冲区并填充为 0
    QVector<float> clearData(dataSize / sizeof(float), 0.0f);
    float *buffer = clearData.data();

    // 写入清零数据到指定地址范围
    status = RFM2gWrite(handle, startAddress, buffer, dataSize);
    if (status == RFM2G_SUCCESS) {
//        qDebug() << "Cleared RFM2G data successfully.";
    } else {
        qCritical() << "Failed to clear RFM2G data. Error code:" << status;
    }

    // 关闭设备
    RFM2gClose(&handle);
}



