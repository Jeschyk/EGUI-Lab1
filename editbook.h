#ifndef EDITBOOK_H
#define EDITBOOK_H
#include <QMainWindow>
#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class editbook;
}

class editbook : public QDialog
{
    Q_OBJECT

public:
    explicit editbook(QWidget *parent = 0);
    ~editbook();

    QString Author() const, Title() const, Year() const;

    void Setter(const QString &Author, const QString &Title, const QString &Year, bool flag);

private slots:
    void on_pushButton_clicked();

private:
    Ui::editbook *ui;

};

#endif // EDITBOOK_H
