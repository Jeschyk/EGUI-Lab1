#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QIODevice>
#include <QSortFilterProxyModel>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel2 = new QSortFilterProxyModel(this);
    proxyModel3 = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(csvModel);
    proxyModel2->setSourceModel(proxyModel);
    proxyModel3->setSourceModel(proxyModel2);
    ui->tableView->setModel(proxyModel3);
    isFilter=false;

    csvModel->setColumnCount(3);
    this->setWindowTitle("Library");
    csvModel->setHorizontalHeaderLabels(QStringList() << "Author" << "Title" << "Year");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:lightgray }");
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setModel(csvModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFile file("database.csv");
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File does not exist";
    }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QList<QStandardItem *> standardItemsList;
            for (QString item : line.split(";"))
            {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
        }
        file.close();
    }
    ui->comboBox->addItem("");//comboBox fill

    for(int i=2019; i>=1900; i--)
    {
        int rows=csvModel->rowCount();

        for(int j=0; j<rows; j++)
        {
            if(csvModel->data(csvModel->index(j,2)).toString()==QString::number(i)&&ui->comboBox->currentText()!=QString::number(i))
            {
                ui->comboBox->addItem(QString::number(i));
                break;
            }
        }
    }
}

MainWindow::~MainWindow() //saving into the file
{
    delete ui;

    QString textData;

    int rows=csvModel->rowCount();
    int columns=csvModel->columnCount();

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            textData += csvModel->data(csvModel->index(i,j)).toString();
            textData += ";";
        }
        textData=textData.left(textData.length()-1);
        textData += "\n";
    }

    QFile csvFile("database.csv");
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&csvFile);
        out << textData;
        csvFile.close();
    }


}

void MainWindow::on_pushButton_clicked() //delete
{
    QItemSelection select = ui->tableView->selectionModel()->selection();

    QList <int> RowsToRemove;
    foreach(QModelIndex index,select.indexes())
    {
        RowsToRemove.append(index.row());
    }
    qSort(RowsToRemove);
    int prev = -1;
    for(int i = RowsToRemove.count() - 1; i >= 0; i -= 1)
    {
        int current = RowsToRemove[i];
        if(current != prev)
        {
            if(isFilter==true)
            {
                proxyModel3->removeRows(current, 1);
            }
            else
            {
                csvModel->removeRows(current, 1);
            }
            prev = current;
        }
    }
    ui->comboBox->clear();
    ui->comboBox->addItem("");//comboBox fill

    for(int i=2019; i>=1900; i--)
    {
        int rows=csvModel->rowCount();

        for(int j=0; j<rows; j++)
        {
            if(csvModel->data(csvModel->index(j,2)).toString()==QString::number(i)&&ui->comboBox->currentText()!=QString::number(i))
            {
                ui->comboBox->addItem(QString::number(i));
                break;
            }
        }
    }
}

void MainWindow::on_pushButton_3_clicked() //add
{
    QString Author, Title, Year;

    editbook AddBook(this);
    AddBook.Setter(Author,Title,Year,false);
    int result = AddBook.exec();
    if(result == QDialog::Rejected)
        return;

    Author = AddBook.Author();
    Title = AddBook.Title();
    Year = AddBook.Year();

    if(!Author.isEmpty()&&!Title.isEmpty()&&!Year.isEmpty())
    {
        QList<QStandardItem*> standardItemsList;
        standardItemsList.append((new QStandardItem(Author)));
        standardItemsList.append((new QStandardItem(Title)));
        standardItemsList.append((new QStandardItem(Year)));
        csvModel->insertRow(csvModel->rowCount(), standardItemsList);
    }
    ui->comboBox->clear();
    ui->comboBox->addItem("");//comboBox fill

    for(int i=2019; i>=1900; i--)
    {
        int rows=csvModel->rowCount();

        for(int j=0; j<rows; j++)
        {
            if(csvModel->data(csvModel->index(j,2)).toString()==QString::number(i)&&ui->comboBox->currentText()!=QString::number(i))
            {
                ui->comboBox->addItem(QString::number(i));
                break;
            }
        }
    }
    ui->tableView->setSortingEnabled(true);

}

void MainWindow::on_tableView_doubleClicked() //edit
{


    int select = ui->tableView->selectionModel()->currentIndex().row();

    editbook EditBook(this);

    QString currAuthor, currTitle, currYear;

    if(isFilter==false){
        currAuthor=csvModel->data(csvModel->index(select,0)).toString();
        currTitle=csvModel->data(csvModel->index(select,1)).toString();
        currYear=csvModel->data(csvModel->index(select,2)).toString();
    }
    if(isFilter==true){
        currAuthor=proxyModel3->data(proxyModel3->index(select,0)).toString();
        currTitle=proxyModel3->data(proxyModel3->index(select,1)).toString();
        currYear=proxyModel3->data(proxyModel3->index(select,2)).toString();
    }

    EditBook.Setter(currAuthor,currTitle,currYear,true);
    int result = EditBook.exec();
    if(result == QDialog::Rejected)
        return;

    QString Author, Title, Year;

    Author = EditBook.Author();
    Title = EditBook.Title();
    Year = EditBook.Year();


    if(!Author.isEmpty()&&!Title.isEmpty()&&!Year.isEmpty())
    {
        QList<QStandardItem*> standardItemsList;
        standardItemsList.append((new QStandardItem(Author)));
        standardItemsList.append((new QStandardItem(Title)));
        standardItemsList.append((new QStandardItem(Year)));
        if(isFilter==false){
            csvModel->insertRow(select, standardItemsList);
            csvModel->removeRow(select+1);
        }
        if(isFilter==true){
            Author=proxyModel3->data(proxyModel3->index(select,0)).toString();
            Title=proxyModel3->data(proxyModel3->index(select,1)).toString();
            Year=proxyModel3->data(proxyModel3->index(select,2)).toString();
            for(int i=0;i<csvModel->rowCount();++i)
            {
                if(csvModel->data(csvModel->index(i,0)).toString()==Author&&csvModel->data(csvModel->index(i,1)).toString()==Title&&csvModel->data(csvModel->index(i,2)).toString()==Year)
                {
                    csvModel->insertRow(i, standardItemsList);
                    csvModel->removeRow(i+1);
                }
            }
        }
    }
    else
    {
        warning Warning(this);
        Warning.exec();
    }
    ui->comboBox->clear();
    ui->comboBox->addItem("");//comboBox fill

    for(int i=2019; i>=1900; i--)
    {
        int rows=csvModel->rowCount();

        for(int j=0; j<rows; j++)
        {
            if(csvModel->data(csvModel->index(j,2)).toString()==QString::number(i)&&ui->comboBox->currentText()!=QString::number(i))
            {
                ui->comboBox->addItem(QString::number(i));
                break;
            }
        }
    }
    ui->tableView->setSortingEnabled(true);

}

void MainWindow::on_pushButton_4_clicked()  //filter
{

    proxyModel->setFilterKeyColumn(1);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterFixedString(ui->lineEdit->text());

    proxyModel2->setFilterKeyColumn(0);
    proxyModel2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel2->setFilterFixedString(ui->lineEdit_2->text());

    proxyModel3->setFilterKeyColumn(2);
    proxyModel3->setFilterFixedString(ui->comboBox->currentText());

    ui->tableView->setModel(proxyModel3);

    isFilter=true;

}

void MainWindow::on_pushButton_5_clicked() //clear
{
    ui->tableView->setModel(csvModel);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->comboBox->clear();
    ui->comboBox->addItem("");

    for(int i=2019; i>=1900; i--)
    {
        ui->comboBox->addItem(QString::number(i));
    }

    isFilter=false;
}
