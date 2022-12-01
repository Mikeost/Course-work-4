#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "criminalcaserecordwindow.h"

namespace Ui {
class DataBaseWindow;
}

class DataBaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataBaseWindow(QWidget *parent = nullptr);
    ~DataBaseWindow();

public slots:
    void on_actionreloadDataBase_triggered();

private slots:
    void dataBaseInit();

    void addCriminalRecord();

    void on_actionAddRecord_triggered();

    void on_actionRemoveRecord_triggered();

    void on_criminalCaseTableView_clicked(const QModelIndex &index);

    void on_detectiveTableView_clicked(const QModelIndex &index);

    void on_witnessGroupTableView_clicked(const QModelIndex &index);

    void on_witnessTableView_clicked(const QModelIndex &index);

    void on_suspectsGroupTableView_clicked(const QModelIndex &index);

    void on_suspectTableView_clicked(const QModelIndex &index);

private:
    Ui::DataBaseWindow *ui;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlTableModel *criminalCaseTableModel = new QSqlTableModel();
    QSqlTableModel *detectiveTableModel = new QSqlTableModel();
    QSqlTableModel *witnessGroupTableModel = new QSqlTableModel();
    QSqlTableModel *witnessTableModel = new QSqlTableModel();
    QSqlTableModel *suspectsGroupTableModel = new QSqlTableModel();
    QSqlTableModel *suspectTableModel = new QSqlTableModel();

    int currentRow;

    CriminalCaseRecordWindow *ccrW;
};

#endif // DATABASEWINDOW_H
