#include "databasewindow.h"
#include "ui_databasewindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

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

void DataBaseWindow::editCriminalRecord(){
    bool ok;
    currentRow = QInputDialog::getInt(this, tr("Вибір запису для редагування"),
                                            tr("Введіть номер запису для редагування:"),
                                            currentRow + 1, 1, ui->criminalCaseTableView->model()->rowCount(), 1, &ok);

    if(ok){
        --currentRow;

        ccrW = new CriminalCaseRecordWindow("edit");
        connect(this, &DataBaseWindow::signal, ccrW, &CriminalCaseRecordWindow::idxInit);
        ccrW->show();
        emit signal(currentRow);

        connect(ccrW, &CriminalCaseRecordWindow::signal, this, &DataBaseWindow::on_actionreloadDataBase_triggered);
    }
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


void DataBaseWindow::on_actionEditRecord_triggered()
{
    switch(ui->tabWidget->currentIndex()){
    case 0:
        editCriminalRecord();
        break;
    default:
        break;
    }
}

DataBaseWindow::~DataBaseWindow()
{
    delete ui;
}

void DataBaseWindow::on_searchAction_triggered()
{
    QString inputTask;
    bool isFind = false;
    switch(ui->tabWidget->currentIndex()){
        case 0:
             inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть номер справи:"), QLineEdit::Normal);
            for (int var = 0; var < ui->criminalCaseTableView->model()->rowCount(); ++var) {
                        if(ui->criminalCaseTableView->model()->index(var, 0).data() == inputTask){
                            ui->criminalCaseTableView->selectRow(var);
                            ui->criminalCaseTableView->setFocus();
                            return;
                        }
            }
            QMessageBox::information(0, "Повідомлення", "Данного номеру справи немає!");
            break;

        case 1:
            inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть табельний номер слідчого:"), QLineEdit::Normal);
            for (int var = 0; var < ui->detectiveTableView->model()->rowCount(); ++var) {
                        if(ui->detectiveTableView->model()->index(var, 0).data() == inputTask){
                            ui->detectiveTableView->selectRow(var);
                            ui->detectiveTableView->setFocus();
                            return;
                        }
            }
            QMessageBox::information(0, "Повідомлення", "Данного табельного номеру немає!");
            break;

        case 2:
            inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть код групи свідків:"), QLineEdit::Normal);
            ui->witnessGroupTableView->clearSelection();
            for (int var = 0; var < ui->witnessGroupTableView->model()->rowCount(); ++var) {
                        if(ui->witnessGroupTableView->model()->index(var, 0).data() == inputTask){
                            ui->witnessGroupTableView->selectRow(var);
                            ui->witnessGroupTableView->setFocus();
                            isFind = true;
                        }
            }
            if(!isFind){
                QMessageBox::information(0, "Повідомлення", "Данного коду групи свідків немає!");
            }
            break;

        case 3:
            inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть код свідка:"), QLineEdit::Normal);
            for (int var = 0; var < ui->witnessTableView->model()->rowCount(); ++var) {
                        if(ui->witnessTableView->model()->index(var, 0).data() == inputTask){
                            ui->witnessTableView->selectRow(var);
                            ui->witnessTableView->setFocus();
                            return;
                        }
            }
            QMessageBox::information(0, "Повідомлення", "Данного коду свідка немає!");
            break;

        case 4:
            inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть код групи підозрюваних:"), QLineEdit::Normal);
            ui->suspectsGroupTableView->clearSelection();
            for (int var = 0; var < ui->suspectsGroupTableView->model()->rowCount(); ++var) {
                        if(ui->suspectsGroupTableView->model()->index(var, 0).data() == inputTask){
                            ui->suspectsGroupTableView->selectRow(var);
                            ui->suspectsGroupTableView->setFocus();
                            isFind = true;
                        }
            }
            if(!isFind){
                QMessageBox::information(0, "Повідомлення", "Данного коду групи підозрюваних немає!");
            }
            break;

        case 5:
            inputTask = QInputDialog::getText(this, tr("Пошук за ключовим полем"),
                                                            tr("Введіть код підозрюваного:"), QLineEdit::Normal);
            for (int var = 0; var < ui->suspectTableView->model()->rowCount(); ++var) {
                        if(ui->suspectTableView->model()->index(var, 0).data() == inputTask){
                            ui->suspectTableView->selectRow(var);
                            ui->suspectTableView->setFocus();
                            return;
                        }
            }
            QMessageBox::information(0, "Повідомлення", "Данного коду підозрюваного немає!");
            break;
        }
}

