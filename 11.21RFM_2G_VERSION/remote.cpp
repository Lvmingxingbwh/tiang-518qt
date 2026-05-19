#include "remote.h"
#include "ui_remote.h"

// 构造函数
Remote::Remote(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Remote)
{
    ui->setupUi(this);  // 设置UI
    initializeUiComponents();  // 初始化UI组件
    simulateAutomaticOperation();  // 模拟自动操作
}

// 析构函数
Remote::~Remote()
{
    delete ui;
    // 确保线程已经完全停止后再删除
    if (receiverThread)
    {
        receiverThread->quit();
        receiverThread->wait();
        delete receiverThread;
    }

    if (logThread)
    {
        logThread->quit();
        logThread->wait();
        delete logThread;
    }

    if (udpSocket != nullptr)
    {
        udpSocket->close();  // 关闭UDP套接字
    }
}

// 界面初始化函数
void Remote::initializeUiComponents()
{
    // 设置默认IP和Port
    defaultIp = "127.0.0.1";
    defaultPort = 5080;
    ui->IP_lineEdit->setText(defaultIp);  // 设置IP显示
    ui->Port_lineEdit->setText(QString::number(defaultPort));  // 设置端口显示

    InitThread();       // 初始化线程
    InitTorqueChart();  // 初始化扭矩图表
    InitSpeedChart();   // 初始化速度图表

    // 连接图表的鼠标移动事件，以显示提示工具
    connect(ui->Speed, &QCustomPlot::mouseMove, this, &Remote::showPointToolTip);
    connect(ui->Torque, &QCustomPlot::mouseMove, this, &Remote::showPointToolTip);
}

// 将label控件变成圆形指示灯
void Remote::setLedIndicator(QLabel* label, int color, int size)
{
    label->setText("");  // 清空label中的文字

    // 设置矩形大小，根据UI设置的大小调整
    QString min_width = QString("min-width: %1px;").arg(size);
    QString min_height = QString("min-height: %1px;").arg(size);
    QString max_width = QString("max-width: %1px;").arg(size);
    QString max_height = QString("max-height: %1px;").arg(size);

    // 设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size / 2);
    QString border = "border:1px solid black;";

    // 设置背景颜色
    QString background = "background-color:";
    switch (color) {
    case 0:
        background += "rgb(255,0,0)";  // 红色
        break;
    case 1:
        background += "rgb(0,255,0)";  // 绿色
        break;
    case 3:
        background += "rgb(190,190,190)";  // 灰色
        break;
    default:
        break;
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);  // 应用样式
}


// 初始化接收线程
void Remote::InitThread()
{
    // 创建接收子线程
    receiverThread = new QThread(this);
    receiver = new Receive();
    receiver->moveToThread(receiverThread);

    // 创建日志子线程
    logThread = new QThread(this);
    logger = new qlog();
    logger->moveToThread(logThread);

    // 信号与槽的连接
    connect(this, &Remote::dataReceived, receiver, &Receive::handleData); // 将接收到的数据发送给receiver处理
    connect(receiver, &Receive::dataSend, this, &Remote::updateUI);       // 用于更新UI界面的实时位置，速度，扭矩，电流，错误码
    connect(receiver, &Receive::dataSend, logger, &qlog::updateData);    // 将接收到的数据发送给日志记录器处理
    connect(logger, &qlog::logUpdated, this, &Remote::updateLogUI);      // 更新日志界面
}

// UDP开始连接
void Remote::on_Connect_Button_clicked()
{
    ip = ui->IP_lineEdit->text().trimmed();  // 获取输入的IP
    port = ui->Port_lineEdit->text().toInt();  // 获取输入的端口

    // 检查IP和端口有效性
    if (!QHostAddress(ip).isNull() && port > 0 && port < 65535)
    {
        if (udpSocket)
        {
            udpSocket->deleteLater();
            udpSocket = nullptr;
        }

        udpSocket = new QUdpSocket(this);
        connect(udpSocket, &QUdpSocket::readyRead, this, &Remote::processData); // 准备读取数据
        receiverThread->start();  // 启动接收线程
        logThread->start();
        if (!udpSocket->bind(QHostAddress::Any, port))
        {
            QMessageBox::critical(this, tr("错误"), tr("绑定端口失败: ") + udpSocket->errorString());
            return;
        }

        QMessageBox::information(this, tr("连接状态"), tr("连接成功"));
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入有效的IP地址和端口号。"));
    }
}

// UDP断开连接
void Remote::on_Disconnect_Button_clicked()
{
    if (udpSocket != nullptr)
    {
        udpSocket->close();
        udpSocket->deleteLater();
        udpSocket = nullptr;

        QMessageBox::information(this, tr("连接状态"), tr("已断开连接"));
    }
}

//数据处理
void Remote::processData()
{
    // 检查是否有待处理的数据报
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), nullptr, nullptr);

        // 发出信号，传递接收到的数据
        emit dataReceived(datagram);
    }
}

// 数据发送
void Remote::SendCommand(const QByteArray &command)
{
    if (command.isEmpty())
    {
        qDebug() << "尝试发送空命令";
        return;
    }

    if (udpSocket != nullptr && udpSocket->state() == QAbstractSocket::BoundState)
    {
        QHostAddress address(ip);
        qint64 bytesSent = udpSocket->writeDatagram(command.data(), command.size(), address, port);

        if (bytesSent == -1)
        {
            QMessageBox::critical(this, tr("错误"), tr("发送命令失败: ") + udpSocket->errorString());
        }
    }

}

//开始自行运行开关
void Remote::on_StartButton_toggled(bool checked)
{
    if (checked) {
        // 按钮被按下，开始自动化操作

        // 首先检查UDP连接是否已经建立
        if (udpSocket && udpSocket->state() == QAbstractSocket::BoundState)
        {
            if (!timer) {
                timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, &Remote::simulateButtonPressAndRelease);
            }
            // ui->StartButton->setText(tr("开始"));
            timer->start(10);  // 每秒触发一次
        } else
        {
            QMessageBox::warning(this, tr("警告"), tr("UDP连接未建立。"));
            // 如果UDP未连接，重置按钮状态为未选中
            QAbstractButton *button = qobject_cast<QAbstractButton *>(sender());
            if (button)
            {
                button->setChecked(false);
            }
        }
    } else
    {
        // 按钮被释放，停止自动化操作
        if (timer)
        {
            timer->stop();
        }
        // ui->StartButton->setText(tr("结束"));
    }
}

// 定时运行定时器
void Remote::simulateAutomaticOperation() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Remote::simulateButtonPressAndRelease);
    // timer->start(1000);  // 设置定时器周期为1000毫秒（1秒）
}

//模拟开关按下
void Remote::simulateButtonPressAndRelease()
{
    // 模拟按钮按下的操作
    // 这里调用按钮按下时应该执行的函数
    // 例如，我们模拟按下电机控制按钮
    qDebug() << "模拟按钮按下";
    btnRelativeMove_pressed();

    // 等待一定时间，模拟用户按住按钮的时间
    QTimer::singleShot(100, this, &Remote::btnRelativeMove_released);

    // 模拟按钮释放的操作
    // 这里调用按钮释放时应该执行的函数
    qDebug() << "模拟按钮释放";
    btnRelativeMove_released();
}

//开关按下
void Remote::btnRelativeMove_pressed()
{
    // 这里是按钮按下时的逻辑
    qDebug() << "执行电机相对移动开始逻辑";

    // 这里可以添加发送特定命令到电机的逻辑，例如：
    // QByteArray messageData = ...; // 创建适当的消息数据
    // SendCommand(messageData);
}

//开关释放
void Remote::btnRelativeMove_released()
{
    // 这里是按钮释放时的逻辑
    qDebug() << "执行电机相对移动结束逻辑";

    // 这里可以添加发送特定命令到电机的逻辑，例如：
    QByteArray messageData = QByteArray::fromHex("07");
    SendCommand(messageData);
}















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 实现主界面的UI更新
void Remote::updateUI(double position,  double speed,  double torque,  double current,  int errorCode)
{
    // 更新电机实时参数
    updateMotorPosition(position, speed, torque, current, errorCode);
    // 更新图表
    updateTorqueChart(torque);
    updateSpeedChart(speed);
}

//更新 PLC 端电机参数
void Remote::updateMotorPosition(double position, double speed, double torque, double current, int errorCode)
{
    // 假设torque是当前扭矩的百分比
    double maxTorque = 73.0; // 最大扭矩值
    double actualTorque = maxTorque * torque / 100.0; // 计算实际扭矩值
    // 更新竖向电机参数显示
    ui->Position_lineEdit->setText(QString::number(position, 'f', 2));
    ui->Speed_lineEdit->setText(QString::number(speed, 'f', 2));
    ui->Torque_lineEdit->setText(QString::number(actualTorque, 'f', 2));
    ui->Current_lineEdit->setText(QString::number(current, 'f', 2));
    ui->Error_lineEdit->setText(QString::number(errorCode));

}

//日志显示功能
void Remote::updateLogUI(const QString &logEntry)
{
    logList.append(logEntry);
    while (logList.size() > 20) {
        logList.removeFirst();
    }
    if(logger_power)
    {
        ui->logText->setPlainText(logList.join("\n"));
    }
    // 确保更新滚动条位置
    QTextCursor cursor = ui->logText->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logText->setTextCursor(cursor);
}

//日志保存按钮
void Remote::on_LogSalve_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, tr("选择日志文件保存路径"),
        "", // 默认路径，也可以是上次选择的路径或常用路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 仅当用户选择了目录时才更新路径
    if (!dir.isEmpty()) {
        logger->setLogFilePath(dir);
    }
}

//日志清除按钮
void Remote::on_LogClear_clicked()
{
    ui->logText->clear();
}

//日志控制
void Remote::on_LogControl_toggled(bool checked)
{
    if (checked)
    {
        ui->LogControl->setText(tr("开始"));  // 设置按钮文本为“显示”
        logger_power=true; // 显示日志控件
        qDebug() << "按下";
    } else
    {
        ui->LogControl->setText(tr("暂停"));  // 设置按钮文本为“隐藏”
        logger_power=false; // 隐藏日志控件
        qDebug() << "释放";

    }
}

// 初始化图表的通用函数
void Remote::initChart(QCustomPlot *customPlot, const QColor &color, double xRangeStart,
                       double xRangeEnd, double yRangeStart, double yRangeEnd)
{
    customPlot->addGraph(); // 添加一个图表
    customPlot->graph(0)->setPen(QPen(color)); // 设置图表颜色

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss:zzz"); // 设置x轴时间显示格式
    customPlot->xAxis->setTicker(dateTicker);

    customPlot->xAxis->setRange(xRangeStart, xRangeEnd); // 设置x轴范围
    customPlot->yAxis->setRange(yRangeStart, yRangeEnd); // 设置y轴范围
    customPlot->setMouseTracking(true); // 启用鼠标跟踪
}

// 更新图表数据的通用函数
void Remote::updateChart(QCustomPlot *customPlot, double value)
{
    double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0; // 获取当前时间戳（秒）
    customPlot->graph(0)->addData(currentTime, value); // 使用时间戳添加数据点

    double window = 0.50; // 定义时间窗口（秒）

    // 移除超出时间窗口的旧数据点（如果您想限制数据点数量）
    customPlot->graph(0)->data()->removeBefore(currentTime - window);

    customPlot->xAxis->setRange(currentTime - window, currentTime); // 更新x轴范围以只显示最新的数据点

    customPlot->replot(); // 重绘图表
}

// 初始化速度图表
void Remote::InitSpeedChart()
{
    initChart(ui->Speed, Qt::red, 0, 200, -100, 100); // 使用红色初始化速度图表
}

// 初始化扭矩图表
void Remote::InitTorqueChart()
{
    initChart(ui->Torque, Qt::blue, 0, 200, -100, 100); // 使用蓝色初始化扭矩图表
}

// 更新速度图表
void Remote::updateSpeedChart(double speed)
{
    updateChart(ui->Speed, speed); // 使用新的速度值更新图表
}

// 更新扭矩图表
void Remote::updateTorqueChart(double torque)
{
    updateChart(ui->Torque, torque); // 使用新的扭矩值更新图表
}

// 显示点的提示工具函数
void Remote::showPointToolTip(QMouseEvent *event) {
    QCustomPlot *customPlot = qobject_cast<QCustomPlot *>(sender()); // 获取触发事件的图表
    if (!customPlot) return; // 如果不是图表，直接返回

    double x = customPlot->xAxis->pixelToCoord(event->pos().x()); // 将像素坐标转换为图表的x坐标
    double y = customPlot->yAxis->pixelToCoord(event->pos().y()); // 将像素坐标转换为图表的y坐标

    double minDistanceSquared = std::numeric_limits<double>::max(); // 初始化最小距离
    QString tooltipText;
    for (int i = 0; i < customPlot->graph(0)->data()->size(); ++i) {
        double dataX = customPlot->graph(0)->data()->at(i)->key; // 获取数据点的x坐标
        double dataY = customPlot->graph(0)->data()->at(i)->value; // 获取数据点的y坐标
        double distanceSquared = qPow(dataX - x, 2) + qPow(dataY - y, 2); // 计算当前点到鼠标位置的距离平方

        if (distanceSquared < minDistanceSquared) {
            minDistanceSquared = distanceSquared; // 更新最小距离平方
            QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(dataX * 1000)); // 将毫秒时间戳转换为QDateTime
            tooltipText = QString("Time: %1\nValue: %2").arg(dateTime.toString("hh:mm:ss.zzz")).arg(dataY); // 准备显示的提示文本
        }
    }
    double thresholdSquared = 0.1; // 设置显示提示的距离阈值
    if (minDistanceSquared < thresholdSquared) {
        QToolTip::showText(event->globalPos(), tooltipText, customPlot);
        // QToolTip::showText(event->globalPosition().toPoint(), tooltipText, customPlot); // 如果当前点与鼠标的距离小于阈值，则显示提示
    }
}


void Remote::cleanupResources()
{
    // // 关闭并重置指示灯状态
    // setLedIndicator(ui->label_led0, Grey, 48);
    // setLedIndicator(ui->label_led1, Grey, 48);
    // setLedIndicator(ui->signal, Grey, 48);

    // 如果接收线程存在，则安全地停止并删除它
    if (receiverThread)
    {
        receiverThread->quit();
        receiverThread->wait();
        delete receiverThread;
        receiverThread = nullptr;
    }

    // 如果日志线程存在，则安全地停止并删除它
    if (logThread)
    {
        logThread->quit();
        logThread->wait();
        delete logThread;
        logThread = nullptr;
    }

    // 如果UDP套接字已创建，则关闭它
    if (udpSocket != nullptr)
    {
        udpSocket->close();
    }
}

void Remote::on_pushButton_clicked()
{

    cleanupResources();  // 清理资源
    emit closed();
    // this->close();  // 关闭当前窗口

}

