#ifndef EXCELREADER_H
#define EXCELREADER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QThread>
#include <QString>
#include <QAxObject>
#include <QFileSystemWatcher>
class MainWindow;
class ExcelReader : public QObject {
    Q_OBJECT
public:
    explicit ExcelReader(const QString& filePath, MainWindow *mainWindow, QObject *parent = nullptr);

    // void startMonitoring();
    bool isProcessingFileChange = false;

signals:
    void readingFinished(const QString &result);  // 读取完成后发出结果的信号
    void triggerPowerButton();
    void triggerNewSignal(float param1, float param2);
    void fileChangedSignal(const QString &filePath);  // 新增信号


protected:
    void run();  // 线程入口函数

private:
    QString filePath;
    QFileSystemWatcher *watcher;
    void readExcelFile();
    MainWindow *mainWindow;

    QTimer *remotetimer; // 声明为成员变量
private slots:
    // 当文件发生变化时调用/*
    // void onFileChanged(const QString &path);
};


#endif // EXCELREADER_H

