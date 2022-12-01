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

void DataBaseWindow::on_actionAddRecord_triggered()
{
    switch(ui->tabWidget->currentIndex()){
    case 0:
        addCriminalRecord();
        break;
    case 1:
        detectiveTableModel->insertRow(detectiveTableModel->rowCount());
        break;
    case 2:
        witnessGroupTableModel->insertRow(witnessGroupTableModel->rowCount());
        break;
    case 3:
        witnessTableModel->insertRow(witnessTableModel->rowCount());
        break;
    case 4:
        suspectsGroupTableModel->insertRow(suspectsGroupTableModel->rowCount());
        break;
    case 5:
        suspectTableModel->insertRow(suspectTableModel->rowCount());
        break;
    }
}

void DataBaseWindow::addCriminalRecord(){
    ccrW = new CriminalCaseRecordWindow("add");
    connect(ccrW, &CriminalCaseRecordWindow::signal, this, &DataBaseWindow::on_actionreloadDataBase_triggered);
    ccrW->show();
}

DataBaseWindow::~DataBaseWindow()
{
    delete ui;
}


void DataBaseWindow::on_actionreloadDataBase_triggered()
{
    // Таблиця "Справа"
    criminalCaseTableModel->select();
    ui->criminalCaseTableView->setModel(criminalCaseTableModel);
    ui->criminalCaseTableView->resizeColumnsToContents();

    // Таблиця "Слідчий"
    detectiveTableModel->select();
    ui->detectiveTableView->setModel(detectiveTableModel);
    ui->detectiveTableView->resizeColumnsToContents();

    // Таблиця "Група свідків"
    witnessGroupTableModel->select();
    ui->witnessGroupTableView->setModel(witnessGroupTableModel);
    ui->witnessGroupTableView->resizeColumnsToContents();

    // Таблиця "Свідок"
    witnessTableModel->select();
    ui->witnessTableView->setModel(witnessTableModel);
    ui->witnessTableView->resizeColumnsToContents();

    // Таблиця "Група підозрюваних"
    suspectsGroupTableModel->select();
    ui->suspectsGroupTableView->setModel(suspectsGroupTableModel);
    ui->suspectsGroupTableView->resizeColumnsToContents();

    // Таблиця "Підозрюваний"
    suspectTableModel->select();
    ui->suspectTableView->setModel(suspectTableModel);
    ui->suspectTableView->resizeColumnsToContents();
}


void DataBaseWindow::on_actionRemoveRecord_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Видалення", "Ви дійсно хочете видалити запис №" + QString::number(currentRow + 1) + "?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        switch(ui->tabWidget->currentIndex()){
            case 0:
                criminalCaseTableModel->removeRow(currentRow);
                criminalCaseTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            case 1:
                detectiveTableModel->removeRow(currentRow);
                detectiveTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            case 2:
                witnessGroupTableModel->removeRow(currentRow);
                witnessGroupTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            case 3:
                witnessTableModel->removeRow(currentRow);
                witnessTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            case 4:
                suspectsGroupTableModel->removeRow(currentRow);
                suspectsGroupTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            case 5:
                suspectTableModel->removeRow(currentRow);
                suspectTableModel->select();
                on_actionreloadDataBase_triggered();
                break;
            default:
                break;
            }
    }
}


void DataBaseWindow::on_criminalCaseTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void DataBaseWindow::on_detectiveTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void DataBaseWindow::on_witnessGroupTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void DataBaseWindow::on_witnessTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void DataBaseWindow::on_suspectsGroupTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void DataBaseWindow::on_suspectTableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}

