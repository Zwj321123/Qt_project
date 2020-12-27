#include "thickness.h"
#include "ui_thickness.h"

thickness::thickness(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::thickness)
{
    ui->setupUi(this);
}

thickness::~thickness()
{
    delete ui;
}

void thickness::on_buttonBox_accepted()
{
    change_thickness = ui->doubleSpinBox->value();
}
