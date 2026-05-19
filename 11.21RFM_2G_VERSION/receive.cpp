#include "receive.h"
#include <QTimer>

Receive::Receive(QObject *parent)
    : QObject{parent}
{
    // Create a timer with a 10ms interval
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Receive::emitData); // Connect the timer to the emitData slot
    timer->start(1000); // Start the timer with a 10ms interval
}

#pragma pack(push, 1)
struct DataPacket {
    float position;
    float speed;
    float torque;
    float current;
    float errorCode;

    float position2;
    float speed2;
    float torque2;
    float current2;
    float errorCode2;

    float Acc;
    quint32 Frame;
    quint16 Total_status;
    quint16 Total_error;

    float X_position;
    float X_vec;
    quint32 X_Status;

    float Y_position;
    float Y_vec;
    quint32 Y_Status;

    float Z_position;
    float Z_vec;
    quint32 Z_Status;

    float roll;
    float roll_vec;
    quint32 Roll_Status;

    float pitch;
    float pitch_vec;
    quint32 Pitch_Status;

    float yaw;
    float yaw_vec;
    quint32 yaw_Status;

    float Theta1;
    float Theta1_Vec;
    float Theta2;
    float Theta2_Vec;

    float Theta3;
    float Theta3_Vec;
    float Theta4;
    float Theta4_Vec;

    float Theta5;
    float Theta5_Vec;
    float Theta6;
    float Theta6_Vec;

    quint32 Rob_FB1;
    quint32 Rob_FB2;
};
#pragma pack(pop)
//void Receive::handleData(const QByteArray &data)
//{
//    QDataStream stream(data);
//    stream.setByteOrder(QDataStream::LittleEndian); // Ensure the same byte order as the sender

//    // Declare variables for storing the data read from the stream
//    stream.readRawData(reinterpret_cast<char*>(&position), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&speed), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&torque), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&current), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&errorCode), sizeof(float));

//    stream.readRawData(reinterpret_cast<char*>(&position2), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&speed2), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&torque2), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&current2), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&errorCode2), sizeof(float));

//    stream.readRawData(reinterpret_cast<char*>(&Acc), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Frame), sizeof(quint32));
//    stream.readRawData(reinterpret_cast<char*>(&Total_status), sizeof(quint32));
//    stream.readRawData(reinterpret_cast<char*>(&Total_error), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&X_position), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&X_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&X_Status), sizeof(quint32));
//    qDebug() << "X_position:" << X_position
//                 << "X_vec:" << X_vec
//                 << "X_Status:" << X_Status;
//    stream.readRawData(reinterpret_cast<char*>(&Y_position), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Y_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Y_Status), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&Z_position), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Z_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Z_Status), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&roll), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&roll_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Roll_Status), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&pitch), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&pitch_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Pitch_Status), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&yaw), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&yaw_vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&yaw_Status), sizeof(quint32));

//    stream.readRawData(reinterpret_cast<char*>(&Theta1), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta1_Vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta2), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta2_Vec), sizeof(float));

//    stream.readRawData(reinterpret_cast<char*>(&Theta3), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta3_Vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta4), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta4_Vec), sizeof(float));

//    stream.readRawData(reinterpret_cast<char*>(&Theta5), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta5_Vec), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta6), sizeof(float));
//    stream.readRawData(reinterpret_cast<char*>(&Theta6_Vec), sizeof(float));

//    stream.readRawData(reinterpret_cast<char*>(&Rob_FB1), sizeof(quint32));
//    stream.readRawData(reinterpret_cast<char*>(&Rob_FB2), sizeof(quint32));
//    qDebug() << "Raw data:" << data.toHex(' ').toUpper();
//}
void Receive::handleData(const QByteArray &data)
{
    // 检查数据包是否完整
    const int expectedSize = sizeof(float) * 32 + sizeof(quint32) * 12 + sizeof(quint16) * 2;
    if (data.size() < expectedSize) {
        qWarning() << "Received incomplete data packet. Expected:" << expectedSize << "bytes, but got:" << data.size();
        return;
    }

    qDebug() << "Raw data:" << data.toHex(' ').toUpper(); // 打印原始字节流

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian); // 确保字节序为小端

    // 定义一个 lambda 函数，用于解析 float 和 quint32
    auto readFloat = [&]() -> float {
        float value;
        stream.readRawData(reinterpret_cast<char*>(&value), sizeof(float));
        return value;
    };

    auto readUInt32 = [&]() -> quint32 {
        quint32 value;
        stream.readRawData(reinterpret_cast<char*>(&value), sizeof(quint32));
        return value;
    };

    auto readUInt16 = [&]() -> quint16 {
        quint16 value;
        stream.readRawData(reinterpret_cast<char*>(&value), sizeof(quint16));
        return value;
    };

    // 按顺序解析数据
    position = readFloat();
    speed = readFloat();
    torque = readFloat();
    current = readFloat();
    errorCode = readFloat();

    position2 = readFloat();
    speed2 = readFloat();
    torque2 = readFloat();
    current2 = readFloat();
    errorCode2 = readFloat();

    Acc = readFloat();
    Frame = readUInt32();
    Total_status = readUInt16();
    Total_error = readUInt16();

    X_position = readFloat();
    X_vec = readFloat();
    X_Status = readUInt32();

    Y_position = readFloat();
    Y_vec = readFloat();
    Y_Status = readUInt32();

    Z_position = readFloat();
    Z_vec = readFloat();
    Z_Status = readUInt32();

    roll = readFloat();
    roll_vec = readFloat();
    Roll_Status = readUInt32();

    pitch = readFloat();
    pitch_vec = readFloat();
    Pitch_Status = readUInt32();

    yaw = readFloat();
    yaw_vec = readFloat();
    yaw_Status = readUInt32();

    Theta1 = readFloat();
    Theta1_Vec = readFloat();
    Theta2 = readFloat();
    Theta2_Vec = readFloat();

    Theta3 = readFloat();
    Theta3_Vec = readFloat();
    Theta4 = readFloat();
    Theta4_Vec = readFloat();

    Theta5 = readFloat();
    Theta5_Vec = readFloat();
    Theta6 = readFloat();
    Theta6_Vec = readFloat();

    Rob_FB1 = readUInt32();
    Rob_FB2 = readUInt32();

    // 打印解析后的数据
//    qDebug() << "Parsed Data:";
//    qDebug() << "Position:" << position << "Speed:" << speed << "Torque:" << torque
//             << "Current:" << current << "ErrorCode:" << errorCode;

//    qDebug() << "Position2:" << position2 << "Speed2:" << speed2 << "Torque2:" << torque2
//             << "Current2:" << current2 << "ErrorCode2:" << errorCode2;

//    qDebug() << "Acc:" << Acc << "Frame:" << Frame
//             << "Total_status:" << Total_status << "Total_error:" << Total_error;

//    qDebug() << "X_Position:" << X_position << "X_Vec:" << X_vec << "X_Status:" << X_Status;
//    qDebug() << "Y_Position:" << Y_position << "Y_Vec:" << Y_vec << "Y_Status:" << Y_Status;
//    qDebug() << "Z_Position:" << Z_position << "Z_Vec:" << Z_vec << "Z_Status:" << Z_Status;

//    qDebug() << "Roll:" << roll << "Roll_Vec:" << roll_vec << "Roll_Status:" << Roll_Status;
//    qDebug() << "Pitch:" << pitch << "Pitch_Vec:" << pitch_vec << "Pitch_Status:" << Pitch_Status;
//    qDebug() << "Yaw:" << yaw << "Yaw_Vec:" << yaw_vec << "Yaw_Status:" << yaw_Status;

//    qDebug() << "Theta1:" << Theta1 << "Theta1_Vec:" << Theta1_Vec
//             << "Theta2:" << Theta2 << "Theta2_Vec:" << Theta2_Vec;

//    qDebug() << "Theta3:" << Theta3 << "Theta3_Vec:" << Theta3_Vec
//             << "Theta4:" << Theta4 << "Theta4_Vec:" << Theta4_Vec;

//    qDebug() << "Theta5:" << Theta5 << "Theta5_Vec:" << Theta5_Vec
//             << "Theta6:" << Theta6 << "Theta6_Vec:" << Theta6_Vec;

//    qDebug() << "Rob_FB1:" << Rob_FB1 << "Rob_FB2:" << Rob_FB2;
}

void Receive::emitData()

{
    // Emit the signal with the latest parsed data every 10ms
    emit dataSend(position, speed, torque, current, errorCode,
                  position2, speed2, torque2, current2, errorCode2,
                  Acc, Frame, Total_status, Total_error,
                  X_position, X_vec, X_Status,
                  Y_position, Y_vec, Y_Status,
                  Z_position, Z_vec, Z_Status,
                  roll, roll_vec, Roll_Status,
                  pitch, pitch_vec, Pitch_Status,
                  yaw, yaw_vec, yaw_Status,
                  Theta1, Theta1_Vec, Theta2, Theta2_Vec,
                  Theta3, Theta3_Vec, Theta4, Theta4_Vec,
                  Theta5, Theta5_Vec, Theta6, Theta6_Vec,
                  Rob_FB1, Rob_FB2);
}
