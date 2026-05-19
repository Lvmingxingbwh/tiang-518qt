#include "ExcelReader.h"
#include <QAxObject>
#include <QTimer>
#include "Mainwindow.h"


ExcelReader::ExcelReader(const QString& filePath, MainWindow *mainWindow, QObject *parent)
    : QObject(parent), filePath(filePath) {
    // 初始化定时器

    // QTimer*remotetimer = new QTimer(this);
    remotetimer = new QTimer(this); // 初始化定时器
    connect(remotetimer, &QTimer::timeout, this, &ExcelReader::run);
    // 监听 MainWindow 的 bPowerChanged 信号
    connect(mainWindow, &MainWindow::bPowerChanged, this, [this](bool bPower) {
        if (bPower) {
            qDebug() << "bPower is true in ExcelReader, starting timer...";

            remotetimer->start(10000); // 启动定时器
            qDebug() << "定时器启动";
        } else {
            qDebug() << "bPower is false in ExcelReader, stopping timer...";
            remotetimer->stop(); // 停止定时器
        }
    });
}
    // Connect the timer to the emitData slot
    // remotetimer->start(10000); // Start the timer with a 10ms interval


    // watcher = new QFileSystemWatcher(this);

    // // 添加 Excel 文件到监视列表
    // watcher->addPath(filePath);

    // 连接文件变化信号
    // connect(watcher, &QFileSystemWatcher::fileChanged, this, &ExcelReader::onFileChanged);
    // // QString filePath = "C:\\Users\\ASUS\\Desktop\\test.xls";


    // 开始监控文件
    // startMonitoring();

////////////////////////////使用定时器实现自运行//////////////////////////////////////////////////////////
void ExcelReader::run() {
    QString result;

    // 使用 QAxObject 打开 Excel 文件
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);

    QAxObject *workbooks = excel.querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", filePath);
    QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

    QAxObject *usedRange = worksheet->querySubObject("UsedRange");
    QAxObject *rows = usedRange->querySubObject("Rows");
    QAxObject *columns = usedRange->querySubObject("Columns");

    int rowCount = rows->property("Count").toInt();
    int columnCount = columns->property("Count").toInt();

    QStringList lines;

    // 获取第一行第一列的单元格内容
    QAxObject *firstCell = worksheet->querySubObject("Cells(int,int)", 1, 1);
    QString firstCellValue = firstCell->property("Value").toString();
    delete firstCell;  // 释放资源

    // 判断第一个单元格的值是否为 1 或 2
    if (firstCellValue == "1") {
        emit triggerPowerButton();  // 发射信号
    } else if (firstCellValue == "2") {
        // 获取第二个单元格（第1行第2列）和第三个单元格（第1行第3列）的内容
        QAxObject *secondCell = worksheet->querySubObject("Cells(int,int)", 1, 2);
        QString secondCellValue = secondCell->property("Value").toString();
        delete secondCell;  // 释放资源

        QAxObject *thirdCell = worksheet->querySubObject("Cells(int,int)", 1, 3);
        QString thirdCellValue = thirdCell->property("Value").toString();
        delete thirdCell;  // 释放资源

        // 判断第二个和第三个单元格是否为空
        if (secondCellValue.isEmpty() || thirdCellValue.isEmpty()) {
            qDebug() << "Error: Second or third cell is empty.";
        } else {
            // 提取第二和第三个单元格的浮动参数
            bool ok1, ok2;
            float param1 = secondCellValue.toFloat(&ok1);
            float param2 = thirdCellValue.toFloat(&ok2);

            if (ok1 && ok2) {
                emit triggerNewSignal(param1, param2);  // 发送新信号，传递两个浮点参数
            } else {
                qDebug() << "Error: Invalid float parameter values in second or third cell.";
            }
        }
    } else {
        qDebug() << "Error: First cell is neither '1' nor '2'.";
    }

    workbook->dynamicCall("Close()");
    excel.dynamicCall("Quit()");

    delete rows;
    delete columns;
    delete usedRange;
    delete worksheet;
    delete workbook;
    delete workbooks;
}
/////////////////////////////////////使用‘基于事件的文件监控’QFileSystemWatcher///////////////////////////
// // Qt 提供了一个强大的类 QFileSystemWatcher，它允许你监控文件或目录的变化。一旦 Excel 文件被修改，
// // QFileSystemWatcher会触发一个信号，你可以在信号响应中读取文件内容并执行相应的操作。
// void ExcelReader::startMonitoring() {
//     // 开始文件监控
//     qDebug() << "开始监控 Excel 文件变化: " << filePath;
// }

// // 当文件发生变化时调用
// void ExcelReader::onFileChanged(const QString &path) {

//     if (isProcessingFileChange) {
//         return; // 如果上次变化还未处理完毕，就不再处理新的变化
//     }

//     isProcessingFileChange = true;

//     // 文件发生变化时，重新读取 Excel 文件
//     readExcelFile();

//     // 处理完毕后清除标志
//     isProcessingFileChange = false;

//     // 发射文件变化信号
//     emit fileChangedSignal(path);  // 发送文件变化信号
// }

// // 读取 Excel 文件并处理内容
// void ExcelReader::readExcelFile() {
//     QString result;

//     // 使用 QAxObject 打开 Excel 文件
//     QAxObject excel("Excel.Application");
//     excel.setProperty("Visible", false);

//     QAxObject *workbooks = excel.querySubObject("Workbooks");
//     QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", filePath);
//     QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

//     QAxObject *usedRange = worksheet->querySubObject("UsedRange");
//     QAxObject *rows = usedRange->querySubObject("Rows");
//     QAxObject *columns = usedRange->querySubObject("Columns");

//     int rowCount = rows->property("Count").toInt();
//     int columnCount = columns->property("Count").toInt();

//     // 获取第一行第一列的单元格内容
//     QAxObject *firstCell = worksheet->querySubObject("Cells(int,int)", 1, 1);
//     QString firstCellValue = firstCell->property("Value").toString();
//     delete firstCell;  // 释放资源

//     // 判断第一个单元格的值是否为 1 或 2
//     if (firstCellValue == "1") {
//         emit triggerPowerButton();  // 发射信号
//     } else if (firstCellValue == "2") {
//         // 获取第二个单元格（第1行第2列）和第三个单元格（第1行第3列）的内容
//         QAxObject *secondCell = worksheet->querySubObject("Cells(int,int)", 1, 2);
//         QString secondCellValue = secondCell->property("Value").toString();
//         delete secondCell;  // 释放资源

//         QAxObject *thirdCell = worksheet->querySubObject("Cells(int,int)", 1, 3);
//         QString thirdCellValue = thirdCell->property("Value").toString();
//         delete thirdCell;  // 释放资源

//         // 判断第二个和第三个单元格是否为空
//         if (secondCellValue.isEmpty() || thirdCellValue.isEmpty()) {
//             qDebug() << "Error: Second or third cell is empty.";
//         } else {
//             // 提取第二和第三个单元格的浮动参数
//             bool ok1, ok2;
//             float param1 = secondCellValue.toFloat(&ok1);
//             float param2 = thirdCellValue.toFloat(&ok2);

//             if (ok1 && ok2) {
//                 emit triggerNewSignal(param1, param2);  // 发送新信号，传递两个浮点参数
//             } else {
//                 qDebug() << "Error: Invalid float parameter values in second or third cell.";
//             }
//         }
//     } else {
//         qDebug() << "Error: First cell is neither '1' nor '2'.";
//     }

//     workbook->dynamicCall("Close()");
//     excel.dynamicCall("Quit()");

//     delete rows;
//     delete columns;
//     delete usedRange;
//     delete worksheet;
//     delete workbook;
//     delete workbooks;
// }

