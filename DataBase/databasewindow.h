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

    void editCriminalRecord();

    void on_actionAddRecord_triggered();

    void on_actionRemoveRecord_triggered();

    void on_criminalCaseTableView_clicked(const QModelIndex &index);

    void on_detectiveTableView_clicked(const QModelIndex &index);

    void on_witnessGroupTableView_clicked(const QModelIndex &index);

    void on_witnessTableView_clicked(const QModelIndex &index);

    void on_suspectsGroupTableView_clicked(const QModelIndex &index);

    void on_suspectTableView_clicked(const QModelIndex &index);

    void on_actionEditRecord_triggered();

    void on_searchAction_triggered();

    void on_filterTypePushButton_clicked(bool checked);

    void on_filterRankPushButton_clicked(bool checked);

    void on_filterWitnessGroupCodePushButton_clicked(bool checked);

    void on_filterWitnessSurnamePushButton_clicked(bool checked);

    void on_filterSuspectsGroupCodePushButton_clicked(bool checked);

    void on_filterSuspectSurnamePushButton_clicked(bool checked);

    void on_actionFirstQuery_triggered();

    void on_actionSecondQuery_triggered();

    void on_actionThirdQuery_triggered();

    void on_actionFourthQuery_triggered();

    void on_actionQueries_triggered();

    void on_actionReportCriminalCase_triggered();

    void on_actionReportDetective_triggered();

    void on_actionReportGroupWitnesses_triggered();

    void on_actionReportWitness_triggered();

    void on_actionReportGroupSuspects_triggered();

    void on_actionReportSuspect_triggered();

    void on_actionReports_triggered();

    void on_actionReportFirstQuery_triggered();

    void on_actionReportSecondQuery_triggered();

    void on_actionReportThirdQuery_triggered();

    void on_actionReportFourthQuery_triggered();

    void on_actionHelp_triggered();

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

signals:
    void signal(int);
};

#endif // DATABASEWINDOW_H
