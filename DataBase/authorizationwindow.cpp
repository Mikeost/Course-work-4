#include "authorizationwindow.h"
#include "ui_authorizationwindow.h"
#include "databasewindow.h"
#include <QTableView>
#include <QMessageBox>

AuthorizationWindow::AuthorizationWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);

    db.setConnectOptions("UNIX_SOCKET=/Applications/MAMP/tmp/mysql/mysql.sock");
    db.setHostName("localhost");
    db.setPort(8888);
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("auDB");

    ui->authorizationErrorLabel->setVisible(false);
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}

void AuthorizationWindow::on_signInPushButton_clicked()
{
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    db.open();
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    QTableView *tableView = new QTableView(this);
    queryModel->setQuery("SELECT * "
                         "FROM `users`"
                         "WHERE `login` = '" + login + "'"
                         " AND `pass` = '" + password + "'");

    tableView->setModel(queryModel);

    if(tableView->model()->rowCount() > 0){
        ui->authorizationErrorLabel->setVisible(false);
        DataBaseWindow* window = new DataBaseWindow();
        window->show();
        this->hide();
    }
    else{
        ui->authorizationErrorLabel->setVisible(true);
    }

}

