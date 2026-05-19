#include "qlog.h"

qlog::qlog(QObject *parent)
    : QObject{parent}
    , logTimer(new QTimer(this))
    , monitorTimer(new QTimer(this)) // 新增的监控定时器
    , logFile(new QFile())
{
    connect(logTimer, &QTimer::timeout, this, &qlog::logData);  // 将计时器的超时信号连接到日志记录函数

    logTimer->setTimerType(Qt::PreciseTimer);
    logTimer->start(logInterval);

    monitorTimer->setSingleShot(true);
    connect(monitorTimer, &QTimer::timeout, this, &qlog::checkErrorCode);

    // 设置日志文件的默认保存路径
    if (logFilePath.isEmpty()) {
        setLogFilePath("E:/Data");
    }
}

// 析构函数，负责关闭文件和计时器
qlog::~qlog()
{
    if (logFile && logFile->isOpen()) {
        logFile->close();
    }

    if (logTimer) {
        logTimer->stop();
    }

    if (monitorTimer) {
        monitorTimer->stop();
    }
}

// 设置日志文件路径
void qlog::setLogFilePath(const QString &newPath) {
    QString effectivePath = newPath.isEmpty() ? "E:/Data" : newPath;  // 如果没有指定新路径，则使用默认路径

    // 创建路径
    if (!QDir(effectivePath).exists()) {
        QDir().mkpath(effectivePath);
    }

    logFilePath = effectivePath;
    initLog();  // 初始化日志文件
}

// 初始化日志文件
void qlog::initLog()
{
    QString path = logFilePath.isEmpty() ? "E:/Data" : logFilePath;  // 使用已设置的路径或默认路径
    QDir().mkpath(path);  // 确保路径存在

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmsszzz");
    fileName = QString("%1/PLCDataLog_%2.csv").arg(path, timestamp);
    logFile.reset(new QFile(fileName));  // 创建并打开新的日志文件

    if (logFile->open(QFile::WriteOnly | QFile::Append))
    {
        QTextStream stream(logFile.data());
        stream << "Timestamp, position, speed, torque, current, errorCode, position2, speed2, torque2, current2, errorCode2\n";
    }
}

// 更新数据
void qlog::updateData(float position, float speed, float torque, float current, float errorCode,
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
                      float Rob_FB1, float Rob_FB2)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    if (lastUpdateTime.isNull() || lastUpdateTime.msecsTo(currentTime) >= logInterval)
    {
        lastUpdateTime = currentTime;

        QString timestamp = currentTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
        QString logEntry = QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27, %28, %29, %30, %31, %32, %33, %34, %35, %36, %37, %38, %39, %40, %41, %42, %43, %44, %45, %46, %47, %48, %49, %50, %51, %52, %53, %54, %55, %56, %57, %58\n")
                               .arg(timestamp)
                               .arg(position, 0, 'f', 2)
                               .arg(speed, 0, 'f', 2)
                               .arg(torque, 0, 'f', 2)
                               .arg(current, 0, 'f', 2)
                               .arg(errorCode)
                               .arg(position2, 0, 'f', 2)
                               .arg(speed2, 0, 'f', 2)
                               .arg(torque2, 0, 'f', 2)
                               .arg(current2, 0, 'f', 2)
                               .arg(errorCode2)
                               .arg(Acc, 0, 'f', 2)
                               .arg(Frame, 0, 'f', 2)
                               .arg(Total_status, 0, 'f', 2)
                               .arg(Total_error, 0, 'f', 2)
                               .arg(X_position, 0, 'f', 2)
                               .arg(X_vec, 0, 'f', 2)
                               .arg(X_Status, 0, 'f', 2)
                               .arg(Y_position, 0, 'f', 2)
                               .arg(Y_vec, 0, 'f', 2)
                               .arg(Y_Status, 0, 'f', 2)
                               .arg(Z_position, 0, 'f', 2)
                               .arg(Z_vec, 0, 'f', 2)
                               .arg(Z_Status, 0, 'f', 2)
                               .arg(roll, 0, 'f', 2)
                               .arg(roll_vec, 0, 'f', 2)
                               .arg(Roll_Status, 0, 'f', 2)
                               .arg(pitch, 0, 'f', 2)
                               .arg(pitch_vec, 0, 'f', 2)
                               .arg(Pitch_Status, 0, 'f', 2)
                               .arg(yaw, 0, 'f', 2)
                               .arg(yaw_vec, 0, 'f', 2)
                               .arg(yaw_Status, 0, 'f', 2)
                               .arg(Theta1, 0, 'f', 2)
                               .arg(Theta1_Vec, 0, 'f', 2)
                               .arg(Theta2, 0, 'f', 2)
                               .arg(Theta2_Vec, 0, 'f', 2)
                               .arg(Theta3, 0, 'f', 2)
                               .arg(Theta3_Vec, 0, 'f', 2)
                               .arg(Theta4, 0, 'f', 2)
                               .arg(Theta4_Vec, 0, 'f', 2)
                               .arg(Theta5, 0, 'f', 2)
                               .arg(Theta5_Vec, 0, 'f', 2)
                               .arg(Theta6, 0, 'f', 2)
                               .arg(Theta6_Vec, 0, 'f', 2)
                               .arg(Rob_FB1, 0, 'f', 2)
                               .arg(Rob_FB2, 0, 'f', 2);

        logBuffer.append(logEntry);
    }
}

void qlog::logData()
{
    if (!logFile->isOpen() || logBuffer.isEmpty()) {
        return;
    }
    QTextStream stream(logFile.data());
    const QString &logEntry = logBuffer.last();
    stream << logEntry;
    emit logUpdated(logEntry);
    logBuffer.clear();  // 清空缓冲区
}

// 获取当前 errorCode
int qlog::getErro_Shux() const
{
    return errorCode;
}

// 开始监控 errorCode
void qlog::startMonitoringErrorCode()
{
    // 启动2秒的监控
    monitorTimer->start(2000);  // 2秒后触发 timeout 信号
}

// 定时器超时后检查 errorCode
void qlog::checkErrorCode()
{
    // 检查 errorCode 是否为0
    if (errorCode == 0) {
        emit monitoringFinished(true);  // 发出 True 信号
    } else {
        emit monitoringFinished(false);  // 发出 False 信号
    }
}
