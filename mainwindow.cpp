#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAxObject>
#include "auto.h"

// 构造函数，初始化用户界面
#define DEVICE "\\\\.\\rfm2g1"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow),
    selfTestTimer1(nullptr),
    selfTestTimer2(nullptr),
    selfTestTimer3(nullptr)
{
    ui->setupUi(this);          // 设置用户界面
    initializeUiComponents();   // 初始化界面组件
    InitTableWidget();
    InitThread();       // 初始化处理数据的线程
    InitTorqueChart();  // 初始化扭矩图表
    InitSpeedChart();   // 初始化速度图表
    InitTimer();         // 位置检查定时器

    step = 0;
    retryCount = 0;
    allPassed = true;

    //connect(this, &MainWindow::motorDataProcessed, this, &MainWindow::onMotorDataProcessed);
    //connect(excelreader, &ExcelReader::triggerPowerButton, this, [this]() {
       // on_btnPower_toggled(true);  // 或者 false，根据你的需求
    //});
    //connect(excelreader, &ExcelReader::triggerNewSignal, this, &MainWindow::handleNewSignal);
    // 连接 ExcelReader 的文件变化信号


        // 连接UI按钮与Auto类中的startAuto和stopAuto方法
//        connect(ui->auto_2, &QPushButton::clicked, this, &MainWindow::on_auto_2_clicked);




    labels2 = {
        ui->label3_er_1, ui->label3_er_2, ui->label3_er_3, ui->label3_er_4,
        ui->label3_er_5, ui->label3_er_6, ui->label3_er_7, ui->label3_er_8,
        ui->label3_er_17, ui->label3_er_18, ui->label3_er_19, ui->label3_er_20,
        ui->label3_er_21, ui->label3_er_22, ui->label3_er_23, ui->label3_er_24};


     /*---------------------------------------- 创建和启动自动按键线程----------------------------------*/
    //autoKeyPressThread = new AutoKeyPressThread(this);
   // autoKeyPressThread->start();
    /*------------------------------------------------------------------------------------------------*/

    // 设置解锁快捷键
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(ui->Arm_Widget);
    setLayout(layout);

    QShortcut *unlockShortcut = new QShortcut(QKeySequence("Ctrl+L"), this);
    connect(unlockShortcut, &QShortcut::activated, this, &MainWindow::unlock);
}

// 析构函数，负责资源的清理
MainWindow::~MainWindow()
{
    // 关闭指示灯，设置
    // setLedIndicator(ui->label_led0, Grey, 48);
    setLedIndicator(ui->label_led1, Grey, 48);
    setLedIndicator(ui->signal, Grey, 48);
    setLedIndicator(ui->label2_led1, Grey, 48);
    setLedIndicator(ui->label3_1, 3, 40);  // Grey = 3
    setLedIndicator(ui->label3_2, 3, 40);
    setLedIndicator(ui->label3_3, 3, 40);
    setLedIndicator(ui->label3_4, 3, 40);
    setLedIndicator(ui->label3_5, 3, 40);
    setLedIndicator(ui->label3_6, 3, 40);
    setLedIndicator(ui->label3_7, 3, 40);
    setLedIndicator(ui->label3_8, 3, 40);
    setLedIndicator(ui->label3_9, 3, 40);
    setLedIndicator(ui->label3_10, 3,40);
    setLedIndicator(ui->label3_11, 3,40);
    setLedIndicator(ui->label3_12, 3,40);
    setLedIndicator(ui->label3_13, 3,40);
    setLedIndicator(ui->label3_14, 3,40);
    setLedIndicator(ui->label3_15, 3,40);
    setLedIndicator(ui->label3_16, 3,40);
    setLedIndicator(ui->label3_er_1, 3,  70);
    setLedIndicator(ui->label3_er_2, 3,  70);
    setLedIndicator(ui->label3_er_3, 3,  70);
    setLedIndicator(ui->label3_er_4, 3,  70);
    setLedIndicator(ui->label3_er_5, 3,  70);
    setLedIndicator(ui->label3_er_6, 3,  70);
    setLedIndicator(ui->label3_er_7, 3,  70);
    setLedIndicator(ui->label3_er_8, 3,  70);
    setLedIndicator(ui->label3_er_17, 3, 70);
    setLedIndicator(ui->label3_er_18, 3, 70);
    setLedIndicator(ui->label3_er_19, 3, 70);
    setLedIndicator(ui->label3_er_20, 3, 70);
    setLedIndicator(ui->label3_er_21, 3, 70);
    setLedIndicator(ui->label3_er_22, 3, 70);
    setLedIndicator(ui->label3_er_23, 3, 70);
    setLedIndicator(ui->label3_er_24, 3, 70);
    setLedIndicator(ui->label3_er_67, 3, 70);
    setLedIndicator(ui->label3_er_68, 3, 70);
    setLedIndicator(ui->label3_er_69, 3, 70);
    setLedIndicator(ui->label3_er_70, 3, 70);
    setLedIndicator(ui->label3_er_71, 3, 70);
    setLedIndicator(ui->label3_er_72, 3, 70);

    //
    // 创建两个 QList 来存储所有 QLabel 对象的指针


    // 使用 for 循环遍历 QList，并调用 setLedIndicator 函数


    closePower();
    // 如果接收线程存在，则退出并等待其完成
    if (receiverThread)
    {
        receiverThread->quit();
        receiverThread->wait();
        delete receiverThread;  // 删除线程对象
    }

    // 如果日志线程存在，则退出并等待其完成
    if (logThread)
    {
        logThread->quit();
        logThread->wait();

        delete logThread;  // 删除线程对象
    }
    if(excelreaderThread)
    {
        excelreaderThread->quit();
        excelreaderThread->wait();
    }
    if(workerThread)
    {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
    }
    // 如果UDP套接字存在，则关闭它
    if (udpSocket != nullptr)
    {
        udpSocket->close();
    }
    //if (autoKeyPressThread && autoKeyPressThread->isRunning())
    //{
      //  autoKeyPressThread->stop();
       // autoKeyPressThread->wait(); // 等待线程结束
      //  delete autoKeyPressThread;
    //}
}

/*----------------------------------------------初始化功能-----------------------------------------------------------*/
// 初始化界面组件
void MainWindow::initializeUiComponents()
{   // 初始化指示灯状态为灰色
    // setLedIndicator(ui->label_led0, Grey, 48);
    setLedIndicator(ui->label_led1, Grey, 48);
    setLedIndicator(ui->label2_led0, Grey, 48);
    setLedIndicator(ui->label2_led1, Grey, 48);
    setLedIndicator(ui->signal, Grey, 48);
    setLedIndicator(ui->label3_1, 3,  40);  // Grey = 3
    setLedIndicator(ui->label3_2, 3,  40);
    setLedIndicator(ui->label3_3, 3,  40);
    setLedIndicator(ui->label3_4, 3,  40);
    setLedIndicator(ui->label3_5, 3,  40);
    setLedIndicator(ui->label3_6, 3,  40);
    setLedIndicator(ui->label3_7, 3,  40);
    setLedIndicator(ui->label3_8, 3,  40);
    setLedIndicator(ui->label3_9, 3,  40);
    setLedIndicator(ui->label3_10, 3, 40);
    setLedIndicator(ui->label3_11, 3, 40);
    setLedIndicator(ui->label3_12, 3, 40);
    setLedIndicator(ui->label3_13, 3, 40);
    setLedIndicator(ui->label3_14, 3, 40);
    setLedIndicator(ui->label3_15, 3, 40);
    setLedIndicator(ui->label3_16, 3, 40);
    setLedIndicator(ui->label3_er_1, 3,  50);
    setLedIndicator(ui->label3_er_2, 3,  50);
    setLedIndicator(ui->label3_er_3, 3,  50);
    setLedIndicator(ui->label3_er_4, 3,  50);
    setLedIndicator(ui->label3_er_5, 3,  50);
    setLedIndicator(ui->label3_er_6, 3,  50);
    setLedIndicator(ui->label3_er_7, 3,  50);
    setLedIndicator(ui->label3_er_8, 3,  50);
    setLedIndicator(ui->label3_er_17, 3, 50);
    setLedIndicator(ui->label3_er_18, 3, 50);
    setLedIndicator(ui->label3_er_19, 3, 50);
    setLedIndicator(ui->label3_er_20, 3, 50);
    setLedIndicator(ui->label3_er_21, 3, 50);
    setLedIndicator(ui->label3_er_22, 3, 50);
    setLedIndicator(ui->label3_er_23, 3, 50);
    setLedIndicator(ui->label3_er_24, 3, 50);
    setLedIndicator(ui->label3_er_67, 3, 70);
    setLedIndicator(ui->label3_er_68, 3, 70);
    setLedIndicator(ui->label3_er_69, 3, 70);

    setLedIndicator(ui->label3_er_70, 3, 70);
    setLedIndicator(ui->label3_er_71, 3, 70);
    setLedIndicator(ui->label3_er_72, 3, 70);


    // 使用 for 循环遍历 QList，并调用 setLedIndicator 函数



    // 设置默认IP地址和端口号
    defaultIp = "169.254.237.68";
    defaultPort = 5060;
    ui->IPtextBox->setText(defaultIp);                          // 设置IP地址文本框
    ui->PorttextBox->setText(QString::number(defaultPort));     // 设置端口号文本框
    //////////////////////////RFM2G///////////////////////////////
    readButton = new QPushButton("Read RFM2G Data", this);


        // 初始化显示读取数据的 QLineEdit
        readLineEdit = new QLineEdit(this);
        readLineEdit->setReadOnly(true);


        // 初始化写入按钮
        writeButton = new QPushButton("Write RFM2G Data", this);


        // 初始化输入写入数据的 QLineEdit
        writeLineEdit = new QLineEdit(this);


        // 初始化 RFM2G
        initializeRfm2g();


        // 连接读取按钮
//        connect(readButton, &QPushButton::clicked, this, &MainWindow::readDataFromRfm2g);

//        // 连接写入按钮
//        connect(writeButton, &QPushButton::clicked, this, &MainWindow::writeDataToRfm2g);



    // 连接鼠标移动事件，用于在图表上显示点的详细信息
    connect(ui->Speed_customPlot, &QCustomPlot::mouseMove, this, &MainWindow::showPointToolTip);
    connect(ui->Torque_customPlot, &QCustomPlot::mouseMove, this, &MainWindow::showPointToolTip);
}

void MainWindow::InitTimer()
{   // 创建定时器，用于定时发送数据
    positionCheckTimer = new QTimer(this);
    connect(positionCheckTimer , &QTimer::timeout, this, &MainWindow::checkPosition);
    // remoteTimer = new QTimer(this);
    // connect(remoteTimer,&QTimer::timeout,this,&MainWindow::readXls);
    // remoteTimer->start(1000);
}

// 初始化LED指示灯
void MainWindow::setLedIndicator(QLabel* label, int color, int size)
{
    // 清空label中的文本
    label->setText("");
    // 设置label的大小和形状
    QString min_width = QString("min-width: %1px;").arg(size);
    QString min_height = QString("min-height: %1px;").arg(size);
    QString max_width = QString("max-width: %1px;").arg(size);
    QString max_height = QString("max-height: %1px;").arg(size);
    QString border_radius = QString("border-radius: %1px;").arg(size / 2);  // 圆形边界
    QString border = "border:1px solid black;";  // 黑色边框

    // 根据颜色参数设置背景色
    QString background = "background-color:";
    switch (color)
    {
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

    // 应用样式表
    const QString sheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(sheetStyle);
}

// 初始化接收和日志记录线程
void MainWindow::InitThread()
{
    // 创建并启动接收数据的子线程
    receiverThread = new QThread(this);
    receiver = new Receive();
    receiver->moveToThread(receiverThread);

    // 创建并启动日志记录的子线程
    logThread = new QThread(this);
    logger = new qlog();
    logger->moveToThread(logThread);


    //创建远程线程
    excelreaderThread = new QThread(this);
    QString filePath = "C:\\Users\\ASUS\\Desktop\\test.xls"; // 指定 Excel 文件路径
    // excelreader = new ExcelReader();
    excelreader = new ExcelReader(filePath, this, this); // 传递 MainWindow 指针和 parent
    excelreader->moveToThread(excelreaderThread);

    worker = new Worker(nullptr);

    // 创建一个新的工作线程
    workerThread = new QThread(this);

    // 将 Worker 移动到新的工作线程
    worker->moveToThread(workerThread);

    // 连接信号与槽
    connect(workerThread, &QThread::started, worker, &Worker::startTimer,Qt::QueuedConnection);

    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater,Qt::QueuedConnection);
    connect(worker,&Worker::sendCommandResquest,this,&MainWindow::SendCommand);
    connect(worker, &Worker::absoluteMoveRequest, this, &MainWindow::handleAbsoluteMove);
    connect(worker, &Worker::abreluteMoveRequest, this, &MainWindow::handleresoluteMove);
    connect(worker, &Worker::powerStateChanged, this, &MainWindow::handlePowerStateChange,Qt::QueuedConnection);
    // 启动线程
    workerThread->start();
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qDebug()<<"123"<<workerThread;


//         创建一个线程来运行 Worker
//        QThread *workerThread = new QThread(this);

//        // 将 Worker 移动到工作线程中
//        worker->moveToThread(workerThread);

//        connect(workerThread, &QThread::started, worker, &Worker::startTimer);// 当日志更新时更新UI
//        connect(workerThread, &QThread::started, worker, &Worker::startTimer); // 连接线程启动时的槽

//       connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);  // 清理 Worker 对象







    // 连接信号和槽以处理数据和日志记录
    connect(this, &MainWindow::dataReceived, receiver, &Receive::handleData);  // 将接收到的数据发送给receiver处理
    connect(receiver, &Receive::dataSend, this, &MainWindow::updateUI,Qt::QueuedConnection);        // 接收数据后更新UI
    connect(receiver, &Receive::dataSend, logger, &qlog::updateData,Qt::QueuedConnection);          // 将接收到的数据发送给日志记录器
    connect(logger, &qlog::logUpdated, this, &MainWindow::updateLogUI,Qt::QueuedConnection);


}


// 初始化图表
void MainWindow::initChart(QCustomPlot *customPlot, const QColor &color, double xRangeStart,
                           double xRangeEnd, double yRangeStart, double yRangeEnd)
{
    customPlot->addGraph();                     // 在图表中添加新的图形
    customPlot->graph(0)->setPen(QPen(color));  // 设置图形的颜色

    // 配置时间格式的x轴
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss:zzz"); // x轴时间显示格式
    customPlot->xAxis->setTicker(dateTicker);

    // 设置x轴和y轴的范围
    customPlot->xAxis->setRange(xRangeStart, xRangeEnd);
    customPlot->yAxis->setRange(yRangeStart, yRangeEnd);

    // 启用鼠标跟踪，以便显示鼠标所指数据点的详细信息
    customPlot->setMouseTracking(true);
    // 设置背景颜色
    customPlot->setBackground(QColor("#2E3B4E"));

    // 轴线和标签颜色
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);

    // 网格颜色
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
}


//void MainWindow::InitTableWidget()
//{
//    // 设置行数和列数
//    ui->Arm_Widget->setRowCount(13); // 设置行数为13
//    ui->Arm_Widget->setColumnCount(4); // 设置列数为4
//
//    // 隐藏水平和垂直表头
//    ui->Arm_Widget->horizontalHeader()->setVisible(false);
//    ui->Arm_Widget->verticalHeader()->setVisible(false);
//
//    // 设置参数名称
//    QStringList paramNames = {"", "X轴", "Y轴", "Z轴", "横转角", "俯仰角", "航偏角","自由度1","自由度2","自由度3","自由度4","自由度5","自由度6" };
//
//    for (int i = 0; i < paramNames.size(); ++i)
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(paramNames.at(i));
//        ui->Arm_Widget->setItem(i, 0, item);  // 在第一列设置参数名称
//    }
//
//    // 设置第一行的标题项
//    ui->Arm_Widget->setItem(0, 1, new QTableWidgetItem("Position"));
//    ui->Arm_Widget->setItem(0, 2, new QTableWidgetItem("Speed"));
//    ui->Arm_Widget->setItem(0, 3, new QTableWidgetItem("Status"));
//
//    // 设置列宽平均分布
//    int tableWidth = ui->Arm_Widget->width();
//    int columnWidth = tableWidth / ui->Arm_Widget->columnCount();
//    for (int i = 0; i < ui->Arm_Widget->columnCount(); ++i)
//    {
//        ui->Arm_Widget->setColumnWidth(i, columnWidth);
//    }
//
//    // 设置列宽和行高自适应
//
//    ui->Arm_Widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->Arm_Widget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    // ui->Arm_Widget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
//
//
//
//    // 设置表格字体大小
//    QFont font = ui->Arm_Widget->font();
//    font.setPixelSize(20);  // 设置字体大小为20像素
//    ui->Arm_Widget->setFont(font);
//
//    // 设置表格的样式，包括背景颜色和表格线颜色
//    //ui->Arm_Widget->setStyleSheet("QTableWidget { background-color: rgb(255,0,0); gridline-color : rgb(0, 0, 0); }");
//
//    // 为表格项设置背景颜色和文本对齐方式
//    for (int row = 0; row < ui->Arm_Widget->rowCount(); ++row)
//    {
//        for (int col = 0; col < ui->Arm_Widget->columnCount(); ++col)
//        {
//            QTableWidgetItem *item = ui->Arm_Widget->item(row, col);
//            if (item == nullptr)
//            {
//                item = new QTableWidgetItem();
//                ui->Arm_Widget->setItem(row, col, item);
//            }
//            item->setBackground(QColor("lightblue"));
//            item->setTextAlignment(Qt::AlignCenter);
//        }
//    }
//}
void MainWindow::InitTableWidget()
{
    // 设置行数和列数
    ui->Arm_Widget->setRowCount(13); // 设置行数为13
    ui->Arm_Widget->setColumnCount(4); // 设置列数为4

    // 隐藏水平和垂直表头
    ui->Arm_Widget->horizontalHeader()->setVisible(false);
    ui->Arm_Widget->verticalHeader()->setVisible(false);

    // 设置参数名称
    QStringList paramNames = {"", "X轴", "Y轴", "Z轴", "横转角", "俯仰角", "航偏角", "自由度1", "自由度2", "自由度3", "自由度4", "自由度5", "自由度6"};

    for (int i = 0; i < paramNames.size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(paramNames.at(i));
        item->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
        ui->Arm_Widget->setItem(i, 0, item);  // 在第一列设置参数名称
    }

    // 设置第一行的标题项
    QTableWidgetItem *item1 = new QTableWidgetItem("Position");
    item1->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
    ui->Arm_Widget->setItem(0, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem("Speed");
    item2->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
    ui->Arm_Widget->setItem(0, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem("Status");
    item3->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
    ui->Arm_Widget->setItem(0, 3, item3);

    // 设置列宽平均分布
    int tableWidth = ui->Arm_Widget->width();
    int columnWidth = tableWidth / ui->Arm_Widget->columnCount();
    for (int i = 0; i < ui->Arm_Widget->columnCount(); ++i)
    {
        ui->Arm_Widget->setColumnWidth(i, columnWidth);
    }

    // 设置列宽和行高自适应
    ui->Arm_Widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Arm_Widget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格字体大小
    QFont font = ui->Arm_Widget->font();
    font.setPixelSize(20);  // 设置字体大小为20像素
    ui->Arm_Widget->setFont(font);

    // 为表格项设置背景颜色和文本对齐方式
    for (int row = 0; row < ui->Arm_Widget->rowCount(); ++row)
    {
        for (int col = 0; col < ui->Arm_Widget->columnCount(); ++col)
        {
            QTableWidgetItem *item = ui->Arm_Widget->item(row, col);
            if (item == nullptr)
            {
                item = new QTableWidgetItem();
                ui->Arm_Widget->setItem(row, col, item);
            }
            item->setBackground(QColor("lightblue"));
            item->setTextAlignment(Qt::AlignCenter);
            item->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
        }
    }
}



/*----------------------------------------------界面锁定功能-----------------------------------------------------------*/
//界面锁定功能
void MainWindow::Lock()
{
    // 禁用其他按钮
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        if (button != ui->Lock) {
            button->setEnabled(false);
        }
    }
}
//界面解锁功能
void MainWindow::unlock()
{
    // 启用其他按钮
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        button->setEnabled(true);
    }
}

void MainWindow::on_Lock_toggled(bool checked)
{
    if (checked) {
        ui->Lock->setText(tr("解锁")); // 当选中时，更改按钮文本
        Lock(); // 锁定界面
    } else {
        unlock(); // 解锁界面
        ui->Lock->setText(tr("锁定")); // 当选中时，更改按钮文本
    }
}


/*------------------------------------------ 日志功能 -----------------------------------------------------------*/
// 日志显示控制的槽函数
void MainWindow::on_Log_control_toggled(bool checked)
{
    if (checked)
    {
        ui->Log_control->setText(tr("开始")); // 当选中时，更改按钮文本
        logger_power = true; // 开启日志记录功能
    }
    else
    {
        ui->Log_control->setText(tr("暂停")); // 当未选中时，更改按钮文本
        logger_power = false; // 关闭日志记录功能
    }
}




// 清空日志显示的槽函数
void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear(); // 清空日志显示区域
}

// 更新日志显示界面
void MainWindow::updateLogUI(const QString &logEntry)
{
    // 将新的日志条目添加到列表
    logList.append(logEntry);
    // 限制日志列表大小，超出部分从头部移除
    while (logList.size() > 20)
    {
        logList.removeFirst();
    }
    // 如果日志显示开关打开，则更新文本框内容
    if (logger_power)
    {
        ui->textEdit->setPlainText(logList.join("\n"));
    }
    // 更新文本编辑器的滚动条，使其滚动到最新日志位置
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

// 弹出对话框，让用户选择日志文件的保存路径
void MainWindow::on_Select_log_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, tr("选择日志文件保存路径"),
        "", // 默认路径为空，可以根据需要设置为常用路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 用户选择了目录后，更新日志保存路径
    if (!dir.isEmpty())
    {
        logger->setLogFilePath(dir);
    }
}

/*---------------------------------------------- UDP功能 -----------------------------------------------------------*/
// 开始连接的槽函数
void MainWindow::on_Connection_button_clicked()
{
//    // 获取IP地址和端口号
//    ip = ui->IPtextBox->text().trimmed();
//    port = ui->PorttextBox->text().toInt();

//    // 检查IP地址和端口号的有效性
//    if (!QHostAddress(ip).isNull() && port > 0 && port < 65535)
//    {
//        // 如果已有UDP套接字，则先删除
//        if (udpSocket)
//        {
//            deleteUdpSocket(); // 调用删除UDP套接字的函数
//        }

//        // 创建一个新的UDP套接字
//        udpSocket = new QUdpSocket(this);
//        // 绑定readyRead信号，当UDP套接字有数据读取时触发
//        connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processData);

//        // 启动接收和日志线程
//        receiverThread->start();
//        qDebug()<<"RECyunxing";
//        logThread->start();
//        excelreaderThread->start();
//        workerThread->start();

//        if (receiverThread->isRunning()) {
//            qDebug() << "线程正在运行";
//        } else {
//            qDebug() << "线程没有运行";
//        }
//        receiverThread->setPriority(QThread::HighPriority);
//        logThread->setPriority(QThread::LowPriority);
//        excelreaderThread->setPriority(QThread::NormalPriority);


//        // 尝试绑定端口，监听传入的数据
//        if (!udpSocket->bind(QHostAddress::Any, port))
//        {
//            // 如果绑定失败，显示错误信息
//            // setLedIndicator(ui->signal, Red, 48); // 红色表示错误
//            QMessageBox::critical(this, tr("错误"), tr("绑定端口失败: ") + udpSocket->errorString());
//             updateEnsureUdp = false;
//            return;
//        }

//        // 如果绑定成功，显示成功信息
//        // setLedIndicator(ui->signal, Green, 48); // 绿色表示成功

//        updateEnsureUdp = true;
//    }
//    else
//    {
//        // 如果IP地址或端口号无效，显示警告
//        QMessageBox::warning(this, tr("警告"), tr("请输入有效的IP地址和端口号。"));
//    }
//    udpconnect();


    // 获取IP地址和端口号
       ip = ui->IPtextBox->text().trimmed();
       port = ui->PorttextBox->text().toInt();

       // 检查IP地址和端口号的有效性
       if (!QHostAddress(ip).isNull() && port > 0 && port < 65535)
       {
           // 如果已有UDP套接字，则先删除
           if (udpSocket)
           {
               deleteUdpSocket(); // 调用删除UDP套接字的函数
           }

           // 使用系统ping命令检查PLC连接
           checkPLCConnection();
       }
       else
       {
           // 如果IP地址或端口号无效，显示警告
           QMessageBox::warning(this, tr("警告"), tr("请输入有效的IP地址和端口号。"));
       }

       // 删除原来的udpconnect()调用
       // udpconnect();
}

// 断开UDP连接的槽函数
void MainWindow::on_Disconnection_button_clicked()
{
    closePower(); // 断开连接时关闭电源
    deleteUdpSocket(); // 断开连接时释放UDP套接字资源
    // 更新UI状态，显示为“未连接”
    setLedIndicator(ui->label_led1, Red, 48); // 红色表示未连接

    setLedIndicator(ui->signal, Red, 48);
    ui->btnPower->blockSignals(true);
    ui->btnPower->setChecked(false); // 重置电源按钮状态
    ui->btnPower->blockSignals(false);
    ui->btnPower->setText(tr("上电"));
    // 重置电源状态变量
    bPower = false;
    // 显示断开连接的信息
    QMessageBox::information(this, tr("连接状态"), tr("已断开连接"));

}

// 删除UDP套接字的函数
void MainWindow::deleteUdpSocket()
{
    // 如果UDP套接字存在，则释放资源
    if (udpSocket != nullptr)
    {
        closePower();
        udpSocket->close();
        udpSocket->deleteLater();
        udpSocket = nullptr;
    }
}
void MainWindow::checkPLCConnection()
{
    // 显示正在连接的状态
//    setLedIndicator(ui->signal, Yellow, 48); // 黄色表示正在连接

    // 创建QProcess对象执行ping命令
    QProcess *pingProcess = new QProcess(this);

    // 连接信号以处理ping完成事件
    connect(pingProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [this, pingProcess](int exitCode, QProcess::ExitStatus exitStatus) {
            // 读取ping命令的输出
            QString output = QString::fromLocal8Bit(pingProcess->readAllStandardOutput());

            // 分析ping结果
            bool connectionSuccessful = false;

            // 根据不同系统检查ping输出中的成功标志
            if (output.contains("TTL=") || output.contains("time=") || output.contains("来自")) {
                connectionSuccessful = true;
            }
            // 根据ping结果更新UI并处理连接
            if (connectionSuccessful) {
                // Ping成功，继续设置UDP连接
                setupUDPConnection();
            } else {
                // Ping失败，显示错误消息
                setLedIndicator(ui->signal, Red, 48); // 红色表示连接失败
                QMessageBox::critical(this, tr("连接状态"),
                                     tr("PLC连接失败，请检查IP地址或网络连接"));
                updateEnsureUdp = false;
            }

            // 清理QProcess资源
            pingProcess->deleteLater();
        });

    // 设置ping命令和参数（根据不同操作系统）
    QStringList arguments;
    #ifdef Q_OS_WIN
        // Windows系统 - 使用 -n 参数指定次数，-w 参数指定超时（毫秒）
        arguments << "-n" << "2" << "-w" << "1000" << ip;

    #endif

    // 启动ping进程
    pingProcess->start("ping", arguments);

    // 显示等待消息
    QMessageBox waitMsg(QMessageBox::Information, tr("请稍候"),
                       tr("正在检测PLC连接..."), QMessageBox::NoButton, this);

    // 设置一个定时器自动关闭等待消息框（最多等待3秒）
    QTimer::singleShot(3000, &waitMsg, &QMessageBox::close);

    // 显示等待消息框
    waitMsg.exec();
}

void MainWindow::setupUDPConnection()
{
    // 创建一个新的UDP套接字
    udpSocket = new QUdpSocket(this);

    // 绑定readyRead信号，当UDP套接字有数据读取时触发
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processData);

    // 启动接收和日志线程
    receiverThread->start();
    logThread->start();
    excelreaderThread->start();
    workerThread->start();

    // 设置线程优先级
    receiverThread->setPriority(QThread::HighPriority);
    logThread->setPriority(QThread::LowPriority);
    excelreaderThread->setPriority(QThread::NormalPriority);

    // 尝试绑定端口，监听传入的数据
    if (!udpSocket->bind(QHostAddress::Any, port))
    {
        // 如果绑定失败，显示错误信息
        setLedIndicator(ui->signal, Red, 48); // 红色表示错误
        QMessageBox::critical(this, tr("错误"), tr("绑定端口失败: ") + udpSocket->errorString());
        updateEnsureUdp = false;
        return;
    }

    // 绑定成功，更新连接状态
    setLedIndicator(ui->signal, Green, 48); // 绿色表示连接成功
    QMessageBox::information(this, tr("连接状态"), tr("PLC连接成功，UDP通信已建立"));
    updateEnsureUdp = true;
}

// 从UDP套接字读取数据并发送到receiver类
void MainWindow::processData()
{
    // 循环检查UDP套接字中是否有待处理的数据报
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        // 调整数组大小以匹配待读取的数据报大小
        datagram.resize(udpSocket->pendingDatagramSize());
        // 读取数据报
        udpSocket->readDatagram(datagram.data(), datagram.size(), nullptr, nullptr);

        // 发出信号，传递接收到的数据给receiver处理

        clearOldDataBuffer();

        // 处理新数据
        processNewData(datagram);
    }
}
void MainWindow::clearOldDataBuffer() {
    // 假设数据保存在一个QList中
    dataBuffer.clear();
}

void MainWindow::processNewData(const QByteArray &datagram) {
    // 将新数据添加到缓冲区
    float newData;
    memcpy(&newData, datagram.data(), sizeof(float));
    dataBuffer.append(newData);
    // 继续进行其他的数据处理操作...
    emit dataReceived(datagram);
}
void MainWindow::udpconnect()
{

    if (ui->RealPosition_Box->text().isEmpty())
    {
        UDP_Connect = false;
        setLedIndicator(ui->signal, Red, 48); // 红色表示错误
        QMessageBox::information(this, tr("连接状态"), tr("连接失败！！！"));
    }
    else
    {
        UDP_Connect = true;
        setLedIndicator(ui->signal, Green, 48);
        QMessageBox::information(this, tr("连接状态"), tr("连接成功"));
    }
}

// 发送命令到远程端
void MainWindow::SendCommand(const QByteArray &command)
{
    // 检查命令是否为上电或断电命令
       if (command == QByteArray::fromHex("00")) {
           // 断电命令处理
           bPower = false;
           ui->btnPower->blockSignals(true);
           ui->btnPower->setChecked(false);
           ui->btnPower->setText(tr("上电"));
           ui->btnPower->blockSignals(false);
           setLedIndicator(ui->label_led1, Red, 48);
           setLedIndicator(ui->label3_er_69, Red, 70);
       }
       else if (command == QByteArray::fromHex("01")) {
           // 上电命令处理
           bPower = true;
           ui->btnPower->blockSignals(true);
           ui->btnPower->setChecked(true);
           ui->btnPower->setText(tr("断电"));
           ui->btnPower->blockSignals(false);
           setLedIndicator(ui->label_led1, Green, 48);
           setLedIndicator(ui->label3_er_69, Green, 70);
       }
    // 检查是否有命令要发送
    if (command.isEmpty())
    {
        qDebug() << "尝试发送空命令";
        return;
    }

    // 检查UDP套接字是否已经绑定到端口
    if (udpSocket != nullptr && udpSocket->state() == QAbstractSocket::BoundState)
    {
        QHostAddress address(ip); // 使用成员变量ip作为远程地址
        // 发送数据报到指定的IP地址和端口
        qint64 bytesSent = udpSocket->writeDatagram(command.data(), command.size(), address, port);

        // 检查发送是否成功
        if (bytesSent == -1)
        {
            QMessageBox::critical(this, tr("错误"), tr("发送命令失败: ") + udpSocket->errorString());
        }
    }
}

/*---------------------------------------------- 清除资源功能 --------------------------------------------------------*/
// 清除资源
void MainWindow::cleanupResources()
{
    // // 关闭并重置指示灯状态
    // setLedIndicator(ui->label_led0, Grey, 48);
    // (ui->label_led1, Grey, 48);
    // (ui->signal, Grey, 48);

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
    if(workerThread)
    {

        workerThread->quit();
        workerThread->wait();
        delete workerThread;
        workerThread = nullptr;
    }

    // 如果UDP套接字已创建，则关闭它
    if (udpSocket != nullptr)
    {
        udpSocket->close();
    }
}

// 关闭界面初始化电源开关
void MainWindow::closePower()
{
    powerOff(); // 如果按钮未激活，执行断电操作
}


//系统复位按钮
void MainWindow::on_Systemresetbutton_clicked()
{

    cleanupResources();  // 清理资源
    emit closed();
    // this->close();  // 关闭当前窗口
}


/*---------------------------------------------- 电机参数显示功能 -----------------------------------------------------*/
// 更新主界面的UI，显示电机参数和图表数据
void MainWindow::updateUI(float position, float speed, float torque, float current, quint32 errorCode,
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
                          quint32 Rob_FB1, quint32 Rob_FB2)
{

    // 更新电机的实时参数显示
    updateMotorPosition( position, speed, torque, current, errorCode,
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

    //临时转化

    double Torque = static_cast<double>(torque);
    double Speed = static_cast<double>(speed);
    // 更新扭矩和速度图表
    updateTorqueChart(Torque);

    updateSpeedChart(Speed);
}

// 更新电机的实时参数显示
void MainWindow::updateMotorPosition(float position, float speed, float torque, float current, quint32 errorCode,
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
                                     quint32 Rob_FB1, quint32 Rob_FB2)
{


    quint16 totalStatusQuint16 = static_cast<quint16>(Total_status);
    quint16 totalErrorQuint16 = static_cast<quint16>(Total_error);
    quint16 totalStatus[16];
    quint16 totalError[16];
    errorCode_Shux = errorCode;
    errorCode_Hengx= errorCode2;
    errorCode_Robot = Total_error;
    // 使用位操作获取每一位
    for (int i = 0; i < 16; i++) {
        totalStatus[i] = (totalStatusQuint16 >> i) & 1; // 提取第 i 位
    }
    for (int i = 0; i < 16; i++) {
        totalError[i] = (totalErrorQuint16 >> i) & 1; // 提取第 i 位
    }
    updateIndicator(totalStatus);
    updateerrostatus( totalError);



    VeloTest();
    float maxTorque = 73.0; // 假设的最大扭矩值
    float actualTorque = maxTorque * torque / 100.0; // 计算实际扭矩
    quint32 combinedDword = (static_cast<quint32>(totalStatusQuint16) << 16) | totalErrorQuint16;
    quint32 rfm2g_sendsingal = 1;
    quint32 MovCon1 = 0; // 第一个备用 DWORD
    quint32 MovCon2 = 0; // 第二个备用 DWORD
//    quint32 erorCode = 1;
//    quint32 erorCode2 = 1;//测试使用
    QVector<QVariant> dataToWrite = {
            Frame,           // DWORD 第1个
            rfm2g_sendsingal,    // DWORD 第2个
            combinedDword,     // DWORD 第3个

            // 第4到21个参数 (每组三个: float, float, DWORD)
            X_position, X_vec, X_Status,
            Y_position, Y_vec, Y_Status,
            Z_position, Z_vec, Z_Status,
            roll, roll_vec, Roll_Status,
            pitch, pitch_vec, Pitch_Status,
            yaw, yaw_vec,yaw_Status,

            // 第22到33个参数 (float类型)
            Theta1, Theta1_Vec, Theta2, Theta2_Vec,
            Theta3, Theta3_Vec, Theta4, Theta4_Vec,
            Theta5, Theta5_Vec, Theta6, Theta6_Vec,

            // 第34和35个参数 (DWORD类型)
            Rob_FB1,
            Rob_FB2,

            // 第36到45个参数 (float类型)
            position, speed, actualTorque, current, errorCode,
            position2, speed2, torque2, current2, errorCode2,

            // 第46和47个参数 (DWORD类型)
            MovCon1,
            MovCon2 // 示例：可以调整顺序

        };

        // 调用写入函数
        //readFormattedDataFromMemory1(dataToWrite);
        writeFormattedDataToMemory(dataToWrite);

    testPos =position;
    // 更新第一个电机的实时参数
    ui->RealPosition_Box->setText(QString::number(position, 'f', 2));
    ui->RealSpeed_Box->setText(QString::number(speed, 'f', 2));
    ui->RealTorque_Box->setText(QString::number(actualTorque, 'f', 2));
    ui->Current_Box->setText(QString::number(current, 'f', 2));
    ui->errorCodeDisplayBox->setText(QString::number(errorCode));
    ui->lineEdit->setText(QString::number(Acc, 'f', 2));
    // 更新第二个电机的实时参数
    ui->RealPosition_Box_2->setText(QString::number(position2, 'f', 2));
    ui->RealSpeed_Box_2->setText(QString::number(speed2, 'f', 2));
    ui->RealTorque_Box_2->setText(QString::number(torque2, 'f', 2));
    ui->Current_Box_2->setText(QString::number(current2, 'f', 2));
    ui->errorCodeDisplayBox_2->setText(QString::number(errorCode2));
    ui->Total_Status->setText(QString::number(Total_status));
    ui->Total_Error->setText(QString::number(Total_error));

    auto setItemWithColor = [this](int row, int col, const QString& text) {
        QTableWidgetItem* item = new QTableWidgetItem(text);
        item->setForeground(QBrush(Qt::black)); // 设置字体颜色为黑色
        ui->Arm_Widget->setItem(row, col, item);
    };

    setItemWithColor(1, 1, QString::number(X_position, 'f', 2));
    setItemWithColor(1, 2, QString::number(X_vec, 'f', 2));
    setItemWithColor(1, 3, QString::number(X_Status));

    setItemWithColor(2, 1, QString::number(Y_position, 'f', 2));
    setItemWithColor(2, 2, QString::number(Y_vec, 'f', 2));
    setItemWithColor(2, 3, QString::number(Y_Status));

    setItemWithColor(3, 1, QString::number(Z_position, 'f', 2));
    setItemWithColor(3, 2, QString::number(Z_vec, 'f', 2));
    setItemWithColor(3, 3, QString::number(Z_Status));

    setItemWithColor(4, 1, QString::number(roll, 'f', 2));
    setItemWithColor(4, 2, QString::number(roll_vec, 'f', 2));
    setItemWithColor(4, 3, QString::number(Roll_Status));

    setItemWithColor(5, 1, QString::number(pitch, 'f', 2));
    setItemWithColor(5, 2, QString::number(pitch_vec, 'f', 2));
    setItemWithColor(5, 3, QString::number(Pitch_Status));

    setItemWithColor(6, 1, QString::number(yaw, 'f', 2));
    setItemWithColor(6, 2, QString::number(yaw_vec, 'f', 2));
    setItemWithColor(6, 3, QString::number(yaw_Status));

    setItemWithColor(7, 1, QString::number(Theta1, 'f', 2));
    setItemWithColor(7, 2, QString::number(Theta1_Vec, 'f', 2));

    setItemWithColor(8, 1, QString::number(Theta2, 'f', 2));
    setItemWithColor(8, 2, QString::number(Theta2_Vec, 'f', 2));

    setItemWithColor(9, 1, QString::number(Theta3, 'f', 2));
    setItemWithColor(9, 2, QString::number(Theta3_Vec, 'f', 2));

    setItemWithColor(10, 1, QString::number(Theta4, 'f', 2));
    setItemWithColor(10, 2, QString::number(Theta4_Vec, 'f', 2));

    setItemWithColor(11, 1, QString::number(Theta5, 'f', 2));
    setItemWithColor(11, 2, QString::number(Theta5_Vec, 'f', 2));

    setItemWithColor(12, 1, QString::number(Theta6, 'f', 2));
    setItemWithColor(12, 2, QString::number(Theta6_Vec, 'f', 2));
//    qDebug() << "Parsed Data...............................................................:";
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
//    qDebug() << "Parsed Data in QVector<QVariant> dataToWrite:";
//    for (int i = 0; i < dataToWrite.size(); ++i) {
//        qDebug() << "Index" << i << ":" << dataToWrite[i].toString();


}


// 更新状态指示器
void MainWindow::updateStatusIndicators(const quint16* totalStatus, const quint16* totalError) {
    for (int i = 0; i < 16; i++) {
        // 更新错误和状态指示器
    }
}

// 更新位置图表
void MainWindow::updatePositionCharts(float X_position, float X_vec, quint32 X_Status,
                                      float Y_position, float Y_vec, quint32 Y_Status,
                                      float Z_position, float Z_vec, quint32 Z_Status,
                                      float roll, float roll_vec, quint32 Roll_Status,
                                      float pitch, float pitch_vec, quint32 Pitch_Status,
                                      float yaw, float yaw_vec, quint32 yaw_Status,
                                      float Theta1, float Theta1_Vec, float Theta2, float Theta2_Vec,
                                      float Theta3, float Theta3_Vec, float Theta4, float Theta4_Vec,
                                      float Theta5, float Theta5_Vec, float Theta6, float Theta6_Vec) {
    auto setItemWithColor = [this](int row, int col, const QString& text) {
        QTableWidgetItem* item = new QTableWidgetItem(text);
        item->setForeground(QBrush(Qt::black));
        ui->Arm_Widget->setItem(row, col, item);
    };
    // this->setUpdatesEnabled(false);
    setItemWithColor(1, 1, QString::number(X_position, 'f', 2));
    setItemWithColor(1, 2, QString::number(X_vec, 'f', 2));
    setItemWithColor(1, 3, QString::number(X_Status));

    setItemWithColor(2, 1, QString::number(Y_position, 'f', 2));
    setItemWithColor(2, 2, QString::number(Y_vec, 'f', 2));
    setItemWithColor(2, 3, QString::number(Y_Status));

    setItemWithColor(3, 1, QString::number(Z_position, 'f', 2));
    setItemWithColor(3, 2, QString::number(Z_vec, 'f', 2));
    setItemWithColor(3, 3, QString::number(Z_Status));

    setItemWithColor(4, 1, QString::number(roll, 'f', 2));
    setItemWithColor(4, 2, QString::number(roll_vec, 'f', 2));
    setItemWithColor(4, 3, QString::number(Roll_Status));

    setItemWithColor(5, 1, QString::number(pitch, 'f', 2));
    setItemWithColor(5, 2, QString::number(pitch_vec, 'f', 2));
    setItemWithColor(5, 3, QString::number(Pitch_Status));

    setItemWithColor(6, 1, QString::number(yaw, 'f', 2));
    setItemWithColor(6, 2, QString::number(yaw_vec, 'f', 2));
    setItemWithColor(6, 3, QString::number(yaw_Status));

    setItemWithColor(7, 1, QString::number(Theta1, 'f', 2));
    setItemWithColor(7, 2, QString::number(Theta1_Vec, 'f', 2));

    setItemWithColor(8, 1, QString::number(Theta2, 'f', 2));
    setItemWithColor(8, 2, QString::number(Theta2_Vec, 'f', 2));

    setItemWithColor(9, 1, QString::number(Theta3, 'f', 2));
    setItemWithColor(9, 2, QString::number(Theta3_Vec, 'f', 2));

    setItemWithColor(10, 1, QString::number(Theta4, 'f', 2));
    setItemWithColor(10, 2, QString::number(Theta4_Vec, 'f', 2));

    setItemWithColor(11, 1, QString::number(Theta5, 'f', 2));
    setItemWithColor(11, 2, QString::number(Theta5_Vec, 'f', 2));

    setItemWithColor(12, 1, QString::number(Theta6, 'f', 2));
    setItemWithColor(12, 2, QString::number(Theta6_Vec, 'f', 2));

        // this->setUpdatesEnabled(true); // 恢复更新

}

/*---------------------------------------------- 图表显示功能那个 ----------------------------------------------------*/
// 更新图表数据的通用函数
void MainWindow::updateChart(QCustomPlot *customPlot, double value)
{
    double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;// 获取当前时间戳（秒）
    customPlot->graph(0)->addData(currentTime, value);  // 向图表中添加新的数据点
    double window = 1.00; // 定义显示数据的时间窗口长度（秒）
    customPlot->graph(0)->data()->removeBefore(currentTime - window); // 移除超出时间窗口的旧数据点以保持图表的更新性
    customPlot->xAxis->setRange(currentTime - window, currentTime);  // 设置x轴的范围，以实时显示最新数据
    customPlot->replot(QCustomPlot::rpQueuedReplot);// 重绘图表以显示最新数据
}


void MainWindow::VeloTest()
{
    double testTest = ui->Current_Box->text().toDouble();
    if(testTest > 100)
    {
        (ui->label_led1, Grey, 48);
        setLedIndicator(ui->signal, Grey, 48);
        closePower();
        // 如果接收线程存在，则退出并等待其完成
        if (receiverThread)
        {
            receiverThread->quit();
            receiverThread->wait();
            // delete receiverThread;  // 删除线程对象
        }

        // 如果日志线程存在，则退出并等待其完成
        if (logThread)
        {
            logThread->quit();
            logThread->wait();
            // delete logThread;  // 删除线程对象
        }

        // 如果UDP套接字存在，则关闭它
        if (udpSocket != nullptr)
        {
            udpSocket->close();
        }
        //QMessageBox::warning(this, tr("警告"), tr("电流大于10！"));
        return;
    }

}
// 初始化速度图表
void MainWindow::InitSpeedChart()
{
    initChart(ui->Speed_customPlot, Qt::red, 0, 100, -100, 100);// 使用红色初始化速度图表，设置x轴和y轴的显示范围
}
// 更新速度图表
void MainWindow::updateSpeedChart(double Speed)
{
    updateChart(ui->Speed_customPlot, Speed);// 使用新的速度值更新速度图表
}
// 初始化扭矩图表
void MainWindow::InitTorqueChart()
{
    initChart(ui->Torque_customPlot, Qt::blue, 0, 200, -50, 50);// 使用蓝色初始化扭矩图表，设置x轴和y轴的显示范围
}
// 更新扭矩图表
void MainWindow::updateTorqueChart(double Torque)
{
    updateChart(ui->Torque_customPlot, Torque);// 使用新的扭矩值更新扭矩图表
}
// 显示鼠标指向点的工具提示
void MainWindow::showPointToolTip(QMouseEvent *event)
{
    QCustomPlot *customPlot = qobject_cast<QCustomPlot *>(sender()); // 获取触发事件的图表
    if (!customPlot) return; // 如果获取的对象不是QCustomPlot，则直接返回

    // 将鼠标的像素坐标转换为图表的数据坐标
    float x = customPlot->xAxis->pixelToCoord(event->pos().x());
    float y = customPlot->yAxis->pixelToCoord(event->pos().y());

    float minDistanceSquared = std::numeric_limits<float>::max(); // 设置最小距离为最大可能值
    QString tooltipText;

    // 遍历图表中的数据点
    for (int i = 0; i < customPlot->graph(0)->data()->size(); ++i)
    {
        double dataX = customPlot->graph(0)->data()->at(i)->key;    // 获取数据点的x坐标
        double dataY = customPlot->graph(0)->data()->at(i)->value;  // 获取数据点的y坐标
        double distanceSquared = qPow(dataX - x, 2) + qPow(dataY - y, 2); // 计算点到鼠标位置的距离平方

        // 如果找到更近的数据点，更新最小距离和提示文本
        if (distanceSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceSquared;
            QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(dataX * 1000));
            tooltipText = QString("Time: %1\nValue: %2").arg(dateTime.toString("hh:mm:ss.zzz")).arg(dataY);
        }
    }
    double thresholdSquared = 0.1; // 设置判定距离阈值
    if (minDistanceSquared < thresholdSquared)
    {
        // 如果鼠标与数据点的距离小于阈值，则显示工具提示
        QToolTip::showText(event->globalPos(), tooltipText, customPlot);
        //QToolTip::showText(event->globalPosition().toPoint(), tooltipText, customPlot);
    }
}

/*---------------------------------------------- 电机控制功能 --------------------------------------------------------*/

// 上电开关的槽函数
void MainWindow::on_btnPower_toggled(bool checked)
{
    // 首先检查UDP连接是否已经建立
    if (!ensureUdpConnection())
    {
        // 如果UDP连接未建立，重置按钮状态，不执行上电或断电操作
        resetPowerButtonState(checked);
        return;
    }
    // 根据按钮当前的状态执行相应的操作
    if (checked)
    {
        powerOn();  // 如果按钮被激活，执行上电操作
        //setBPower(true);

    }
    else
    {
        powerOff(); // 如果按钮未激活，执行断电操作
        //setBPower(false);
    }
}

// 检查UDP连接是否已建立
bool MainWindow::ensureUdpConnection()
{
    // 检查udpSocket对象是否存在以及其状态是否为绑定状态
    if (!udpSocket || udpSocket->state() != QAbstractSocket::BoundState)
    {
        QMessageBox::warning(this, tr("警告"), tr("UDP连接尚未建立。"));  // 如果未建立，显示警告
        return false; // 返回false表示连接未建立

    }

    return true; // 返回true表示连接已建立
}


// 重置电源按钮的状态
void MainWindow::resetPowerButtonState(bool checked)
{
    ui->btnPower->blockSignals(true);   // 阻止信号发送，避免触发槽函数
    ui->btnPower->setChecked(!checked); // 重置按钮状态
    ui->btnPower->blockSignals(false);  // 恢复信号发送
}

// 执行上电操作

//状态指示灯
void MainWindow::Shux_Status()
{
    if (ui->errorCodeDisplayBox->text().toInt() != 0)
    {
        setLedIndicator(ui->label3_er_70, Green, 70);  // 更新电源LED;
    }
    else
    {
        setLedIndicator(ui->label3_er_70, Red, 70);  // 更新电源LED;
    }
}
void MainWindow::powerOn()
{
    ui->btnPower->setText(tr("断电"));    // 更新按钮文本表示已上电
    updateLedIndicators(Green);     // 更新LED状态，假定绿色为上电状态
    setLedIndicator(ui->label3_er_69, Green, 70);
    QByteArray messageData = QByteArray::fromHex("01");  // 上电命令的数据
    SendCommand(messageData);   // 发送上电命令
    bPower = true;
    Shux_Status();        // 设置电源状态为开
}

// 执行断电操作
void MainWindow::powerOff()
{
    ui->btnPower->setText(tr("上电"));  // 更新按钮文本表示已断电
    updateLedIndicators(Red);  // 更新LED状态，假定红色为断电状态
    setLedIndicator(ui->label3_er_69, 3, 70);
    QByteArray messageData = QByteArray::fromHex("00");  // 断电命令的数据
    SendCommand(messageData);  // 发送断电命令
    bPower = false;  // 设置电源状态为关
}

// // 更新LED指示灯的状态
void MainWindow::updateLedIndicators(Color powerLed)
{
    setLedIndicator(ui->label_led1, powerLed, 48);  // 更新电源LED
}

// 停止按钮
void MainWindow::on_btnStop_clicked()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("02");  // 停止命令的数据
        SendCommand(messageData);  // 发送停止命令
        logUpdateEnabled = false;  // 停止日志更新
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("03");  // 停止释放命令的数据
    SendCommand(messageData);  // 发送停止释放命令
    logUpdateEnabled = false;  // 继续停止日志更新
}

// 复位按钮按下
void MainWindow::on_btnReset_clicked()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("04");
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("05");
    SendCommand(messageData);
}

// 正向点动按下
void MainWindow::on_btnJogForward_pressed()
{
    if(!bPower)
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
        return;
    }

    QByteArray messageData = QByteArray::fromHex("06");
    SendCommand(messageData);
}

// 正向点动释放
void MainWindow::on_btnJogForward_released()
{
    QByteArray messageData = QByteArray::fromHex("08");
    SendCommand(messageData);
}

// 反向点动按下
void MainWindow::on_JogBackward_pressed()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("07");
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
}

// 反向点动释放
void MainWindow::on_JogBackward_released()
{
    QByteArray messageData = QByteArray::fromHex("08");//("0B")
    SendCommand(messageData);
}

/*---------------------------------------------- 相对和绝对运动功能 ----------------------------------------------------*/
void MainWindow::on_btnRelativeMove_clicked()
{
    // 检查设备是否已上电
    if (!bPower)
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    // 读取用户输入的相对位置和相对速度
    bool okRelativePosition, okRelativeSpeed, okCurrentPosition;
    float RelativePosition = ui->MoveRelative_textBox->text().toFloat(&okRelativePosition);
    float RelativeSpeed = ui->RelativeSpeed_textBox->text().toFloat(&okRelativeSpeed);
    // 获取当前电机位置
    float currentPosition = ui->RealPosition_Box->text().toFloat(&okCurrentPosition);
    // 检查目标位置是否输入正确
    if (!(okRelativePosition && okRelativeSpeed)) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    // 计算总目标位置
    totalPosition = RelativePosition + currentPosition;
    // 检查目标位置是否在允许范围内
    if (totalPosition < 0 || totalPosition > 2000)
    {
        QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(9) // 添加头部信息，假设消息类型为quint8*/
               <<RelativePosition
               <<RelativeSpeed;
            //        //<< static_cast<qint8>(1);//2024////////////

        // auto writeFloat = [&](float value) {
        //           stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        //       };
        // writeFloat(RelativePosition);
        // writeFloat(RelativeSpeed);

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("0A");
        SendCommand(messageData1);
    }
}


//绝对运动按钮
void MainWindow::on_btnAbsoluteMove_clicked()
{
    if (!bPower) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = ui->MoveAbsolute_textBox->text().toFloat(&okAbsolutePosition);
    float AbsoluteSpeed = ui->AbsoluteSpeed_textBox->text().toFloat(&okAbsoluteSpeed);

    if (!(okAbsolutePosition && okAbsoluteSpeed)) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // 检查目标位置是否在0到2000的范围内
    if (AbsolutePosition < 0 || AbsolutePosition > 2000)
    {
        QMessageBox::warning(this, "错误", "目标位置超出允许的范围（0-2000）！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(11)// 添加头部信息，假设消息类型为quint8
               << AbsolutePosition << AbsoluteSpeed;

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("0C");//12
        SendCommand(messageData1);
    }

}


/*------------------------------------------------测试功能------------------------------------------------------------*/
//往复运动
void MainWindow::on_TestBtn_toggled(bool checked)
{
    if (!bPower) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        ui->TestBtn->setChecked(false); // 确保按钮恢复未按下状态
        return;
    }

    if (checked)
    {
        // 如果按钮被按下，启动定时器
        counter = 0;
        beforePos=testPos;
        testPosition= 50 ;
        sendTestCommand(); // 发送初始位置
        // ui->TestBtn->setText("停止测试");
        ui->TestBtn->setStyleSheet("background-color: red; color: white;");
    }
    else
    {
        // 如果按钮被释放，发送释放命令并停止定时器
        QByteArray messageData = QByteArray::fromHex("0A");//10
        SendCommand(messageData);
        // ui->TestBtn->setText("开始测试");
        // 停止并重置定时器
        if (positionCheckTimer ) {
            positionCheckTimer ->stop();
        }
        ui->TestBtn->setStyleSheet("");
    }
}

void MainWindow::sendTestCommand()
{
    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(9) // 添加头部信息，假设消息类型为quint8
           << testPosition << testSpeed; // 设置 AbsoluteSpeed 为 10
    // 发送命令
    SendCommand(messageData);
    // 发送指令19
    QByteArray messageData1 = QByteArray::fromHex("0A");
    SendCommand(messageData1);

    // 如果定时器尚未创建，创建定时器
    if (!positionCheckTimer )
    {
        positionCheckTimer  = new QTimer(this);
        connect(positionCheckTimer , &QTimer::timeout, this, &MainWindow::checkPosition);
    }
    // 启动定时器检查位置
    positionCheckTimer ->start(500); // 每0.5秒检查一次位置
}

void MainWindow::checkPosition()
{
    float currentPosition = testPos;

    // 检查当前位置是否到达目标位置
    if (qAbs(currentPosition - (beforePos + testPosition)) < 1.0) // 假设允许误差为1.0
    {
        positionCheckTimer ->stop();
        // 更新 AbsolutePosition 的值
        if (counter%2 != 0  )
        {
            // testPosition += 50;
            testPosition = 50;
        }
        else
        {
            //testPosition = (counter - 1) * -50;
            testPosition =  -50;
        }
        beforePos = currentPosition; // 更新 beforePos

        // 判断是否达到发送五次命令的要求
        if (++counter == 6)
        {
            ui->TestBtn->setChecked(false); // 确保按钮恢复未按下状态
            ui->TestBtn->setText("开始测试");
        }
        else
        {
            // 发送下一个 testPosition
            sendTestCommand();
        }
    }
}

/*------------------------------------------------正弦运动-------------------------------------------------------------*/
void MainWindow::on_SinusOscillation_clicked()
{
    if(bPower)
    {
        float sinSpeed = ui->AbsoluteSpeed_textBox->text().toFloat();
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(16) // 添加头部信息，假设消息类型为quint8

               <<sinSpeed ; // 设置 AbsoluteSpeed 为 10
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("11");//17
    SendCommand(messageData);
}

/*------------------------------------------------速度模式-------------------------------------------------------------*/
void MainWindow::on_EndLessBtn_clicked()
{
    if(bPower)
    {

        QByteArray messageData1 = QByteArray::fromHex("0D");//13
        SendCommand(messageData1);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("0F");//15
    SendCommand(messageData);
}


void MainWindow::on_Positive_Direction_clicked()
{
    if(bPower)
    {

        QByteArray messageData1 = QByteArray::fromHex("0E");//14
        SendCommand(messageData1);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("0F");//15
    SendCommand(messageData);
}
/*------------------------------------------------速度模式-------------------------------------------------------------*/



//////////////////////////////////////////////////// 横向电机 ///////////////////////////////////////////////////////
void MainWindow::on_btnPower_2_toggled(bool checked)
{
    if (!udpSocket || udpSocket->state() != QAbstractSocket::BoundState)
    {
        QMessageBox::warning(this, tr("警告"), tr("UDP连接尚未建立。"));
        ui->btnPower->blockSignals(true);     // 暂时阻止信号，避免setChecked触发toggled信号
        ui->btnPower->setChecked(!checked);   // 重置按钮的状态为之前的状态
        ui->btnPower->blockSignals(false);    // 恢复信号
        return;                                 // 直接返回，不执行后续操作
    }

    if (checked)
    {

        ui->btnPower_2->setText( ("断电"));    // 按钮现在是选中状态，表示上电
        setLedIndicator(ui->label2_led1, 1, 70);//绿色
        setLedIndicator(ui->label2_led0, 3, 70);//灰色
        setLedIndicator(ui->label3_er_67, Green, 70);//绿色
        QByteArray messageData = QByteArray::fromHex("20"); // 假设"24"是上电命令//32
        SendCommand(messageData);
        bPower2 = true;
        Hengx_Status();
        // logUpdateEnabled= true;
        // logList.clear(); // 清空日志列表
        // logTimer->start(); // 开始记录数据

    }
    else
    {
        ui->btnPower_2->setText(tr("上电"));    // 按钮现在是未选中状态，表示断电
        setLedIndicator(ui->label2_led1, 1, 70);//绿色
        setLedIndicator(ui->label2_led0, 3, 70);//灰色
        setLedIndicator(ui->label3_er_67, Red, 70);//绿色
        QByteArray messageData = QByteArray::fromHex("1F"); // 假设"25"是断电命令//31
        SendCommand(messageData);
        bPower2 = false;

        // logUpdateEnabled= false;
        // logTimer->stop(); // 停止记录数据
    }
}

void MainWindow::Hengx_Status()
{
    if (ui->errorCodeDisplayBox_2->text().toInt() != 0)
    {
        setLedIndicator(ui->label3_er_71, Green, 70);
    }
    else
    {
        setLedIndicator(ui->label3_er_71, Red, 70);
    }
}
// 正向点动按下
void MainWindow::on_btnJogForward_2_pressed()
{
    if(bPower2)
    {
        QByteArray messageData = QByteArray::fromHex("25");//37
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
}

// 正向点动释放
void MainWindow::on_btnJogForward_2_released()
{
    QByteArray messageData = QByteArray::fromHex("27");//39
    SendCommand(messageData);
}

// 反向点动按下
void MainWindow::on_JogBackward_2_pressed()
{
    if(bPower2)
    {
        QByteArray messageData = QByteArray::fromHex("26");//38
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
}

// 反向点动释放
void MainWindow::on_JogBackward_2_released()
{
    QByteArray messageData = QByteArray::fromHex("27");//39
    SendCommand(messageData);
}

////////////////////////////////////////////////////////////系统复位////////////////////////////////////////////////////////////
// 系统复位
void MainWindow::on_Systemresetbutton_2_pressed()
{
    if(bPower)
    {
        //        QByteArray messageData = QByteArray::fromHex("35");
        //        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
}

//相对移动
void MainWindow::on_btnRelativeMove_2_clicked()
{
    if (!bPower2) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okRelativePosition2, okRelativeSpeed2;//,okCurrentPosition;
    float RelativePosition2 = ui->TargetLocation_textBox_2->text().toFloat(&okRelativePosition2);
    float RelativeSpeed2 = ui->TargetSpeed_textBox_2->text().toFloat(&okRelativeSpeed2);


    // 获取当前电机位置
    // float currentPosition = ui->RealPosition_Box_2->text().toFloat(&okCurrentPosition);


    if (!(okRelativePosition2 && okRelativeSpeed2 )) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // // 检查目标位置加当前位置是否在0到2000的范围内
    // float totalPosition = RelativePosition + currentPosition;
    // if (totalPosition < 0 || totalPosition > 2000)
    // {
    //     QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
    //     return;
    // }

    // // 检查当运动超出速度限制范围，强制将速度设为10
    // if(totalPosition>1900||totalPosition <100)
    // {
    //     RelativeSpeed=10;
    // }

    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian); // 或使用QDataStream::BigEndian，取决于目标平台/协议要求

    stream << static_cast<quint8>(40) // 添加头部信息，假设消息类型为quint8
//           << static_cast<quint8>(1)
           << RelativePosition2 << RelativeSpeed2;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("29");//41
    SendCommand(messageData1);
}

//绝对移动
void MainWindow::on_btnAbsoluteMove_2_clicked()
{
    if (!bPower2) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = ui->MoveAbsolute_textBox_2->text().toFloat(&okAbsolutePosition);
    float AbsoluteSpeed = ui->AbsoluteSpeed_textBox_2->text().toFloat(&okAbsoluteSpeed);

    if (!(okAbsolutePosition && okAbsoluteSpeed )) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // 检查目标位置加当前位置是否在0到2000的范围内

    // if (AbsolutePosition < 0 || AbsolutePosition > 2000)
    // {
    //     QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
    //     return;
    // }

    // // 检查当运动超出速度限制范围，强制将速度设为10
    // if(AbsolutePosition>1900||AbsolutePosition < 100)
    // {
    //     AbsoluteSpeed=10;
    // }

    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian); // 或使用QDataStream::BigEndian，取决于目标平台/协议要求


    stream << static_cast<quint8>(42) // 添加头部信息，假设消息类型为quint8
//           << static_cast<quint8>(1)
           << AbsolutePosition << AbsoluteSpeed;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("2B");//43
    SendCommand(messageData1);
}

//停止按钮
void MainWindow::on_btnStop_2_clicked()
{
    if(bPower2)
    {
        QByteArray messageData = QByteArray::fromHex("21");//33
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }

    QByteArray messageData = QByteArray::fromHex("22");//34
    SendCommand(messageData);
}

//复位按钮
void MainWindow::on_btnReset_2_clicked()
{
    if(bPower2)
    {
        QByteArray messageData = QByteArray::fromHex("23");//35
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("24");//36
    SendCommand(messageData);
}

////////////////////////////////////////////////////////////失效目标模拟装置////////////////////////////////////////////////////////////

void MainWindow::on_robot_poweron_clicked()
{
    ui->Robot_power->setText("失效目标模拟装置下电");
    QByteArray messageData = QByteArray::fromHex("37");
    SendCommand(messageData);
    bPower3 = true;
}


void MainWindow::on_robot_poweroff_clicked()
{
    ui->Robot_power->setText("失效目标模拟装置上电");
    QByteArray messageData = QByteArray::fromHex("38");
    SendCommand(messageData);
    bPower3 = false;
}



bool MainWindow::areAllVecFieldsEmpty() {
    // 检查所有输入框是否为空
    QString xVecText = ui->x_v->text();
    QString yVecText = ui->y_v->text();
    QString zVecText = ui->z_v->text();
    QString rxVecText = ui->rx_v->text();
    QString ryVecText = ui->ry_v->text();
    QString rzVecText = ui->rz_v->text();

    // 如果所有输入框都为空，返回 true；否则返回 false
    return xVecText.isEmpty() && yVecText.isEmpty() && zVecText.isEmpty() &&
           rxVecText.isEmpty() && ryVecText.isEmpty() && rzVecText.isEmpty();
}

void MainWindow::on_robot_jpmove_pressed()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool NoVec = false;
    if (areAllVecFieldsEmpty())
    {
        NoVec = true;
    }

    bool okX_pos, okX_vec, okY_pos, okY_vec, okZ_pos, okZ_vec, okRx_pos, okRx_vec, okRy_pos, okRy_vec, okRz_pos, okRz_vec;
    float X_pos = ui->x_p->text().toFloat(&okX_pos);
    float Y_pos = ui->y_p->text().toFloat(&okY_pos);
    float Z_pos = ui->z_p->text().toFloat(&okZ_pos);
    float Rx_pos = ui->rx_p->text().toFloat(&okRx_pos);
    float Ry_pos = ui->ry_p->text().toFloat(&okRy_pos);
    float Rz_pos = ui->rz_p->text().toFloat(&okRz_pos);

    float X_vec = ui->x_v->text().toFloat(&okX_vec);
    float Y_vec = ui->y_v->text().toFloat(&okY_vec);
    float Z_vec = ui->z_v->text().toFloat(&okZ_vec);
    float Rx_vec = ui->rx_v->text().toFloat(&okRx_vec);
    float Ry_vec = ui->ry_v->text().toFloat(&okRy_vec);
    float Rz_vec = ui->rz_v->text().toFloat(&okRz_vec);

    if (!(okX_pos && okX_vec && okY_pos && okY_vec && okZ_pos && okZ_vec &&
          okRx_pos && okRx_vec && okRy_pos && okRy_vec && okRz_pos && okRz_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(62) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(X_pos);
        writeFloat(Y_pos);
        writeFloat(Z_pos);
        writeFloat(Rx_pos);
        writeFloat(Ry_pos);
        writeFloat(Rz_pos);

        if (!NoVec)
        {
            writeFloat(X_vec);
            writeFloat(Y_vec);
            writeFloat(Z_vec);
            writeFloat(Rx_vec);
            writeFloat(Ry_vec);
            writeFloat(Rz_vec);
        }

        SendCommand(messageData);
}
}

void MainWindow::on_robot_jpmove_released()
{
    QByteArray messageData1 = QByteArray::fromHex("3F");
    SendCommand(messageData1);
}



void MainWindow::on_robot_Cartesianmove_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okX_pos, okX_vec,okY_pos,okY_vec,okZ_pos, okZ_vec, okRx_pos, okRx_vec, okRy_pos, okRy_vec, okRz_pos, okRz_vec;
    float X_pos = ui->x_p->text().toFloat(&okX_pos);
    float Y_pos = ui->y_p->text().toFloat(&okY_pos);
    float Z_pos = ui->z_p->text().toFloat(&okZ_pos);
    float Rx_pos = ui->rx_p->text().toFloat(&okRx_pos);
    float Ry_pos = ui->ry_p->text().toFloat(&okRy_pos);
    float Rz_pos = ui->rz_p->text().toFloat(&okRz_pos);

    float X_vec = ui->x_v->text().toFloat(&okX_vec);
    float Y_vec = ui->y_v->text().toFloat(&okY_vec);
    float Z_vec = ui->z_v->text().toFloat(&okZ_vec);
    float Rx_vec = ui->rx_v->text().toFloat(&okRx_vec);
    float Ry_vec = ui->ry_v->text().toFloat(&okRy_vec);
    float Rz_vec = ui->rz_v->text().toFloat(&okRz_vec);


    if (!(okX_pos && okX_vec && okY_pos && okY_vec && okZ_pos && okZ_vec &&
          okRx_pos && okRx_vec && okRy_pos && okRy_vec && okRz_pos && okRz_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(X_pos);
        writeFloat(Y_pos);
        writeFloat(Z_pos);
        writeFloat(Rx_pos);
        writeFloat(Ry_pos);
        writeFloat(Rz_pos);

        if (!NoVec)
        {
            writeFloat(X_vec);
            writeFloat(Y_vec);
            writeFloat(Z_vec);
            writeFloat(Rx_vec);
            writeFloat(Ry_vec);
            writeFloat(Rz_vec);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }
}

//正弦

void MainWindow::on_robot_stop_pressed()
{    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    QByteArray messageData = QByteArray::fromHex("3B");
    SendCommand(messageData);


}


void MainWindow::on_robot_stop_released()
{
    QByteArray messageData1 = QByteArray::fromHex("3C");
    SendCommand(messageData1);
}


void MainWindow::on_robot_zero_2_pressed()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    QByteArray messageData = QByteArray::fromHex("39");
    SendCommand(messageData);
}
void MainWindow::on_robot_zero_2_released()
{
    QByteArray messageData = QByteArray::fromHex("3A");
    SendCommand(messageData);
}

void MainWindow::on_end_code_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
        QByteArray messageData = QByteArray::fromHex("47");
        SendCommand(messageData);


        QByteArray messageData1 = QByteArray::fromHex("48");
        SendCommand(messageData1);

}


void MainWindow::on_alarm_reset_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    QByteArray messageData = QByteArray::fromHex("49");
    SendCommand(messageData);


    QByteArray messageData1 = QByteArray::fromHex("4A");
    SendCommand(messageData1);
}


void MainWindow::on_robot_follow_clicked(bool checked)
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    if(checked)
    {
        ui->robot_follow->setText(tr("结束画圆运动"));
        QByteArray messageData = QByteArray::fromHex("42");  // 这里保持不变，假设 42 是开始画圆的命令
        SendCommand(messageData);
    }
    else
    {
        ui->robot_follow->setText(tr("跟踪运动(⚪)"));
        QByteArray messageData = QByteArray::fromHex("43");  // 这里发送一个不同的命令，假设 43 是结束画圆的命令
        SendCommand(messageData);
    }
}

void MainWindow::on_robot_follow_2_clicked(bool checked)
{    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    if(checked)
    {
        ui->robot_follow_2->setText(tr("结束正弦运动"));
        QByteArray messageData = QByteArray::fromHex("44");  // 这里假设 44 是开始正弦运动的命令
        SendCommand(messageData);
    }
    else
    {
        ui->robot_follow_2->setText(tr("跟踪运动(正弦)"));
        QByteArray messageData = QByteArray::fromHex("45");  // 这里假设 45 是结束正弦运动的命令
        SendCommand(messageData);
    }
}
// 机器人状态指示灯
// 假设你在 .ui 文件中已经有一个 QLabel，名称为 label_indicator

//状态
void MainWindow::updateIndicator(quint16 totalStatus[16])
{
    QList<QLabel*> labels1 = {
        ui->label3_1, ui->label3_2, ui->label3_3, ui->label3_4,
        ui->label3_5, ui->label3_6, ui->label3_7, ui->label3_8,
        ui->label3_9, ui->label3_10, ui->label3_11, ui->label3_12,
        ui->label3_13, ui->label3_14, ui->label3_15, ui->label3_16
    };


    for (int i = 0; i < 16; ++i)
    {
        if (totalStatus[i] == 1)
        {
            // 如果对应位为 1，设置指示灯为红色
            setLedIndicator(labels1[i], Red, 40);
        }
        else
        {
            // 如果对应位为 0，设置指示灯为绿色
            setLedIndicator(labels1[i], Green, 40);
        }
    }
}

void MainWindow::updateerrostatus(quint16 totalError[16])
{


    for (int i = 0; i < 16; ++i)
    {
        if (totalError[i] == 1)
        {
            // 如果对应位为 1，设置指示灯为红色
            setLedIndicator(labels2[i], 0, 70);
        }
        else
        {
            // 如果对应位为 0，设置指示灯为绿色
            setLedIndicator(labels2[i], 1, 70);
        }
    }
}
/****************************************************主界面竖向监测***********************************************************/
void MainWindow::on_H_btnRelativeMove_clicked()
{
    // 检查设备是否已上电
        if (!bPower)
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    // 读取用户输入的相对位置和相对速度
    bool okRelativePosition, okRelativeSpeed, okCurrentPosition;
    float RelativePosition = ui->MoveRelative_textBox_8->text().toFloat(&okRelativePosition);
    float RelativeSpeed = ui->RelativeSpeed_textBox_8->text().toFloat(&okRelativeSpeed);
    // 获取当前电机位置
    float currentPosition = ui->RealPosition_Box->text().toFloat(&okCurrentPosition);
    // 检查目标位置是否输入正确
    if (!(okRelativePosition && okRelativeSpeed)) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    // 计算总目标位置
    totalPosition = RelativePosition + currentPosition;
    // 检查目标位置是否在允许范围内
    if (totalPosition < 0 || totalPosition > 2000)
    {
        QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(9) // 添加头部信息，假设消息类型为quint8
               <<RelativePosition
               <<RelativeSpeed;
        //        //<< static_cast<qint8>(1);//2024////////////

        // auto writeFloat = [&](float value) {
        //           stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        //       };
        // writeFloat(RelativePosition);
        // writeFloat(RelativeSpeed);

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("0A");
        SendCommand(messageData1);
    }
}


void MainWindow::on_btnAbsoluteMove_11_clicked()
{
    if (!bPower) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = ui->MoveAbsolute_textBox_11->text().toFloat(&okAbsolutePosition);
    float AbsoluteSpeed = ui->AbsoluteSpeed_textBox_11->text().toFloat(&okAbsoluteSpeed);

    if (!(okAbsolutePosition && okAbsoluteSpeed)) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // 检查目标位置是否在0到2000的范围内
    if (AbsolutePosition < 0 || AbsolutePosition > 2000)
    {
        QMessageBox::warning(this, "错误", "目标位置超出允许的范围（0-2000）！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(11) // 添加头部信息，假设消息类型为quint8
               // << static_cast<quint8>(1)
               << AbsolutePosition << AbsoluteSpeed;

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("0C");//12
        SendCommand(messageData1);
    }

}



void MainWindow::on_Back_zero_clicked()
{
    if (!bPower) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    float AbsolutePosition = 0;
    float AbsoluteSpeed = 5;
    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(11) // 添加头部信息，假设消息类型为quint8
           // << static_cast<quint8>(1)
           << AbsolutePosition << AbsoluteSpeed;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("0C");//12
    SendCommand(messageData1);
}



void MainWindow::on_Shux_Stop_clicked()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("02");  // 停止命令的数据
        SendCommand(messageData);  // 发送停止命令
        logUpdateEnabled = false;  // 停止日志更新
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("03");  // 停止释放命令的数据
    SendCommand(messageData);  // 发送停止释放命令
    logUpdateEnabled = false;  // 继续停止日志更新
}


void MainWindow::on_Shux_Reset_clicked()
{    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("04");
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("05");
    SendCommand(messageData);

}
/****************************************************主界面横向监测***********************************************************/

void MainWindow::on_btnRelativeMove_11_clicked()
{
    if (!bPower2) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okRelativePosition2, okRelativeSpeed2;//,okCurrentPosition;
    float RelativePosition2 = ui->MoveRelative_textBox_7->text().toFloat(&okRelativePosition2);
    float RelativeSpeed2 = ui->RelativeSpeed_textBox_7->text().toFloat(&okRelativeSpeed2);


    // 获取当前电机位置
    // float currentPosition = ui->RealPosition_Box_2->text().toFloat(&okCurrentPosition);


    if (!(okRelativePosition2 && okRelativeSpeed2 )) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // // 检查目标位置加当前位置是否在0到2000的范围内
    // float totalPosition = RelativePosition + currentPosition;
    // if (totalPosition < 0 || totalPosition > 2000)
    // {
    //     QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
    //     return;
    // }

    // // 检查当运动超出速度限制范围，强制将速度设为10
    // if(totalPosition>1900||totalPosition <100)
    // {
    //     RelativeSpeed=10;
    // }

    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian); // 或使用QDataStream::BigEndian，取决于目标平台/协议要求

    stream << static_cast<quint8>(40) // 添加头部信息，假设消息类型为quint8
           // << static_cast<quint8>(1)
           << RelativePosition2 << RelativeSpeed2;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("29");//41
    SendCommand(messageData1);
}


void MainWindow::on_btnAbsoluteMove_10_clicked()
{
    if (!bPower2) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = ui->MoveAbsolute_textBox_10->text().toFloat(&okAbsolutePosition);
    float AbsoluteSpeed = ui->AbsoluteSpeed_textBox_10->text().toFloat(&okAbsoluteSpeed);

    if (!(okAbsolutePosition && okAbsoluteSpeed )) // 如果任何转换失败
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }

    // 检查目标位置加当前位置是否在0到2000的范围内

    // if (AbsolutePosition < 0 || AbsolutePosition > 2000)
    // {
    //     QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
    //     return;
    // }

    // // 检查当运动超出速度限制范围，强制将速度设为10
    // if(AbsolutePosition>1900||AbsolutePosition < 100)
    // {
    //     AbsoluteSpeed=10;
    // }

    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian); // 或使用QDataStream::BigEndian，取决于目标平台/协议要求


    stream << static_cast<quint8>(42) // 添加头部信息，假设消息类型为quint8
           // << static_cast<quint8>(1)
           << AbsolutePosition << AbsoluteSpeed;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("2B");//43
    SendCommand(messageData1);

}


void MainWindow::on_Hengx_Stop_clicked()
{
    if(bPower2)
    {
        QByteArray messageData = QByteArray::fromHex("21");//33
        SendCommand(messageData);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }

    QByteArray messageData = QByteArray::fromHex("22");//34
    SendCommand(messageData);
}



void MainWindow::on_Hengx_Stop_2_clicked()
{
    {
        if(bPower2)
        {
            QByteArray messageData = QByteArray::fromHex("23");//35
            SendCommand(messageData);
        }
        else
        {
            QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
        }
        QByteArray messageData = QByteArray::fromHex("24");//36
        SendCommand(messageData);
    }
}


void MainWindow::on_Hengx_zero_clicked()
{
    if (!bPower2) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }

    bool okAbsolutePosition, okAbsoluteSpeed;
    float AbsolutePosition = 0;
    float AbsoluteSpeed = 5;


        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << static_cast<quint8>(42) // 添加头部信息，假设消息类型为quint8
               << static_cast<quint8>(1)
               << AbsolutePosition << AbsoluteSpeed;

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("2B");//43
        SendCommand(messageData1);
}


/***********************************************************总界面失效目标模拟装置*************************************************/


void MainWindow::on_Robot_power_toggled(bool checked)
{
    if (!ensureUdpConnection())
        {
            // 如果UDP连接未建立，重置按钮状态，不执行上电或断电操作
            resetPowerButtonState(checked);
            return;
        }
        if (checked)
        {
                ui->Robot_power->setText(tr("失效目标模拟装置下电"));
                QByteArray messageData = QByteArray::fromHex("37");
                SendCommand(messageData);
                bPower3 = true;
                setLedIndicator(ui->label3_er_68, Green, 70);
                Robot_Status();
        }
        else
        {
                ui->Robot_power->setText(tr("失效目标模拟装置上电"));
                QByteArray messageData = QByteArray::fromHex("38");
                SendCommand(messageData);
                bPower3 = false;
                setLedIndicator(ui->label3_er_68, Red, 70);
        }
    }
    void MainWindow::on_robot_zero_10_pressed()//回零
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("39");
        SendCommand(messageData);
    }


    void MainWindow::on_robot_zero_10_released()
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("3A");
        SendCommand(messageData);
    }


    void MainWindow::on_robot_stop_5_pressed()
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("3B");
        SendCommand(messageData);
    }


    void MainWindow::on_robot_stop_5_released()
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("3C");
        SendCommand(messageData);
    }



    void MainWindow::on_end_code_5_clicked()
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("47");
        SendCommand(messageData);


        QByteArray messageData1 = QByteArray::fromHex("48");
        SendCommand(messageData1);
    }


    void MainWindow::on_alarm_reset_5_clicked()
    {
        if (!bPower3) // 检查设备是否已上电
        {
            QMessageBox::warning(this, "警告", "设备未上电！");
            return;
        }
        QByteArray messageData = QByteArray::fromHex("49");
        SendCommand(messageData);


        QByteArray messageData1 = QByteArray::fromHex("4A");
        SendCommand(messageData1);
    }

    bool MainWindow::areAllVecFieldsEmpty_2() {
        // 检查所有输入框是否为空
        QString xVecText = ui->x_v_2->text();
        QString yVecText = ui->y_v_2->text();
        QString zVecText = ui->z_v_2->text();
        QString rxVecText = ui->rx_v_2->text();
        QString ryVecText = ui->ry_v_2->text();
        QString rzVecText = ui->rz_v_2->text();

        // 如果所有输入框都为空，返回 true；否则返回 false
        return xVecText.isEmpty() && yVecText.isEmpty() && zVecText.isEmpty() &&
               rxVecText.isEmpty() && ryVecText.isEmpty() && rzVecText.isEmpty();
    }










    void MainWindow::on_robot_Cartesianmove_5_clicked()
    {

            if (!bPower3) // 检查设备是否已上电
            {
                QMessageBox::warning(this, "警告", "设备未上电！");
                return;
            }
            bool okX_pos, okX_vec,okY_pos,okY_vec,okZ_pos, okZ_vec, okRx_pos, okRx_vec, okRy_pos, okRy_vec, okRz_pos, okRz_vec;
            float X_pos = ui->x_p_2->text().toFloat(&okX_pos);
            float Y_pos = ui->y_p_2->text().toFloat(&okY_pos);
            float Z_pos = ui->z_p_2->text().toFloat(&okZ_pos);
            float Rx_pos = ui->rx_p_2->text().toFloat(&okRx_pos);
            float Ry_pos = ui->ry_p_2->text().toFloat(&okRy_pos);
            float Rz_pos = ui->rz_p_2->text().toFloat(&okRz_pos);

            float X_vec = ui->x_v_2->text().toFloat(&okX_vec);
            float Y_vec = ui->y_v_2->text().toFloat(&okY_vec);
            float Z_vec = ui->z_v_2->text().toFloat(&okZ_vec);
            float Rx_vec = ui->rx_v_2->text().toFloat(&okRx_vec);
            float Ry_vec = ui->ry_v_2->text().toFloat(&okRy_vec);
            float Rz_vec = ui->rz_v_2->text().toFloat(&okRz_vec);


            if (!(okX_pos && okX_vec && okY_pos && okY_vec && okZ_pos && okZ_vec &&
                  okRx_pos && okRx_vec && okRy_pos && okRy_vec && okRz_pos && okRz_vec))
            {
                QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
                return;
            }
            else
            {
                QByteArray messageData;
                QDataStream stream(&messageData, QIODevice::WriteOnly);
                stream.setByteOrder(QDataStream::LittleEndian);

                // 先写入标头
                stream << static_cast<quint8>(64) // 添加头部信息
                       << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

                // 使用 writeRawData 方法手动写入 float 类型数据
                auto writeFloat = [&](float value) {
                    stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
                };

                writeFloat(X_pos);
                writeFloat(Y_pos);
                writeFloat(Z_pos);
                writeFloat(Rx_pos);
                writeFloat(Ry_pos);
                writeFloat(Rz_pos);

                if (!NoVec)
                {
                    writeFloat(X_vec);
                    writeFloat(Y_vec);
                    writeFloat(Z_vec);
                    writeFloat(Rx_vec);
                    writeFloat(Ry_vec);
                    writeFloat(Rz_vec);
                }

                SendCommand(messageData);

                QByteArray messageData1 = QByteArray::fromHex("41");
                SendCommand(messageData1);
            }
        }


        void MainWindow::on_robot_jpmove_5_clicked()
        {

                if (!bPower3) // 检查设备是否已上电
                {
                    QMessageBox::warning(this, "警告", "设备未上电！");
                    return;
                }
                bool NoVec = false;
                if (areAllVecFieldsEmpty())
                {
                    NoVec = true;
                }

                bool okX_pos, okX_vec, okY_pos, okY_vec, okZ_pos, okZ_vec, okRx_pos, okRx_vec, okRy_pos, okRy_vec, okRz_pos, okRz_vec;
                float X_pos = ui->x_p_2->text().toFloat(&okX_pos);
                float Y_pos = ui->y_p_2->text().toFloat(&okY_pos);
                float Z_pos = ui->z_p_2->text().toFloat(&okZ_pos);
                float Rx_pos = ui->rx_p_2->text().toFloat(&okRx_pos);
                float Ry_pos = ui->ry_p_2->text().toFloat(&okRy_pos);
                float Rz_pos = ui->rz_p_2->text().toFloat(&okRz_pos);

                float X_vec = ui->x_v_2->text().toFloat(&okX_vec);
                float Y_vec = ui->y_v_2->text().toFloat(&okY_vec);
                float Z_vec = ui->z_v_2->text().toFloat(&okZ_vec);
                float Rx_vec = ui->rx_v_2->text().toFloat(&okRx_vec);
                float Ry_vec = ui->ry_v_2->text().toFloat(&okRy_vec);
                float Rz_vec = ui->rz_v_2->text().toFloat(&okRz_vec);

                if (!(okX_pos && okX_vec && okY_pos && okY_vec && okZ_pos && okZ_vec &&
                      okRx_pos && okRx_vec && okRy_pos && okRy_vec && okRz_pos && okRz_vec))
                {
                    QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
                    return;
                }
                else
                {
                    QByteArray messageData;
                    QDataStream stream(&messageData, QIODevice::WriteOnly);
                    stream.setByteOrder(QDataStream::LittleEndian);

                    // 先写入标头
                    stream << static_cast<quint8>(62) // 添加头部信息
                           << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

                    // 使用 writeRawData 方法手动写入 float 类型数据
                    auto writeFloat = [&](float value) {
                        stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
                    };

                    writeFloat(X_pos);
                    writeFloat(Y_pos);
                    writeFloat(Z_pos);
                    writeFloat(Rx_pos);
                    writeFloat(Ry_pos);
                    writeFloat(Rz_pos);

                    if (!NoVec)
                    {
                        writeFloat(X_vec);
                        writeFloat(Y_vec);
                        writeFloat(Z_vec);
                        writeFloat(Rx_vec);
                        writeFloat(Ry_vec);
                        writeFloat(Rz_vec);
                    }

                    SendCommand(messageData);
                    QByteArray messageData1 = QByteArray::fromHex("3F");
                    SendCommand(messageData1);
                }
            }

        void MainWindow::Robot_Status()
        {
            if(ui->Total_Error->text().toInt() != 0)
            {
                setLedIndicator(ui->label3_er_72, Red, 70);
            }
            else
            {
                setLedIndicator(ui->label3_er_72, Green, 70);
            }
        }

/***************************************************自检功能**********************************************************/
        void MainWindow::selfTest() {
            // 在 switch 语句外声明局部变量
            QByteArray messageData;

            switch (step) {
            case 0:
                ui->selfTest_edit->setText("竖向电机自检中...");
                step++;  // 进入下一个步骤
                break;

            case 1:
                messageData = QByteArray::fromHex("1");
                SendCommand(messageData);
                if (errorCode_Shux == 0) {
                    ui->selfTest_edit->append("竖向电机自检成功...");
                    System_ready = true;
                    step++;
                    retryCount = 0;  // 重置重试计数
                } else {
                    if (retryCount < 1) {
                        ui->selfTest_edit->append("竖向电机自检失败，正在重新检测...");
                        retryCount++;
                        QTimer::singleShot(1000, this, &MainWindow::selfTest);
                        return;  // 提前返回，不继续执行递归调用
                    } else {
                        // 显示竖向电机的错误码
                        ui->selfTest_edit->append(QString("竖向电机自检失败，错误码: %1").arg(errorCode_Shux));
                        allPassed = false;  // 设置为失败状态
                        step++;
                        retryCount = 0;  // 重置重试计数
                    }
                }
                messageData = QByteArray::fromHex("0");
                SendCommand(messageData);
                break;

            case 2:
                ui->selfTest_edit->append("横向电机自检中...");
                step++;
                break;

            case 3:
                messageData = QByteArray::fromHex("20");
                SendCommand(messageData);
                if (errorCode_Hengx == 0) {
                    ui->selfTest_edit->append("横向电机自检成功...");
                    step++;
                    retryCount = 0;
                } else {
                    if (retryCount < 1) {
                        ui->selfTest_edit->append("横向电机自检失败，正在重新检测...");
                        retryCount++;
                        QTimer::singleShot(1000, this, &MainWindow::selfTest);
                        return;
                    } else {
                        // 显示横向电机的错误码
                        ui->selfTest_edit->append(QString("横向电机自检失败，错误码: %1").arg(errorCode_Hengx));
                        allPassed = false;  // 设置为失败状态
                        step++;
                        retryCount = 0;
                    }
                }
                messageData = QByteArray::fromHex("1F");
                SendCommand(messageData);
                break;

            case 4:
                ui->selfTest_edit->append("失效目标模拟装置自检中...");
                step++;
                break;

            case 5:
                messageData = QByteArray::fromHex("37");
                SendCommand(messageData);
                if (errorCode_Robot == 0) {
                    ui->selfTest_edit->append("失效目标模拟装置自检成功...");
                    step++;
                    retryCount = 0;
                } else {
                    if (retryCount < 1) {
                        ui->selfTest_edit->append("失效目标模拟装置自检失败，正在重新检测...");
                        retryCount++;
                        QTimer::singleShot(1000, this, &MainWindow::selfTest);
                        return;
                    } else {
                        // 显示失效目标模拟装置的错误码
                        ui->selfTest_edit->append(QString("失效目标模拟装置自检失败，错误码: %1").arg(errorCode_Robot));
                        allPassed = false;  // 设置为失败状态
                        step++;
                        retryCount = 0;
                    }
                }
                messageData = QByteArray::fromHex("38");
                SendCommand(messageData);
                break;

            case 6:
                // 最终判断所有步骤是否成功
                if (allPassed) {
                    ui->selfTest_edit->append("<font color='green'><b>系统自检成功</b></font>");
                } else {
                    ui->selfTest_edit->append("<font color='red'><b>自检出现异常</b></font>");
                }

                return;  // 结束自检流程
            }

            // 继续执行自检的下一步骤
            QTimer::singleShot(100, this, &MainWindow::selfTest);
        }

/*************************************************************重新自检***************************************************************/
        void MainWindow::on_resetSelfTest_clicked()
        {
            // 清空显示区域
            ui->selfTest_edit->clear();
            ui->selfTest_edit->setText("重新开始自检...");
            /*自检参数*/

            step = 0;        // 跟踪自检步骤
            retryCount = 0;  // 重试次数
            allPassed = true;  // 自检是否成功
            QTimer::singleShot(100, this, &MainWindow::selfTest);
            // selfTest();
        }


        void MainWindow::on_pushButton_2_clicked()
        {
            selfTest();
        }
///////////////////////////////////////////////////////远程（读取内存卡数据自运行）/////////////////////////////////////////////////
//        void MainWindow::sendAbsoluteMoveCommand(float remoteposition, float remotespeed) {
//            if (!bPower) {  // 检查设备是否已上电
//                QMessageBox::warning(this, "警告", "设备未上电！");
//                return;
//            }

//            // 检查目标位置和速度值是否合法
//            if (remoteposition < 0 || remoteposition > 2000) {
//                QMessageBox::warning(this, "错误", "目标位置加当前位置超出允许的范围（0-2000）！");
//                return;
//            }

//            // 如果位置超过阈值，将速度限制为 10
//            if (remotespeed < 0 || remotespeed > 100) {
//                remotespeed = 10;
//            }

//            QByteArray messageData;
//            QDataStream stream(&messageData, QIODevice::WriteOnly);
//            stream.setByteOrder(QDataStream::LittleEndian);

//            stream << static_cast<quint8>(42) // 添加头部信息
//                   << static_cast<quint8>(1)
//                   << remoteposition << remotespeed;

//            SendCommand(messageData);

//            QByteArray messageData1 = QByteArray::fromHex("2B"); // 发送固定指令
//            SendCommand(messageData1);
//        }

//        void MainWindow::handleNewSignal(float param1, float param2) {
//            sendAbsoluteMoveCommand(param1, param2);  // 使用发射的参数值
//        }





//        void MainWindow::setBPower(bool value) {
//            if (bPower != value) {
//                qDebug() << "Changing bPower from" << bPower << "to" << value;
//                bPower = value;
//                emit bPowerChanged(value); // 发射信号
//            }
//        }

//        bool MainWindow::getBPower() const {
//            return bPower; // 返回当前 bPower 的状态
//        }
void MainWindow::initializeRfm2g() {
    RFM2G_STATUS status = RFM2gOpen(const_cast<char*>(DEVICE), &handle);  // 打开设备 0
    if (status != RFM2G_SUCCESS) {
        qCritical() << "Failed to initialize RFM2G device. Error code:" << status;
    } else {
        qDebug() << "RFM2G initialized successfully.";
    }
}

// 从 RFM2G 读取数据
struct DataGroup {
    float value1;
    float value2;
    RFM2G_UINT32 dword;
};

void MainWindow::readDataFromRfm2g() {
    QByteArray buffer;
    buffer.resize(3 * sizeof(RFM2G_UINT32) + 6 * sizeof(DataGroup)+12*sizeof(float));

        RFM2G_STATUS status = RFM2gRead(handle, baseAddress, buffer.data(), buffer.size());
        if (status == RFM2G_SUCCESS) {
            RFM2G_UINT32 dword1 = *reinterpret_cast<RFM2G_UINT32*>(buffer.data());
            RFM2G_UINT32 dword2 = *reinterpret_cast<RFM2G_UINT32*>(buffer.data() + sizeof(dword1));
            RFM2G_UINT32 dword3 = *reinterpret_cast<RFM2G_UINT32*>(buffer.data() + 2 * sizeof(dword1));

            DataGroup* groups = reinterpret_cast<DataGroup*>(buffer.data() + 3 * sizeof(RFM2G_UINT32));
            QString displayText = QString("%1, %2, %3,").arg(dword1).arg(dword2).arg(dword3);

            for (int i = 0; i < 6; i++) {
                displayText += QString("%1, %2, %3,")
                    .arg(groups[i].value1, 0, 'f', 2)
                    .arg(groups[i].value2, 0, 'f', 2)
                    .arg(groups[i].dword);
            }
            for(int j =0;j<12;j++)
            {
                displayText += QString(",%1")
                        .arg(omga[j],0,'f',2);

            }

            qDebug() << "Data read:" << displayText;
            readLineEdit->setText(displayText.trimmed());
        } else {
            qCritical() << "Failed to read data. Error code:" << status;
        }
}

// 向 RFM2G 写入数据

void MainWindow::clearRfm2gData() {
    if (!handle) {
        qCritical() << "RFM2G handle is not initialized.";
        return;
    }

    // 清除地址范围 0X4002000 - 0X4002088
    const RFM2G_UINT32 startAddress = 0x4008000;
    const RFM2G_UINT32 endAddress = 0x4008280;
    const RFM2G_UINT32 dataSize = endAddress - startAddress + 1;

    // 创建缓冲区并填充为 0
    QVector<float> clearData(dataSize / sizeof(float), 0.0f);
    float *buffer = clearData.data();

    // 写入清零数据到指定地址范围
    RFM2G_STATUS status = RFM2gWrite(handle, startAddress, buffer, dataSize);
    if (status == RFM2G_SUCCESS) {
        qDebug() << "Cleared RFM2G data successfully.";
    } else {
        qCritical() << "Failed to clear RFM2G data. Error code:" << status;
    }
}

void MainWindow::on_readButton_clicked()
{
    QVector<QString> dataTypes = {
            "DWORD", "DWORD", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "DWORD",
            "float", "float", "float", "float", "float", "DWORD",
            "float", "float", "float", "float", "float", "DWORD",
            "DWORD", "DWORD",
            "float", "float", "float", "float", "float",
            "float", "float", "float", "float", "float",
            "DWORD", "DWORD"
        };

        QVector<QVariant> data = readFormattedDataFromMemory(dataTypes);

        if (!data.isEmpty()) {
            qDebug() << "Data read from memory:";
            for (int i = 0; i < data.size(); ++i) {
                qDebug() << "Index" << i << ":" << data[i];
            }
        }
}


void MainWindow::on_writeButton_clicked()
{
//    writeDataToRfm2g();
}

void MainWindow::on_clearButton_clicked()
{
    clearRfm2gData();
}
void MainWindow::writeFormattedDataToMemory(const QVector<QVariant>& dataToWrite) {
    // 检查反射内存句柄是否已初始化
        if (!handle) {
            qCritical() << "RFM2G handle is not initialized. Unable to write.";
            return;
        }

        if (dataToWrite.isEmpty()) {
            qCritical() << "No data to write. The input QVector is empty.";
            return;
        }

        // 准备缓冲区
        QByteArray buffer;
        buffer.reserve(dataToWrite.size() * sizeof(float)); // 预留足够空间，优化性能

        for (int i = 0; i < dataToWrite.size(); ++i) {
            if (dataToWrite[i].type() == QVariant::UInt) { // DWORD 类型
                quint32 value = qToLittleEndian(dataToWrite[i].toUInt()); // 转为小端格式
                buffer.append(reinterpret_cast<const char*>(&value), sizeof(quint32));
            } else if (dataToWrite[i].canConvert<float>()) { // Float 类型
                float value = dataToWrite[i].toFloat();
                quint32 littleEndianFloat = qToLittleEndian(*reinterpret_cast<quint32*>(&value));
                buffer.append(reinterpret_cast<const char*>(&littleEndianFloat), sizeof(float));
            } else {
                qCritical() << "Unsupported or unhandled data type at index" << i
                            << ". Type:" << dataToWrite[i].typeName();
                return;
            }
        }

        // 确保缓冲区不是空的
        if (buffer.isEmpty()) {
            qCritical() << "Buffer is empty. No data was prepared for writing.";
            return;
        }

        // 写入到设备
        RFM2G_STATUS status = RFM2gWrite(handle, baseAddress, buffer.data(), buffer.size());
        if (status == RFM2G_SUCCESS) {
//            qDebug() << "Data written to memory successfully. Size:" << buffer.size() << "bytes.";
        } else {
            qCritical() << "Failed to write data to memory. Error code:" << status
                        << ". Please check device and connection.";
        }
}
QVector<QVariant> MainWindow::readFormattedDataFromMemory(const QVector<QString>& dataTypes) {
    QVector<QVariant> result;

    // 检查反射内存句柄是否已初始化
    if (!handle) {
        qCritical() << "RFM2G handle is not initialized. Unable to read.";
        return result;
    }

    // 计算读取所需的总大小
    int totalSize = 0;
    for (const auto& type : dataTypes) {
        if (type == "float") {
            totalSize += sizeof(float);
        } else if (type == "DWORD") {
            totalSize += sizeof(quint32);
        } else {
            qCritical() << "Unsupported data type in dataTypes:" << type;
            return result;
        }
    }

    // 检查总大小是否合法
    if (totalSize <= 0) {
        qCritical() << "Total size to read is zero or negative. Check dataTypes.";
        return result;
    }

    // 准备缓冲区
    QByteArray buffer(totalSize, 0);

    // 从设备读取数据
    RFM2G_STATUS status = RFM2gRead(handle, baseAddress, buffer.data(), buffer.size());
    if (status != RFM2G_SUCCESS) {
        qCritical() << "Failed to read data from memory. Error code:" << status;
        return result;
    }

    // 解包数据
    const char* dataPtr = buffer.data();
    for (const auto& type : dataTypes) {
        if (type == "float") {
            // 转换小端到主机字节序
            quint32 rawValue;
            memcpy(&rawValue, dataPtr, sizeof(float));
            rawValue = qFromLittleEndian(rawValue);
            float value = *reinterpret_cast<float*>(&rawValue);
            result.append(QVariant::fromValue(value));
            dataPtr += sizeof(float);
        } else if (type == "DWORD") {
            // 转换小端到主机字节序
            quint32 value;
            memcpy(&value, dataPtr, sizeof(quint32));
            value = qFromLittleEndian(value);
            result.append(QVariant::fromValue(value));
            dataPtr += sizeof(quint32);
        } else {
            qCritical() << "Unsupported data type encountered while unpacking:" << type;
            return result;
        }

        // 防止缓冲区溢出
        if (dataPtr - buffer.data() > totalSize) {
            qCritical() << "Buffer overrun detected. Check dataTypes and device data.";
            return result;
        }
    }

    qDebug() << "Data read successfully. Total items:" << result.size();
    return result;
}

QVector<QVariant> MainWindow::readFormattedDataFromMemory1(const QVector<QVariant>& dataTypes) {
    QVector<QVariant> result;

    // 检查反射内存句柄是否已初始化
    if (!handle) {
        qCritical() << "RFM2G handle is not initialized. Unable to read.";
        return result;
    }

    // 计算读取所需的总大小
    int totalSize = 0;
    for (const auto& type : dataTypes) {
        if (type == QVariant::UInt) {   // DWORD 类型
            totalSize += sizeof(quint32);
        } else if (type == QVariant::Double) { // Float 类型
            totalSize += sizeof(float);
        } else {
            qCritical() << "Unsupported data type in dataTypes:" << type.typeName();
            return result;
        }
    }

    // 检查总大小是否合法
    if (totalSize <= 0) {
        qCritical() << "Total size to read is zero or negative. Check dataTypes.";
        return result;
    }

    // 准备缓冲区
    QByteArray buffer(totalSize, 0);

    // 从设备读取数据
    RFM2G_STATUS status = RFM2gRead(handle, baseAddress, buffer.data(), buffer.size());
    if (status != RFM2G_SUCCESS) {
        qCritical() << "Failed to read data from memory. Error code:" << status;
        return result;
    }

    // 解包数据
    const char* dataPtr = buffer.data();
    QVector<QVariant> tempResult;
    for (const auto& type : dataTypes) {
        if (type == QVariant::UInt) {   // DWORD 类型
            quint32 value;
            memcpy(&value, dataPtr, sizeof(quint32));
            value = qFromLittleEndian(value);
            tempResult.append(QVariant::fromValue(value));
            dataPtr += sizeof(quint32);
        } else if (type == QVariant::Double) { // Float 类型
            float value;
            memcpy(&value, dataPtr, sizeof(float));
            value = qFromLittleEndian(*reinterpret_cast<quint32*>(&value));
            tempResult.append(QVariant::fromValue(value));
            dataPtr += sizeof(float);
        } else {
            qCritical() << "Unsupported data type encountered while unpacking:" << type.typeName();
            return result;
        }

        // 防止缓冲区溢出
        if (dataPtr - buffer.data() > totalSize) {
            qCritical() << "Buffer overrun detected. Check dataTypes and device data.";
            return result;
        }
    }

    // 检查第二个参数是否是DWORD类型且值为1
    if (tempResult.size() > 1 && tempResult[1].type() == QVariant::UInt) {
        quint32 secondValue = tempResult[1].toUInt();
        if (secondValue == 1) {
            // 将第二个元素设置为 0
            qDebug() << "Second parameter is DWORD with value 1. Modifying it to 0.";
            tempResult[1] = QVariant::fromValue(quint32(0));
        }
    }

    // 将修改后的数据写回内存
    writeFormattedDataToMemory(tempResult);

    qDebug() << "Data read successfully. Total items:" << tempResult.size();
    return tempResult;
}




void MainWindow::on_MainRfmRead_clicked()
{

        // 定义地址
        RFM2G_UINT32 baseAddress = 0x4008000;

        // 定义存储读取数据的结构
        struct Parameters {
            RFM2G_UINT32 MainCom;  // DWORD 类型
            float MainPos;         // FLOAT 类型
            float MainSpeed;       // FLOAT 类型
        };

        Parameters params = {}; // 初始化为零
        RFM2G_STATUS status;

        // 从 RFM2G 读取数据
        status = RFM2gRead(handle, baseAddress, &params, sizeof(params));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to read data from memory. Error code:" << status;
            return;
        }

        // 将 MainCom 转换为二进制并存储到数组
        int binaryArray[32] = {0}; // 用于存储 MainCom 的二进制位，最多 32 位
        for (int i = 0; i < 32; ++i) {
            binaryArray[i] = (params.MainCom >> i) & 1; // 提取每一位
        }

        // 打印读取的数据
        qDebug() << "MainCom:" << QString("0x%1").arg(params.MainCom, 8, 16, QChar('0'));
        qDebug() << "Binary representation of MainCom:";
        for (int i = 31; i >= 0; --i) { // 逆序输出
            qDebug() << binaryArray[i];
        }
        qDebug() << "MainPos:" << params.MainPos;
        qDebug() << "MainSpeed:" << params.MainSpeed;

        // 判断数组第一个元素是否为 1
        if (binaryArray[31] == 1) { // 第一个元素在数组的最高位位置
            qDebug() << "Array first element is 1, sending command...";

        } else {
            qDebug() << "Array first element is not 1, no command sent.";
        }
    }



#pragma pack(push, 1) // 强制按1字节对齐，确保与定义的顺序一致
struct Parameters {
    RFM2G_UINT32 MainCom;  // DWORD 类型参数
    float MainPos;         // FLOAT 类型参数
    float MainSpeed;       // FLOAT 类型参数
};
#pragma pack(pop)

void MainWindow::on_WRITECOM_clicked()
{
    RFM2G_UINT32 baseAddress = 0x4008000;

        // 定义写入数据的结构
        struct Parameters {
            RFM2G_UINT32 MainCom;  // DWORD 类型
            float MainPos;         // FLOAT 类型
            float MainSpeed;       // FLOAT 类型
        };

        // 准备写入的数据
        Parameters params = {
            0x80000000,  // MainCom 示例值
            3.14f,       // MainPos 示例值
            2.71f        // MainSpeed 示例值
        };

        // 调用 RFM2gWrite 写入数据
        RFM2G_STATUS status = RFM2gWrite(handle, baseAddress, &params, sizeof(params));
        if (status != RFM2G_SUCCESS) {
            qCritical() << "Failed to write data to memory. Error code:" << status;
            return;
        }

        // 打印成功信息
//        qDebug() << "Data successfully written to memory:";
//        qDebug() << "MainCom:" << QString("0x%1").arg(params.MainCom, 8, 16, QChar('0'));
//        qDebug() << "MainPos:" << params.MainPos;
//        qDebug() << "MainSpeed:" << params.MainSpeed;


}


void MainWindow::handleAbsoluteMove(float position, float speed)
{


    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(11)  // Message type
           << position
           << speed;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("0C");
    SendCommand(messageData1);
}

void MainWindow::handleresoluteMove(float position, float speed)
{


    QByteArray messageData;
    QDataStream stream(&messageData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << static_cast<quint8>(9)  // Message type
           << position
           << speed;

    SendCommand(messageData);

    QByteArray messageData1 = QByteArray::fromHex("0A");
    SendCommand(messageData1);
}

// mainwindow.cpp
void MainWindow::handlePowerStateChange(bool powered)
{
    // 更新UI状态
    ui->btnPower->blockSignals(true);  // 暂时阻塞信号避免触发clicked
    ui->btnPower->setChecked(powered);
    ui->btnPower->setText(powered ? tr("断电") : tr("上电"));
    ui->btnPower->blockSignals(false);

    // 更新LED指示灯状态
    setLedIndicator(ui->label_led1, powered ? Green : Red, 48);
    setLedIndicator(ui->label3_er_69, powered ? Green : Red, 70);

    // 更新内部状态
    bPower = powered;

    // 如果是断电，可能需要执行其他清理操作
    if (!powered) {
        // 执行断电时的其他操作
    }
}
//-----------------------失效目标捕获装置（单轴控制）---------------------------------//
void MainWindow::on_Robot_X_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okX_pos, okX_vec;
    float X_pos_single = ui->x_p->text().toFloat(&okX_pos);
    float X_vec_single = ui->x_v->text().toFloat(&okX_vec);
    if (!(okX_pos && okX_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(X_pos_single);

        if (!NoVec)
        {
            writeFloat(X_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }


}
void MainWindow::on_Robot_Y_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okY_pos, okY_vec;
    float Y_pos_single = ui->x_p->text().toFloat(&okY_pos);
    float Y_vec_single = ui->x_v->text().toFloat(&okY_vec);
    if (!(okY_pos && okY_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(Y_pos_single);

        if (!NoVec)
        {
            writeFloat(Y_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }

}

void MainWindow::on_Robot_Z_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okZ_pos, okZ_vec;
    float Z_pos_single = ui->x_p->text().toFloat(&okZ_pos);
    float Z_vec_single = ui->x_v->text().toFloat(&okZ_vec);
    if (!(okZ_pos && okZ_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(Z_pos_single);

        if (!NoVec)
        {
            writeFloat(Z_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }
}


void MainWindow::on_Robot_Rx_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okRx_pos, okRx_vec;
    float Rx_pos_single = ui->x_p->text().toFloat(&okRx_pos);
    float Rx_vec_single = ui->x_v->text().toFloat(&okRx_vec);
    if (!(okRx_pos && okRx_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(Rx_pos_single);

        if (!NoVec)
        {
            writeFloat(Rx_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }
}


void MainWindow::on_Robot_Ry_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okRy_pos, okRy_vec;
    float Ry_pos_single = ui->x_p->text().toFloat(&okRy_pos);
    float Ry_vec_single = ui->x_v->text().toFloat(&okRy_vec);
    if (!(okRy_pos && okRy_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(Ry_pos_single);

        if (!NoVec)
        {
            writeFloat(Ry_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
        SendCommand(messageData1);
    }
}


void MainWindow::on_Robot_Rz_clicked()
{
    if (!bPower3) // 检查设备是否已上电
    {
        QMessageBox::warning(this, "警告", "设备未上电！");
        return;
    }
    bool okRz_pos, okRz_vec;
    float Rz_pos_single = ui->x_p->text().toFloat(&okRz_pos);
    float Rz_vec_single = ui->x_v->text().toFloat(&okRz_vec);
    if (!(okRz_pos && okRz_vec))
    {
        QMessageBox::warning(this, "错误", "输入的数值无效，请检查输入！");
        return;
    }
    else
    {
        QByteArray messageData;
        QDataStream stream(&messageData, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // 先写入标头
        stream << static_cast<quint8>(64) // 添加头部信息
               << static_cast<qint8>(NoVec ? 3 : 2);  // 失效目标模拟装置数据传输FLAG

        // 使用 writeRawData 方法手动写入 float 类型数据
        auto writeFloat = [&](float value) {
            stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        };

        writeFloat(Rz_pos_single);

        if (!NoVec)
        {
            writeFloat(Rz_vec_single);
        }

        SendCommand(messageData);

        QByteArray messageData1 = QByteArray::fromHex("41");
    }

}




void MainWindow::on_Shux_Mon_Stop_clicked()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("02");  // 停止命令的数据
        SendCommand(messageData);  // 发送停止命令
        logUpdateEnabled = false;  // 停止日志更新
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("03");  // 停止释放命令的数据
    SendCommand(messageData);  // 发送停止释放命令
    logUpdateEnabled = false;  // 继续停止日志更新
}


void MainWindow::on_Shu_Mon_Stop1_clicked()
{
    if(bPower)
    {
        QByteArray messageData = QByteArray::fromHex("02");  // 停止命令的数据
        SendCommand(messageData);  // 发送停止命令
        logUpdateEnabled = false;  // 停止日志更新
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("设备未上电！"));
    }
    QByteArray messageData = QByteArray::fromHex("03");  // 停止释放命令的数据
    SendCommand(messageData);  // 发送停止释放命令
    logUpdateEnabled = false;  // 继续停止日志更新
}

