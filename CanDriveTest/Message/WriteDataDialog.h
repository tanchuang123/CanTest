#ifndef WRITEDATADIALOG_H
#define WRITEDATADIALOG_H

#include <QDialog>
#include "ControlCanDll.h"
namespace Ui {
class WriteDataDialog;
}

class WriteDataDialog : public QDialog
{
    Q_OBJECT

public:
    static WriteDataDialog *init();
    explicit WriteDataDialog(QWidget *parent = 0);
    ~WriteDataDialog();
    void GetStatusValue(const QString &text , int value,QString &type);
private slots:
    void on_pushButton_WriteOK_clicked();
 signals:
    void signalValue(int,QString);
private:
    static WriteDataDialog *pWriteDataDialog;
    Ui::WriteDataDialog *ui;
    QString _Type="";
};

#endif // WRITEDATADIALOG_H
