#include "warning.h"
#include "ui_warning.h"

warning::warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warning)
{
    ui->setupUi(this);
    this->setWindowTitle("Warning");
}

warning::~warning()
{
    delete ui;
}

void warning::on_pushButton_clicked()
{
    close();
}
