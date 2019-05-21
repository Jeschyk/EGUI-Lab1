#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <ui_mainwindow.h>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "editbook.h"
#include "warning.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_tableView_doubleClicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *csvModel;
    bool isFilter;

    QSortFilterProxyModel *proxyModel;
    QSortFilterProxyModel *proxyModel2;
    QSortFilterProxyModel *proxyModel3;

};

#endif // MAINWINDOW_H
