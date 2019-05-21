#include "editbook.h"
#include "ui_editbook.h"
#include "mainwindow.h"

editbook::editbook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editbook)
{
    ui->setupUi(this);

    ui->comboBox->addItem("");

    for(int i=2019; i>=1900; i--)
    {
        ui->comboBox->addItem(QString::number(i));
    }
}

editbook::~editbook()
{
    delete ui;
}
void editbook::on_pushButton_clicked()
{
    accept();
}

QString editbook::Author() const
{
    return ui->lineEdit->text();
}
QString editbook::Title() const
{
    return ui->lineEdit_2->text();
}
QString editbook::Year() const
{
    return ui->comboBox->currentText();
}
void editbook::Setter(const QString &Author, const QString &Title, const QString &Year, bool flag)
{
    if(flag==true)
    {
        ui->lineEdit->setText(Author);
        ui->lineEdit_2->setText(Title);
        ui->comboBox->setCurrentText(Year);
        ui->pushButton->setText("Save");
    }
    else
        ui->pushButton->setText("Add");

}
