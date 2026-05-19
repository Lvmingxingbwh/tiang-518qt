#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QUdpSocket>
#include <QtCharts>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QDebug>
#include "receive.h"
#include "qlog.h"
#include "qcustomplot.h"
// #include "autokeypress.h"
#include <QtConcurrent>
#include <QFileDialog>
#include "excelreader.h"
#include "rfm2g_api.h"
#include"rfm2g_windows.h"
#include<QVector>
#include "work.h"
#include<QProcess>
#include<QTimer>



// #include <cmath> // 引入标准数学库，用于生成正弦波

#include <QtMath>
class   Worker;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class login;
class self_test;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void SendCommand(const QByteArray &messageData);
    void handleAbsoluteMove(float position, float speed);
    void handleresoluteMove(float position, float speed);
    void handlePowerStateChange(bool powered);

public:
    void updatePowerState(bool isPowered);  // 添加更新电源状态的函数
    quint32 rfm2g_sendsingal;

    /////////////////auto////////////////////
    quint32 RFM_Com;
    float rfm_1,rfm_2,rfm_3,rfm_4,rfm_5;

     bool Auto_Com[32];
    void readMemoryData();
    void on_AUTO_pressed();
    void on_AUTO_released();
    void Auto();
    void AutoRest();

    void AutoSpeedM_F();
    void  AutoSpeedM_N();
    void RelativeCommand(const QVariant &param1, const QVariant &param2);
    void AbsoluteCommand(const QVariant &param1, const QVariant &param2);

    void AutoSin(const QVariant &param);
    void AutoStop();
    void powerOn();



    //////////////////////////////////////////
    float omga[12];
    void onReadingFinished();
    void handleNewSignal(float param1, float param2);

    void setBPower(bool value);
    bool getBPower() const; // 可选，提供读取 bPower 的接口


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateMotorParameters(float position, float speed, float torque, float current, quint32 errorCode,
                               float position2, float speed2, float torque2, float current2, quint32 errorCode2);
    void updateStatusIndicators(const quint16* totalStatus, const quint16* totalError);
    void updatePositionCharts(float X_position, float X_vec, quint32 X_Status,
                              float Y_position, float Y_vec, quint32 Y_Status,
                              float Z_position, float Z_vec, quint32 Z_Status,
                              float roll, float roll_vec, quint32 Roll_Status,
                              float pitch, float pitch_vec, quint32 Pitch_Status,
                              float yaw, float yaw_vec, quint32 yaw_Status,
                              float Theta1, float Theta1_Vec, float Theta2, float Theta2_Vec,
                              float Theta3, float Theta3_Vec, float Theta4, float Theta4_Vec,
                              float Theta5, float Theta5_Vec, float Theta6, float Theta6_Vec);
    /****************************************************************RFM2G*************************************************************/
    void writeFormattedDataToMemory(const QVector<QVariant>& dataToWrite);


QVector<QVariant> readFormattedDataFromMemory (const QVector<QString>& dataTypes);
QVector<QVariant> readFormattedDataFromMemory1(const QVector<QVariant>& dataTypes);








    void updateUI(
            float position, float speed, float torque, float current, quint32 errorCode,
                              float position2, float speed2, float torque2, float current2, quint32 errorCode2,
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
    void updateLogUI(const QString &logEntry);// 日志UI更新
    void updateMotorPosition(float position, float speed, float torque, float current, quint32 errorCode,
                             float position2, float speed2, float torque2, float current2, quint32 errorCode2,
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


    /*三个机械的errorCode全局变量*/
    quint32 errorCode_Shux;
    quint32 errorCode_Hengx;
    quint32 errorCode_Robot;

    bool System_ready = false;

    void Shux_Status();
    void Hengx_Status();
    void Robot_Status();
    int getErrorCode() const;
    enum Color { Red = 0, Green = 1, Grey = 3 };
    void udpconnect();
    // 存储最新的日志信息。
    QStringList logMessages;
    QFile *logFile;
    QStringList logList; // 用于存储最新的十条日志记录
    qlog *logger= nullptr;
    QThread *logThread= nullptr;


    float testPos;    // 测试位置
    float totalPosition; // 总位置
    float absPosition;   // 绝对位置
    float firstPostion;   // 超限第一次移动位置
    float remainPosition; // 剩余位置
    float targetPosition; // 目标位置（正弦运动）
    bool NoVec;
    //机械臂上电flag
    bool bPower3;
    double Torque;
    double Speed;
    // 日志单次保存最大10MB
    static const qint64 MaxLogFileSize = 10 * 1024 * 1024; // 10 MB

    bool updateEnsureUdp;
    bool areAllVecFieldsEmpty();

    //total_status转换
    quint8 totalStatusQuint16;

    void updateIndicator( quint16 totalStatus[16] );

    //total_error转换
    quint8 totalErrorQuint16;

    void updateerrostatus(quint16 totalError[16]);
    bool UDP_Connect;

    void clearOldDataBuffer();
    void processNewData(const QByteArray &datagram);
    void sendAbsoluteMoveCommand(float remotepostion, float remotespeed);

private slots:
    /*RFM2G*/
    void readDataFromRfm2g();  // 从反射内存读取数据
    //void writeDataToRfm2g();  // 向反射内存写入数据
    void clearRfm2gData();


    void Lock();                            // 屏蔽罩
    void unlock();

      void on_Lock_toggled(bool checked);
    // UI初始化槽函数
    void initializeUiComponents();           // 初始化UI组件槽函数
    void cleanupResources();                 // 清理资源槽函数
    void closePower();                       // 关闭电源槽函数
    // UDP连接槽函数
    void on_Connection_button_clicked();     // UDP连接按钮点击槽函数
    void on_Disconnection_button_clicked();  // UDP断开连接按钮点击槽函数
    void deleteUdpSocket();
    // 定时器初始化槽函数
    void InitTimer();
    // 数据接收发送
    ///////////////////////////////////////////////////////远程（读取内存卡数据自运行）/////////////////////////////////////////////////
    //void readXls();


    void processData();

    //初始化LED灯
    void setLedIndicator(QLabel* label, int color, int size);
    // 初始化创建线程
    void InitThread();

    //更新UI

    // 数据折线更新
    // void updateTorqueChart(float torque);
    // void updateSpeedChart(float speed);
    // void showPointToolTip(QMouseEvent *event);
    void initChart(QCustomPlot *customPlot, const QColor &color, double xRangeStart, double xRangeEnd, double yRangeStart, double yRangeEnd) ;
    void InitSpeedChart();
    void InitTorqueChart();
    void updateChart(QCustomPlot *customPlot, double value);
    void updateTorqueChart(double Torque);
    void updateSpeedChart(double Speed);
    // void showToolTip(QMouseEvent *event, QCustomPlot *customPlot, const QString &dataType);
    void showPointToolTip(QMouseEvent *event);
///////////////////////////////////////////////////////////////////////PLC控制///////////////////////////////////////////////////////////////////////

    // 竖向电机
    // PLC控制相关槽函数
    void on_btnPower_toggled(bool checked);              // 电源按钮状态改变槽函数
                        // 确保UDP连接槽函数
    void resetPowerButtonState(bool checked);            // 重置电源按钮状态槽函数                                    // 上电槽函数
    void powerOff();                                     // 下电槽函数
    void updateLedIndicators(Color powerLed); // 更新LED指示灯状态槽函数
    void on_btnJogForward_pressed();                     // Jog正向按钮按下槽函数
    void on_btnJogForward_released();                    // Jog正向按钮释放槽函数
    void on_JogBackward_pressed();                       // Jog反向按钮按下槽函数
    void on_JogBackward_released();                      // Jog反向按钮释放槽函数
    void on_btnStop_clicked();                           // 停止按钮点击槽函数
    void on_btnReset_clicked();                          // 复位按钮点击槽函数
    void on_btnRelativeMove_clicked();                   // 相对移动按钮点击槽函数
    void on_btnAbsoluteMove_clicked();                   // 绝对移动按钮点击槽函数

    // 旋转电机相关槽函数
    void on_btnPower_2_toggled(bool checked);            // 旋转电机电源按钮状态改变槽函数
    void on_btnStop_2_clicked();                         // 旋转电机停止按钮点击槽函数
    void on_btnReset_2_clicked();                        // 旋转电机复位按钮点击槽函数
    void on_btnRelativeMove_2_clicked();                 // 旋转电机相对移动按钮点击槽函数
    void on_btnAbsoluteMove_2_clicked();                 // 旋转电机绝对移动按钮点击槽函数
    void on_btnJogForward_2_pressed();                   // 旋转电机Jog正向按钮按下槽函数
    void on_btnJogForward_2_released();                  // 旋转电机Jog正向按钮释放槽函数
    void on_JogBackward_2_pressed();                     // 旋转电机Jog反向按钮按下槽函数
    void on_JogBackward_2_released();                    // 旋转电机Jog反向按钮释放槽函数
    void on_Systemresetbutton_2_pressed();               // 旋转电机系统复位按钮按下槽函数
    void on_Select_log_clicked();                        // 选择日志按钮点击槽函数
    void on_Log_control_toggled(bool checked);           // 日志控制按钮状态改变槽函数
    void on_pushButton_clicked();                        // Push按钮点击槽函数
    void on_Systemresetbutton_clicked();                 // 系统复位按钮点击槽函数

    // 测试相关槽函数
    void VeloTest();

    void InitTableWidget();


    void on_TestBtn_toggled(bool checked);               // 测试按钮状态改变槽函数
    void sendTestCommand();                              // 发送测试命令槽函数
    void checkPosition();                                // 检查位置槽函数

    void on_SinusOscillation_clicked();



    void on_EndLessBtn_clicked();

    void on_Positive_Direction_clicked();
//----------------------------------robot---------------------------------------------//
    void on_robot_poweron_clicked();

    void on_robot_poweroff_clicked();

    //void on_robot_zero_2_clicked();

    // void on_robot_jpmove_clicked();

    void on_robot_Cartesianmove_clicked();

    //void on_robot_follow_clicked();

    //void on_robot_follow_2_clicked();

    void on_robot_stop_pressed();

    void on_robot_stop_released();

    void on_robot_zero_2_pressed();

    void on_robot_zero_2_released();

    void on_robot_jpmove_pressed();

    void on_robot_jpmove_released();

    //void on_robot_follow_pressed();

    //void on_robot_follow_released();

    //void on_robot_follow_2_pressed();

   //void on_robot_follow_2_released();



    void on_alarm_reset_clicked();



    void on_end_code_clicked();

    void on_robot_follow_clicked(bool checked);

    void on_robot_follow_2_clicked(bool checked);



    void on_H_btnRelativeMove_clicked();

    void on_btnAbsoluteMove_11_clicked();



    void on_Back_zero_clicked();



    void on_Shux_Stop_clicked();

    void on_Shux_Reset_clicked();





    void on_btnRelativeMove_11_clicked();

    void on_btnAbsoluteMove_10_clicked();

    void on_Hengx_Stop_clicked();

    void on_Hengx_Stop_2_clicked();

    void on_Hengx_zero_clicked();


    void selfTest();

    void on_Robot_power_toggled(bool checked);



    void on_robot_zero_10_pressed();

    void on_robot_zero_10_released();

    void on_robot_stop_5_pressed();

    void on_robot_stop_5_released();


    void on_end_code_5_clicked();

    void on_alarm_reset_5_clicked();

    void on_robot_jpmove_5_clicked();



    void on_robot_Cartesianmove_5_clicked();

    void on_resetSelfTest_clicked();

    void on_pushButton_2_clicked();

    void on_readButton_clicked();

    void on_writeButton_clicked();

    void on_clearButton_clicked();

    void on_MainRfmRead_clicked();

    void on_WRITECOM_clicked();

   // void on_readCom_clicked();

    void checkPLCConnection();
    void setupUDPConnection();


    //失效目标单轴控制
    void on_Robot_X_clicked();
    void on_Robot_Y_clicked();



    void on_Robot_Z_clicked();

    void on_Robot_Rx_clicked();

    void on_Robot_Ry_clicked();

    void on_Robot_Rz_clicked();

    void on_Shux_Mon_Stop_clicked();

    void on_Shu_Mon_Stop1_clicked();

signals:
    void dataReceived(const QByteArray &data); // 用于发送数据的信号
    void closed();
    void motorDataProcessed(float position, float speed, float torque, float current, quint32 errorCode,
                            float position2, float speed2, float torque2, float current2, quint32 errorCode2,
                            const quint16* totalStatus, const quint16* totalError, float Acc, quint32 Frame,
                            float X_position, float X_vec, quint32 X_Status, float Y_position, float Y_vec, quint32 Y_Status,
                            float Z_position, float Z_vec, quint32 Z_Status, float roll, float roll_vec, quint32 Roll_Status,
                            float pitch, float pitch_vec, quint32 Pitch_Status, float yaw, float yaw_vec, quint32 yaw_Status,
                            float Theta1, float Theta1_Vec, float Theta2, float Theta2_Vec, float Theta3, float Theta3_Vec,
                            float Theta4, float Theta4_Vec, float Theta5, float Theta5_Vec, float Theta6, float Theta6_Vec);


    void bPowerChanged(bool value);



private:
    /*自检参数*/

    int step;        // 跟踪自检步骤
    int retryCount;  // 重试次数
    bool allPassed;  // 自检是否成功
    QTimer *autoReadTimer;

    class Auto *autoControl;
    QTimer *selfTestTimer1;
    QTimer *selfTestTimer2;
    QTimer *selfTestTimer3;
    QTimer *remoteTimer;
    //QVector<double> timeData;
   // QVector<double> valueData;

    QList<QLabel*> labels1;
    QList<QLabel*> labels2;
    Ui::MainWindow *ui;                 // 主窗口UI指针

    QWidget *overlay;                   // 遮罩层
    qlog *qlogInstance;

    Worker *worker;


    Receive *receiver;                  // 数据接收实例指针
    QThread *receiverThread;
    QThread *autoThread;
    QThread *workerThread;
        // 数据接收线程指针
    ExcelReader *excelreader;
    QThread *excelreaderThread;
    //远程线程
     // AutoKeyPressThread *autoKeyPressThread;

    // UDP通讯相关
    QUdpSocket *udpSocket = nullptr;    // UDP套接字指针
    QString ip;                         // UDP连接IP地址
    quint16 port;                       // UDP连接端口号

    // 默认IP和端口
    QString defaultIp;                  // 默认 AutoKeyPressThread *autoKeyPressThread; UDP连接IP地址
    quint16 defaultPort;                // 默认UDP连接端口号

    // 时间和数据缓冲
    qint64 startTime;                   // 记录数据起始时间
    QVector<float> dataBuffer;         // 数据缓冲区
    QCPGraph *averageLine;              // 平均值线图指针

    // 定时器
    QTimer *logTimer;                   // 用于记录数据的定时器
    QTimer *displayTimer;               // 用于更新显示的定时器

    // 用户输入和定时器成员变量声明
    float AbsoluteSpeed;           // 用户绝对速度


    // 上电确认
    bool bPower = false;                // 电机1上电状态
    bool bPower2 = false;               // 电机2上电状态
    bool logger_power = false;          // 控制日志显示

    // 日志更新使能
    bool logUpdateEnabled = false;      // 日志更新开关

    // 相对移动
    float initialPosition;             // 初始位置

    // 测试功能
    QTimer *positionCheckTimer;         // 位置检查定时器
    int counter = 0;                    // 计数器
    float testPosition;                // 测试位置
    float testSpeed = 20;              // 测试速度
    float beforePos;                   // 前一位置
    /*RFM2G*/

    QPushButton *readButton;   // 读取按钮
        QPushButton *writeButton;  // 写入按钮
        QLineEdit *readLineEdit;   // 显示读取数据的文本框
        QLineEdit *writeLineEdit;  // 输入写入数据的文本框

        RFM2GHANDLE handle;        // RFM2G 句柄
        const RFM2G_UINT32 baseAddress = 0x4002000;  // 基础地址
        const size_t dataSize = 18 * sizeof(float);  // 数据大小（18 个 float）

        void initializeRfm2g();    // 初始化 RFM2G


public slots:
     bool ensureUdpConnection();
    bool areAllVecFieldsEmpty_2();


};
#endif // MAINWINDOW_H
