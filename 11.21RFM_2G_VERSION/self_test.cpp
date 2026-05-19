#include "self_test.h"
#include "ui_self_test.h"
#include "receive.h"

Self_Test::Self_Test(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::Self_Test),
    timer(new QTimer(this)),
    step(0),
    retryCount(0),  // 初始化 retryCount
    maxRetries(3),  // 设置最大重试次数
    mainWindow(mainWindow),
    QlogObj(new qlog(this))
{
    ui->setupUi(this);
    connect(timer, &QTimer::timeout, this, &Self_Test::updateSelfTestInfo);
   // connect(QlogObj, &qlog::errorCodeChanged, this, &Self_Test::onErrorCodeChanged);
    connect(QlogObj, &qlog::monitoringFinished, this, &Self_Test::handleMonitoringResult);

    udpSocket = new QUdpSocket(this);
    ui->zijian->setReadOnly(true);

    startSelfTest();  // 启动自检
}

Self_Test::~Self_Test()
{
    delete ui;
}

void Self_Test::startSelfTest()
{
    step = 0;
    retryCount = 0;  // 重置重试计数
    timer->start(3000);  // 每 3 秒触发一次
}

// void Self_Test::onErrorCodeChanged(int newErrorCode)
// {
//     // 更新 errorCode 的显示逻辑
//     if (newErrorCode == 0) {
//         ui->zijian->append("竖向平台自检成功...");
//     } else {
//         ui->zijian->append("竖向平台自检失败，错误代码: " + QString::number(newErrorCode));
//     }
// }

void Self_Test::handleMonitoringResult(bool success)
{
    if (success) {
        // 检测到 errorCode 为 0，自检成功
         Shux_zijian = true;
    } else {
        // 自检失败
        retryCount++;
        if (retryCount < maxRetries) {
            ui->zijian->append("重试自检...");
            QlogObj->startMonitoringErrorCode();  // 重新启动监控
        } else {
            ui->zijian->append("自检失败，已达到最大重试次数...");
            Shux_zijian=false;
        }
    }
}


void Self_Test::updateSelfTestInfo()
{
    switch (step) {
    case 0:
        UDP_Send(0);
        ui->zijian->setText("竖向平台自检中...");
        QlogObj->startMonitoringErrorCode();  // 开始监控 errorCode
        step++;  // 移动到下一个步骤
        break;

    case 1:
        if(Shux_zijian ==true)
        {
            ui->zijian->append("竖向平台自检成功...");
            step++;  // 进入下一步
        }
        else
        {
            ui->zijian->append("竖向平台自检失败...");
            step++;  // 进入下一步
        }
        // if (QlogObj->getErro_Shux() == 0) {
        //     ui->zijian->append("竖向平台自检成功...");
        //     step++;  // 进入下一步
        // } else {
        //     if (retryCount == 0) {
        //         ui->zijian->append("竖向平台自检失败，正在重新检测...");
        //         retryCount++;  // 增加重试计数
        //     } else {
        //         ui->zijian->append("竖向平台自检失败！错误代码: " + QString::number(QlogObj->getErro_Shux()));
        //         step++;  // 进入下一步，不再重试
        //         retryCount = 0;  // 重置重试计数
        //     }
        // }
        break;

    case 2:
        ui->zijian->append("机械臂自检中...");
        step++;  // 继续下一步
        break;

    case 3:
        ui->zijian->append("机械臂自检成功...");
        timer->stop();  // 自检完成，停止定时器
        break;

    default:
        ui->zijian->setText("系统启动失败！");
        timer->stop();  // 发生错误时停止定时器
        break;
    }
}

void Self_Test::UDP_Send(int index)
{
    quint16 port = 5060;
    QHostAddress IP("127.0.0.1");  // 目标 IP 地址

    QByteArray sendmessage;
    sendmessage.append(static_cast<char>(0x00));
    sendmessage.append(static_cast<char>(0x01));
    sendmessage.append(static_cast<char>(0x1F));
    sendmessage.append(static_cast<char>(0x20));
    sendmessage.append(static_cast<char>(0x37));
    sendmessage.append(static_cast<char>(0x38));

    if (index >= 0 && index < sendmessage.size()) {
        QByteArray senddata;
        senddata.append(sendmessage[index]);
        udpSocket->writeDatagram(senddata, IP, port);
    } else {
        qDebug() << "Index out of range!";
    }
}
