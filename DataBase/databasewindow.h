#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class DataBaseWindow;
}

class DataBaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataBaseWindow(QWidget *parent = nullptr);
    ~DataBaseWindow();

    void dataBaseInit();

private:
    Ui::DataBaseWindow *ui;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlTableModel *criminalCaseTableModel = new QSqlTableModel();
    QSqlTableModel *detectiveTableModel = new QSqlTableModel();
    QSqlTableModel *witnessGroupTableModel = new QSqlTableModel();
    QSqlTableModel *witnessTableModel = new QSqlTableModel();
    QSqlTableModel *suspectsGroupTableModel = new QSqlTableModel();
    QSqlTableModel *suspectTableModel = new QSqlTableModel();
};

#endif // DATABASEWINDOW_H
