#include "login.h"
#include "ui_login.h"
#include <QTimer>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
    , mainWindow(nullptr)  // 显式初始化为 nullptr
     //remoteWindow(nullptr)  // 显式初始化为 nullptr
    , selfTestWindow(nullptr)
{
    ui->setupUi(this);

}

login::~login()
{
    delete ui;
    // 适当地管理窗口对象
    if (mainWindow) {
        delete mainWindow;
    }
    //if (remoteWindow) {
//delete remoteWindow;
   // }
}

void login::on_localControlButton_clicked()
{
    if (!mainWindow) {
        mainWindow = new MainWindow();
        connect(mainWindow, &MainWindow::closed, this, &login::mainWindow_closed);
    }

    // if (!selfTestWindow) {
    //     selfTestWindow = new Self_Test();  // 确保 selfTestWindow 已经初始化
    // }

    this->hide();  // 隐藏 login 界面
    mainWindow->show();
    // QTimer::singleShot(15000, this, [this]() {
    //     selfTestWindow->hide();  // 隐藏 selfTestWindow
    //     mainWindow->show();      // 显示 MainWindow
    // });
}

// void login::on_localControlButton_clicked()
// {
//     if (!mainWindow) {
//         mainWindow = new MainWindow();
//         connect(mainWindow, &MainWindow::closed, this, &login::mainWindow_closed);
//     }
//     mainWindow->show();
//     this->hide();  // 显示 MainWindow 后隐藏 login 界面
// }


//void login::on_autoRunButton_clicked()
//=/{

//    if (!remoteWindow) {
 //         remoteWindow = new Remote();
  //      connect(remoteWindow, &Remote::closed, this, &login::remoteWindow_closed);
  //  }
//    remoteWindow->show();//
//remotesys = true;
   // this->hide();  // 显示 MainWindow 后隐藏 login 界面

//}

void login::mainWindow_closed()
{
    this->show();  // 当 MainWindow 关闭时，重新显示 login 界面
    delete mainWindow;
    mainWindow = nullptr;
}
void login::remoteWindow_closed()
{
    this->show();  // 当 MainWindow 关闭时，重新显示 login 界面
   // delete remoteWindow;
    //remoteWindow = nullptr;
}
