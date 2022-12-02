#include "criminalcaserecordwindow.h"
#include "ui_criminalcaserecordwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableView>

CriminalCaseRecordWindow::CriminalCaseRecordWindow(QString operation, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CriminalCaseRecordWindow)
{
    ui->setupUi(this);

    this->operation = operation;
    if(this->operation == "add"){
        add();
    }
}

void CriminalCaseRecordWindow::add(){
    setWindowTitle("Додання нового запису");
    ui->criminalCaseLabel->setText("Додання нового запису в таблицю \"Справа\"");

    // Наступний номер справи
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT auto_increment "
                         "FROM information_schema.tables "
                         "WHERE table_schema = 'Слідчий відділ' "
                         "AND table_name = 'Справа'");
    QTableView* tableView = new QTableView();
    tableView->setModel(queryModel);
    ui->numberLineEdit->setText(tableView->model()->index(0, 0).data().toString());

    // Задання дати
    ui->createDateEdit->setDate(QDate::currentDate());
    ui->closeDateEdit->setDate(QDate::currentDate().addDays(1));

    // ComboBox з табельними номерами слідчих
    queryModel->setQuery("SELECT `Табельний номер`, `Прізвище`, `Ім'я`, `По батькові`"
                         "FROM `Слідчий`");
    tableView->setModel(queryModel);
    for (int row = 0; row < tableView->model()->rowCount(); ++row) {
        ui->employeeNumberComboBox->addItem(tableView->model()->index(row, 0).data().toString() + " " +
                              tableView->model()->index(row, 1).data().toString() + " " +
                              tableView->model()->index(row, 2).data().toString() + " " +
                              tableView->model()->index(row, 3).data().toString());
    }
}

void CriminalCaseRecordWindow::idxInit(int index){
    this->idx = index;
    edit();
}

void CriminalCaseRecordWindow::edit(){
    setWindowTitle("Редагування запису");
    ui->criminalCaseLabel->setText("Редагування запису в таблиці \"Справа\"");

    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery("SELECT * "
                         "FROM `Справа`");
    QTableView* tableView = new QTableView();
    tableView->setModel(queryModel);
    ui->numberLineEdit->setText(tableView->model()->index(idx, 0).data().toString());
    ui->nameLineEdit->setText(tableView->model()->index(idx, 1).data().toString());
    ui->typeComboBox->setCurrentText(tableView->model()->index(idx, 2).data().toString());
    ui->reasonPlainTextEdit->insertPlainText(tableView->model()->index(idx, 3).data().toString());
    ui->createDateEdit->setDate(tableView->model()->index(idx, 4).data().toDate());
    if(tableView->model()->index(idx, 5).data().toString() == "NULL"){
        ui->closeDateEdit->setEnabled(false);
        ui->underInvestigationCheckBox->setChecked(true);
    }
    else{
        ui->closeDateEdit->setDate(tableView->model()->index(idx, 5).data().toDate());
    }

    ui->countVolumeSpinBox->setValue(tableView->model()->index(idx, 6).data().toInt());

    queryModel->setQuery("SELECT `Табельний номер`, `Прізвище`, `Ім'я`, `По батькові`"
                         "FROM `Слідчий`");
    tableView->setModel(queryModel);
    for (int row = 0; row < tableView->model()->rowCount(); ++row) {
        ui->employeeNumberComboBox->addItem(tableView->model()->index(row, 0).data().toString() + " " +
                              tableView->model()->index(row, 1).data().toString() + " " +
                              tableView->model()->index(row, 2).data().toString() + " " +
                              tableView->model()->index(row, 3).data().toString());
    }

    queryModel->setQuery("SELECT `Табельний номер слідчого`"
                         "FROM `Справа`"
                         "WHERE `Номер справи` = '" + ui->numberLineEdit->text() + "'");
    tableView->setModel(queryModel);

    for(int i = 0; i < ui->employeeNumberComboBox->count(); ++i){
        if(tableView->model()->index(0, 0).data().toString() == ui->employeeNumberComboBox->itemText(i).left(4)){
            ui->employeeNumberComboBox->setCurrentIndex(i);
            break;
        }
    }

}

CriminalCaseRecordWindow::~CriminalCaseRecordWindow()
{
    delete ui;
}

void CriminalCaseRecordWindow::on_underInvestigationCheckBox_clicked(bool checked)
{
    checked ? ui->closeDateEdit->setEnabled(false) : ui->closeDateEdit->setEnabled(true);
}


void CriminalCaseRecordWindow::on_cancelPushButton_clicked()
{
    this->close();
}


void CriminalCaseRecordWindow::on_savePushButton_clicked()
{
    if(ui->nameLineEdit->text().isEmpty()){
        QMessageBox::information(this, "Помилка!", "Введіть найменування справи!");
        ui->nameLineEdit->setFocus();
        return;
    }

    if(ui->reasonPlainTextEdit->toPlainText().isEmpty()){
        QMessageBox::information(this, "Помилка!", "Введіть підставу заведення діла");
        ui->reasonPlainTextEdit->setFocus();
        return;
    }

    if(ui->reasonPlainTextEdit->toPlainText().size() > 100){
        QMessageBox::information(this, "Помилка!", "Текст підстави заведення діла повина бути не більше 100 символів");
        ui->reasonPlainTextEdit->setFocus();
        return;
    }

    if(!ui->underInvestigationCheckBox->isChecked()){
        if(ui->createDateEdit->date() > ui->closeDateEdit->date()){
            QMessageBox::information(this, "Помилка!", "Дата закриття справи потрібна бути не раніше одного дня від її відкриття");
            ui->closeDateEdit->setFocus();
            return;
        }
    }

    if(this->operation == "add"){
        QString queryAdd = "INSERT INTO `Справа` "
                           "VALUES (NULL, '" +
                           ui->nameLineEdit->text() + "', '" +
                           ui->typeComboBox->currentText() + "', '" +
                           ui->reasonPlainTextEdit->toPlainText() + "', '" +
                           ui->createDateEdit->text() + "', ";
        if(ui->underInvestigationCheckBox->isChecked()){
            queryAdd += "NULL, '";
        }
        else{
            queryAdd += "'" + ui->closeDateEdit->text() + "', '";
        }
        queryAdd += ui->countVolumeSpinBox->text() + "', '";

        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT `Табельний номер` "
                             "FROM `Слідчий`");
        QTableView* tableView = new QTableView();
        tableView->setModel(queryModel);
        queryAdd += tableView->model()->index(ui->employeeNumberComboBox->currentIndex(), 0).data().toString() + "')";

        queryModel->setQuery(queryAdd);

        emit signal();

        this->close();
    }
    else{
        QString queryEdit = "UPDATE `Справа` "
                            "SET `Найменування` = '" + ui->nameLineEdit->text() + "', "
                            "`Тип справи` = '" + ui->typeComboBox->currentText() + "', "
                            "`Підстава` = '" + ui->reasonPlainTextEdit->toPlainText() + "', "
                            "`Дата відкриття` = '" + ui->createDateEdit->text() + "', ";
        if(ui->underInvestigationCheckBox->isChecked()){
            queryEdit += "`Дата закриття` = NULL, ";
        }
        else{
            queryEdit += "`Дата закриття` = '" + ui->closeDateEdit->text() + "', ";
        }

        queryEdit += "`Кількість томів` = '" + ui->countVolumeSpinBox->text() + "', ";

        QSqlQueryModel* queryModel = new QSqlQueryModel();
        queryModel->setQuery("SELECT `Табельний номер` "
                                    "FROM `Слідчий`");
        QTableView* tableView = new QTableView();
        tableView->setModel(queryModel);
        queryEdit += "`Табельний номер слідчого` = '" + tableView->model()->index(ui->employeeNumberComboBox->currentIndex(), 0).data().toString() + "' ";
        queryEdit += "WHERE `Номер справи` = '" + ui->numberLineEdit->text() + "'";
        queryModel->setQuery(queryEdit);

        emit signal();

        this->close();
    }
}
