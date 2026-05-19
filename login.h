#ifndef LOGIN_H
#define LOGIN_H
#include "mainwindow.h"
#include "remote.h"
#include "self_test.h"
#include <QWidget>
#include "ui_Self_test.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT


public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void on_autoRunButton_clicked();
    //Remote *remoteWindow;
    bool remotesys =false;
private slots:

    void on_localControlButton_clicked();


    void mainWindow_closed();
    void remoteWindow_closed();
signals:

    void localControlSelected();
    void autoRunSelected();

private:
    Ui::login *ui;
    MainWindow *mainWindow;  // Pointer for MainWindow

    Self_Test *selfTestWindow;

    ;    // Pointer for Remote
};

#endif // LOGIN_H
