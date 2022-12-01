#ifndef CRIMINALCASERECORDWINDOW_H
#define CRIMINALCASERECORDWINDOW_H

#include <QDialog>

namespace Ui {
class CriminalCaseRecordWindow;
}

class CriminalCaseRecordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CriminalCaseRecordWindow(QString operation, QWidget *parent = nullptr);
    ~CriminalCaseRecordWindow();

public slots:
    void idxInit(int);

private slots:
    void add();

    void edit();

    void on_underInvestigationCheckBox_clicked(bool checked);

    void on_cancelPushButton_clicked();

    void on_savePushButton_clicked();

private:
    Ui::CriminalCaseRecordWindow *ui;
    QString operation;
    int idx;

signals:
    void signal();
};

#endif // CRIMINALCASERECORDWINDOW_H
