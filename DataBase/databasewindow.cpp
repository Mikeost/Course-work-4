#include "databasewindow.h"
#include "ui_databasewindow.h"
#include <QMessageBox>

DataBaseWindow::DataBaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataBaseWindow)
{
    ui->setupUi(this);

    dataBaseInit();
}

void DataBaseWindow::dataBaseInit(){
    // Налаштування бази даних
    db.setConnectOptions("UNIX_SOCKET=/Applications/MAMP/tmp/mysql/mysql.sock");
    db.setHostName("localhost");
    db.setPort(8888);
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("Слідчий відділ");


    // Перевірка відкриття бд, приєднання таблиць
    if(db.open()){
        // Таблиця "Справа"
        criminalCaseTableModel->setTable("Справа");
        criminalCaseTableModel->select();
        ui->criminalCaseTableView->setModel(criminalCaseTableModel);
        ui->criminalCaseTableView->resizeColumnsToContents();
        ui->criminalCaseTableView->horizontalHeader()->setStretchLastSection(true);

        // Таблиця "Слідчий"
        detectiveTableModel->setTable("Слідчий");
        detectiveTableModel->select();
        ui->detectiveTableView->setModel(detectiveTableModel);
        ui->detectiveTableView->resizeColumnsToContents();
        ui->detectiveTableView->horizontalHeader()->setStretchLastSection(true);

        // Таблиця "Группа свідків"
        witnessGroupTableModel->setTable("Група свідків");
        witnessGroupTableModel->select();
        ui->witnessGroupTableView->setModel(witnessGroupTableModel);
        ui->witnessGroupTableView->resizeColumnsToContents();
        ui->witnessGroupTableView->horizontalHeader()->setStretchLastSection(true);

        // Таблиця "Свідок"
        witnessTableModel->setTable("Свідок");
        witnessTableModel->select();
        ui->witnessTableView->setModel(witnessTableModel);
        ui->witnessTableView->resizeColumnsToContents();
        ui->witnessTableView->horizontalHeader()->setStretchLastSection(true);

        // Таблиця "Група підозрюваних"
        suspectsGroupTableModel->setTable("Група підозрюваних");
        suspectsGroupTableModel->select();
        ui->suspectsGroupTableView->setModel(suspectsGroupTableModel);
        ui->suspectsGroupTableView->resizeColumnsToContents();
        ui->suspectsGroupTableView->horizontalHeader()->setStretchLastSection(true);

        // Таблиця "Підозрюваний"
        suspectTableModel->setTable("Підозрюваний");
        suspectTableModel->select();
        ui->suspectTableView->setModel(suspectTableModel);
        ui->suspectTableView->resizeColumnsToContents();
        ui->suspectTableView->horizontalHeader()->setStretchLastSection(true);

    }
    else{
        QMessageBox::information(this, "Помилка!", "База даних не відкрилася!");
    }
}

DataBaseWindow::~DataBaseWindow()
{
    delete ui;
}
