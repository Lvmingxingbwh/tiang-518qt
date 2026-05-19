#ifndef REMOTEUI_H
#define REMOTEUI_H

#include <QMainWindow>

namespace Ui {
class RemoteUI;
}

class RemoteUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteUI(QWidget *parent = nullptr);
    ~RemoteUI();

signals:
    void remoteButtonClicked();  // 定义按钮按下的信号

public slots:
    void on_remoteButton_clicked();  // 槽函数，响应按钮按下事件

private:
    Ui::RemoteUI *ui;
};

#endif // REMOTEUI_H
