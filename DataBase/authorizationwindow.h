#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class AuthorizationWindow; }
QT_END_NAMESPACE

class AuthorizationWindow : public QMainWindow
{
    Q_OBJECT

public:
    AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();

private slots:

    void on_signInPushButton_clicked();

private:
    Ui::AuthorizationWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
};
#endif // AUTHORIZATIONWINDOW_H
