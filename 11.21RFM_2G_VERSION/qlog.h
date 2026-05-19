#ifndef QLOG_H
#define QLOG_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QScopedPointer>

class qlog : public QObject
{
    Q_OBJECT
public:
    explicit qlog(QObject *parent = nullptr);
    ~qlog();

    // 设置日志文件路径
    void setLogFilePath(const QString &newPath);

    // 更新数据
    void updateData(float position, float speed, float torque, float current, float errorCode,
                    float position2, float speed2, float torque2, float current2, float errorCode2,
                    float Acc, float Frame, float Total_status, float Total_error,
                    float X_position, float X_vec, float X_Status,
                    float Y_position, float Y_vec, float Y_Status,
                    float Z_position, float Z_vec, float Z_Status,
                    float roll, float roll_vec, float Roll_Status,
                    float pitch, float pitch_vec, float Pitch_Status,
                    float yaw, float yaw_vec, float yaw_Status,
                    float Theta1, float Theta1_Vec, float Theta2, float Theta2_Vec,
                    float Theta3, float Theta3_Vec, float Theta4, float Theta4_Vec,
                    float Theta5, float Theta5_Vec, float Theta6, float Theta6_Vec,
                    float Rob_FB1, float Rob_FB2);

    // 开始监控 errorCode
    void startMonitoringErrorCode();

    // 获取当前 errorCode
    int getErro_Shux() const;

signals:
    // 当 errorCode 改变时发出信号
    void errorCodeChanged(int newErrorCode);

    // 监控结束信号，如果 errorCode 为 0，则发出 true
    void monitoringFinished(bool success);
    void logUpdated(const QString &logEntry);


private:
    QScopedPointer<QFile> logFile;
    QTimer *logTimer;     // 日志定时器
    QTimer *monitorTimer; // 用于监控 errorCode 的定时器
    QString logFilePath;
    QString fileName;
    int errorCode = 0; // 假设默认 errorCode 为 0
    QDateTime lastUpdateTime;
    int logInterval = 1000; // 日志记录间隔（假设值为 1000 毫秒）
    QStringList logBuffer;  // 用于缓存日志数据

    // 初始化日志文件
    void initLog();
public:
    // 检查 errorCode 并发出信号

 void checkErrorCode();
    // 记录日志数据
    void logData();
};

#endif // QLOG_H
