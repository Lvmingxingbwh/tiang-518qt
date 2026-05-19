#ifndef MECHANICAL_ARM_H
#define MECHANICAL_ARM_H

#include <QWidget>

namespace Ui {
class Mechanical_Arm;
}

class Mechanical_Arm : public QWidget
{
    Q_OBJECT

public:
    explicit Mechanical_Arm(QWidget *parent = nullptr);
    ~Mechanical_Arm();

private:
    Ui::Mechanical_Arm *ui;
};

#endif // MECHANICAL_ARM_H
