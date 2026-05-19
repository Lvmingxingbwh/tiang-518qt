#include "mechanical_arm.h"
#include "ui_mechanical_arm.h"

Mechanical_Arm::Mechanical_Arm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Mechanical_Arm)
{
    ui->setupUi(this);
}

Mechanical_Arm::~Mechanical_Arm()
{
    delete ui;
}
