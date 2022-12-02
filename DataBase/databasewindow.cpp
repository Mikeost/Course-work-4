#include "databasewindow.h"
#include "ui_databasewindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QFile>
#include <QDir>
#include <QDesktopServices>

DataBaseWindow::DataBaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataBaseWindow)
{
    ui->setupUi(this);

    dataBaseInit();

    ui->tabWidget->setTabVisible(6, false);
    ui->tabWidget->setTabVisible(7, false);
    ui->tabWidget->setTabVisible(8, false);
    ui->tabWidget->setTabVisible(9, false);
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

    ui->tabWidget->setTabVisible(6, false);
    ui->tabWidget->setTabVisible(7, false);
    ui->tabWidget->setTabVisible(8, false);
    ui->tabWidget->setTabVisible(9, false);
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


void DataBaseWindow::on_filterTypePushButton_clicked(bool checked)
{
    if(checked){
        ui->filterTypePushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Справа` "
                             "WHERE `Тип справи` = '" + ui->filterTypeComboBox->currentText() + "'");
        ui->criminalCaseTableView->setModel(queryModel);
        ui->criminalCaseTableView->resizeColumnsToContents();
    }
    else{
        ui->filterTypePushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_filterRankPushButton_clicked(bool checked)
{
    if(checked){
        if(ui->filterRankLineEdit->text().isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть звання!");
            ui->filterRankLineEdit->setFocus();
            return;
        }
        ui->filterRankPushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Слідчий` "
                             "WHERE `Звання` = '" + ui->filterRankLineEdit->text() + "'");
        ui->detectiveTableView->setModel(queryModel);
        ui->detectiveTableView->resizeColumnsToContents();
    }
    else{
        ui->filterRankPushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_filterWitnessGroupCodePushButton_clicked(bool checked)
{
    if(checked){
        if(ui->filterWitnessGroupCodeLineEdit->text().isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть код групи!");
            ui->filterWitnessGroupCodeLineEdit->setFocus();
            return;
        }
        ui->filterWitnessGroupCodePushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Група свідків` "
                             "WHERE `Код групи` = '" + ui->filterWitnessGroupCodeLineEdit->text() + "'");
        ui->witnessGroupTableView->setModel(queryModel);
        ui->witnessGroupTableView->resizeColumnsToContents();
    }
    else{
        ui->filterWitnessGroupCodePushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_filterWitnessSurnamePushButton_clicked(bool checked)
{
    if(checked){
        if(ui->filterWitnessSurnameLineEdit->text().isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть прізвище!");
            ui->filterWitnessSurnameLineEdit->setFocus();
            return;
        }
        ui->filterWitnessSurnamePushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Свідок` "
                             "WHERE `Прізвище` = '" + ui->filterWitnessSurnameLineEdit->text() + "'");
        ui->witnessTableView->setModel(queryModel);
        ui->witnessTableView->resizeColumnsToContents();
    }
    else{
        ui->filterWitnessSurnamePushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_filterSuspectsGroupCodePushButton_clicked(bool checked)
{
    if(checked){
        if(ui->filterSuspectsGroupCodeLineEdit->text().isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть код групи!");
            ui->filterSuspectsGroupCodeLineEdit->setFocus();
            return;
        }
        ui->filterSuspectsGroupCodePushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Група підозрюваних` "
                             "WHERE `Код групи` = '" + ui->filterSuspectsGroupCodeLineEdit->text() + "'");
        ui->suspectsGroupTableView->setModel(queryModel);
        ui->suspectsGroupTableView->resizeColumnsToContents();
    }
    else{
        ui->filterSuspectsGroupCodePushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_filterSuspectSurnamePushButton_clicked(bool checked)
{
    if(checked){
        if(ui->filterSuspectSurnameLineEdit->text().isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть прізвище!");
            ui->filterSuspectSurnameLineEdit->setFocus();
            return;
        }
        ui->filterSuspectSurnamePushButton->setText("Скасувати");
        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT * "
                             "FROM `Підозрюваний` "
                             "WHERE `Прізвище` = '" + ui->filterSuspectSurnameLineEdit->text() + "'");
        ui->suspectTableView->setModel(queryModel);
        ui->suspectTableView->resizeColumnsToContents();
    }
    else{
        ui->filterSuspectSurnamePushButton->setText("Фільтрувати");
        on_actionreloadDataBase_triggered();
    }
}


void DataBaseWindow::on_actionFirstQuery_triggered()
{
    ui->tabWidget->setTabVisible(6, true);
    ui->tabWidget->setCurrentIndex(6);
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT `Тип справи`, COUNT(`Кількість томів`) as `Кількість томів` "
                         "FROM `Справа` "
                         "GROUP BY `Тип справи`");
    ui->firstQueryTableView->setModel(queryModel);
    ui->firstQueryTableView->resizeColumnsToContents();
}


void DataBaseWindow::on_actionSecondQuery_triggered()
{
    QString firstDate;
    QString secondDate;
    bool isContinue = false;
    while(true){
        firstDate = QInputDialog::getText(this, tr("Початок періоду"),
                                                tr("Введіть початок періоду (yyyy-mm-dd):"),
                                                QLineEdit::Normal, "2022-01-01", &isContinue);

        if(!isContinue){
            return;
        }
        else{
            isContinue = false;
        }

        if(firstDate.isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть початок періоду!");
        }
        else break;
    }

    while(true){
        secondDate = QInputDialog::getText(this, tr("Кінець періоду"),
                                                 tr("Введіть кінець періоду (yyyy-mm-dd):"),
                                                 QLineEdit::Normal, "2022-05-10", &isContinue);

        if(!isContinue){return;}

        if(secondDate.isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть кінець періоду!");
        }
        else break;
    }

    ui->tabWidget->setTabVisible(7, true);
    ui->tabWidget->setCurrentIndex(7);
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT * FROM `Справа` WHERE `Дата відкриття` > '" + firstDate + "' AND `Дата відкриття` < '" + secondDate + "'");
    ui->secondQueryTableView->setModel(queryModel);
    ui->secondQueryTableView->resizeColumnsToContents();
}


void DataBaseWindow::on_actionThirdQuery_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    QString criminalCaseNumber;
    bool isContinue = false;
    while(true){
        criminalCaseNumber = QInputDialog::getText(this, tr("Введення справи"),
                                                         tr("Введіть номер справи:"),
                                                         QLineEdit::Normal, "", &isContinue);
        if(!isContinue){return;}

        if(criminalCaseNumber.isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть номер справи!");
        }
        else break;
    }

    ui->tabWidget->setTabVisible(8, true);
    ui->tabWidget->setCurrentIndex(8);
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT `Група свідків`.`Номер справи`, `Свідок`.*"
                         "FROM `Свідок`, `Група свідків`"
                         "WHERE `Група свідків`.`Номер справи` = '" + criminalCaseNumber +
                         "' AND `Група свідків`.`Код свідка` = `Свідок`.`Код свідка`");
    ui->thirdQueryTableView->setModel(queryModel);
    ui->thirdQueryTableView->resizeColumnsToContents();
}


void DataBaseWindow::on_actionFourthQuery_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    QString criminalCaseNumber;
    bool isContinue = false;
    while(true){
        criminalCaseNumber = QInputDialog::getText(this, tr("Введення справи"),
                                                         tr("Введіть номер справи:"),
                                                         QLineEdit::Normal, "", &isContinue);
        if(!isContinue){return;}

        if(criminalCaseNumber.isEmpty()){
            QMessageBox::information(this, "Помилка!", "Введіть номер справи!");
        }
        else break;
    }
    ui->tabWidget->setTabVisible(9, true);
    ui->tabWidget->setCurrentIndex(9);
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT `Група підозрюваних`.`Номер справи`, `Підозрюваний`.*"
                         "FROM `Підозрюваний`, `Група підозрюваних`"
                         "WHERE `Група підозрюваних`.`Номер справи` = '" + criminalCaseNumber +
                         "' AND `Група підозрюваних`.`Код підозрюваного` = `Підозрюваний`.`Код підозрюваного`");
    ui->fourthQueryTableView->setModel(queryModel);
    ui->fourthQueryTableView->resizeColumnsToContents();
}

void DataBaseWindow::on_actionQueries_triggered()
{
    bool isOk = false;
    QStringList queries = { "Інформація про кількість томів кожного типу справи",
                            "Інформація про справи за заданим періодом",
                            "Інформація про свідків, які задіяні в заданій справі",
                            "Інформація про підозрюваних, які задіяні в заданій справі" };
    QString query = QInputDialog::getItem(this, "Запит", "Оберіть запит:", queries, 0, QInputDialog::UseListViewForComboBoxItems, &isOk);

    if(!isOk) return;

    if(query == queries.at(0)){
        on_actionFirstQuery_triggered();
    }
    else if(query == queries.at(1)){
        on_actionSecondQuery_triggered();
    }
    else if(query == queries.at(2)){
        on_actionThirdQuery_triggered();
    }
    else if(query == queries.at(3)){
        on_actionFourthQuery_triggered();
    }
}


void DataBaseWindow::on_actionReportCriminalCase_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Справа\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Справа\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Справа\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Номер справи</th>"
                                "<th>Найменування</th>"
                                "<th>Тип справи</th>"
                                "<th>Підстава</th>"
                                "<th>Дата відкриття</th>"
                                "<th>Дата закриття</th>"
                                "<th>Кількість томів</th>"
                                "<th>Табельний номер слідчого</th>"
                            "</tr>";
        for(int row = 0; row < ui->criminalCaseTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->criminalCaseTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->criminalCaseTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }

}


void DataBaseWindow::on_actionReportDetective_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Слідчий\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Слідчий\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Слідчий\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Табельний номер</th>"
                                "<th>Прізвище</th>"
                                "<th>Ім'я</th>"
                                "<th>По батькові</th>"
                                "<th>Звання</th>"
                                "<th>Телефон</th>"
                            "</tr>";
        for(int row = 0; row < ui->detectiveTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->detectiveTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->detectiveTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }
}


void DataBaseWindow::on_actionReportGroupWitnesses_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Група свідків\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Група свідків\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Група свідків\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Код групи</th>"
                                "<th>Номер справи</th>"
                                "<th>Код свідка</th>"
                            "</tr>";
        for(int row = 0; row < ui->witnessGroupTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->witnessGroupTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->witnessGroupTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }
}


void DataBaseWindow::on_actionReportWitness_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Свідок\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Свідок\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Свідок\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Код свідка</th>"
                                "<th>Прізвище</th>"
                                "<th>Ім'я</th>"
                                "<th>По батькові</th>"
                                "<th>Дата народження</th>"
                                "<th>Телефон</th>"
                            "</tr>";
        for(int row = 0; row < ui->witnessTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->witnessTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->witnessTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }
}


void DataBaseWindow::on_actionReportGroupSuspects_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Група підозрюваних\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Група підозрюваних\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Група підозрюваних\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Код групи</th>"
                                "<th>Номер справи</th>"
                                "<th>Код підозрюваного</th>"
                            "</tr>";
        for(int row = 0; row < ui->suspectsGroupTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->suspectsGroupTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->suspectsGroupTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }
}


void DataBaseWindow::on_actionReportSuspect_triggered()
{
    QString path = QDir::currentPath() + QString("Звіт за таблицею \"Підозрюваний\".html");
    QFile file(path);
    QString firstReport;
    if(!file.open(QIODevice::WriteOnly)){
        file.close();
    }
    else {
        firstReport += "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                            "<title>Звіт за таблицею \"Підозрюваний\"</title>"
                            "<style>"
                                "table {"
                                  "border-collapse: collapse;"
                                  "width: 100%;"
                                "}"
                                "th, td {"
                                  "text-align: center;"
                                  "padding: 8px;"
                                "}"

                                "tr:nth-child(even) {"
                                  "background-color: orange;"
                                "}"
                            "</style>"
                       "</head>"
                       "<body>"
                            "<center><h1>Звіт за таблицею \"Підозрюваний\"</h1></center>"
                            "<center><table>"
                            "<tr>"
                                "<th>Код підозрюваного</th>"
                                "<th>Прізвище</th>"
                                "<th>Ім'я</th>"
                                "<th>По батькові</th>"
                                "<th>Дата народження</th>"
                                "<th>Телефон</th>"
                                "<th>Місце проживання</th>"
                            "</tr>";
        for(int row = 0; row < ui->suspectTableView->model()->rowCount(); ++row){
            firstReport += "<tr>";
            for(int col = 0; col < ui->suspectTableView->model()->columnCount(); ++col){
                firstReport += "<td>" + ui->suspectTableView->model()->index(row, col).data().toString() + "</td>";
            }
            firstReport += "</tr>";
        }

        firstReport += "</table></center></body></html>";
        file.write(firstReport.toUtf8());
        file.close();
        QDesktopServices::openUrl(QUrl("file://" + path, QUrl::TolerantMode));
    }
}


void DataBaseWindow::on_actionReports_triggered()
{
    bool isOk = false;
    QStringList reports = { "Звіти за таблицями", "Звіти за запросами", };
    QString reportChoice = QInputDialog::getItem(this, "Звіти", "Оберіть вид звіту:", reports, 0, QInputDialog::UseListViewForComboBoxItems, &isOk);

    if(!isOk) return;

    if(reportChoice == reports.at(0)){
        QStringList reportsTable = { "Звіт за табл. \"Справа\"", "Звіт за табл. \"Слідчий\"",
                                     "Звіт за табл. \"Група свідків\"", "Звіт за табл. \"Свідок\"",
                                     "Звіт за табл. \"Група підозрюваних\"", "Звіт за табл. \"Підозрюваний\"" };
        QString reportChoiceSecond = QInputDialog::getItem(this, "Звіти за таблицями", "Оберіть звіт:", reportsTable, 0, QInputDialog::UseListViewForComboBoxItems, &isOk);

        if(!isOk) return;

        if(reportChoiceSecond == reportsTable.at(0)){
            on_actionReportCriminalCase_triggered();
        }
        else if(reportChoiceSecond == reportsTable.at(1)){
            on_actionReportDetective_triggered();
        }
        else if(reportChoiceSecond == reportsTable.at(2)){
            on_actionReportGroupWitnesses_triggered();
        }
        else if(reportChoiceSecond == reportsTable.at(3)){
            on_actionReportWitness_triggered();
        }
        else if(reportChoiceSecond == reportsTable.at(4)){
            on_actionReportGroupSuspects_triggered();
        }
        else if(reportChoiceSecond == reportsTable.at(5)){
            on_actionReportSuspect_triggered();
        }
    }
    else if(reportChoice == reports.at(1)){

    }
}

