#ifndef RECEIVE_H
#define RECEIVE_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

class Receive : public QObject
{
    Q_OBJECT
public:
    explicit Receive(QObject *parent = nullptr);
    void emitData();
    quint32 Frame, X_Status,Y_Status,Z_Status,Roll_Status,Pitch_Status,yaw_Status;
    float position, speed ,  current,torque;
    float errorCode;
    float position2,  speed2,  torque2,  current2;
    float errorCode2;
    float Acc;
    quint16 Total_status, Total_error;
    float X_position, X_vec;
    float Y_position,  Y_vec,Z_position,  Z_vec,roll,  roll_vec, pitch,  pitch_vec, yaw,  yaw_vec;
    float Theta1,  Theta1_Vec,  Theta2,  Theta2_Vec;
    float Theta3,  Theta3_Vec,  Theta4,  Theta4_Vec;
    float Theta5,  Theta5_Vec,  Theta6,  Theta6_Vec;
    quint32 Rob_FB1; quint32 Rob_FB2;

public slots:
    void handleData(const QByteArray &data);


    // void logData();

signals:
    void dataSend(float position, float speed, float torque, float current, float errorCode,
                  float position2, float speed2, float torque2, float current2, float errorCode2,
                  float Acc, quint32 Frame, quint16 Total_status, quint16 Total_error,
                  float X_position, float X_vec, quint32 X_Status,
                  float Y_position, float Y_vec, quint32 Y_Status,
                  float Z_position, float Z_vec, quint32 Z_Status,
                  float roll, float roll_vec, quint32 Roll_Status,
                  float pitch, float pitch_vec, quint32 Pitch_Status,
                  float yaw, float yaw_vec, quint32 yaw_Status,
                  float Theta1, float Theta1_Vec, float Theta2, float Theta2_Vec,
                  float Theta3, float Theta3_Vec, float Theta4, float Theta4_Vec,
                  float Theta5, float Theta5_Vec, float Theta6, float Theta6_Vec,
                  quint32 Rob_FB1, quint32 Rob_FB2);


private:

    // float lastPosition, lastSpeed, lastTorque, lastCurrent, lastErrorCode,
    //     lastPosition2, lastSpeed2, lastTorque2, lastCurrent2, lastErrorCode2, Acc;
};

#endif // RECEIVE_H
