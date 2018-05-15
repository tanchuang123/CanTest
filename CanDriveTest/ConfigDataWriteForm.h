#ifndef  CONFIGDATAWRITEFORM_H
#define CONFIGDATAWRITEFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "Setting/setting.h"
#include "Message/WriteDataDialog.h"
#include "ConfigDataForm.h"
namespace Ui {
class ConfigDataWriteForm;
}

class ConfigDataWriteForm : public QWidget
{
    Q_OBJECT

public:
    static ConfigDataWriteForm * init();
    explicit ConfigDataWriteForm(QWidget *parent = 0);
    void initTableWidget();
    ~ConfigDataWriteForm();
public slots:
    void cellSelected( int nRow, int nCol);
    void slotTableWidgetItem(int,QString);
private:
    static ConfigDataWriteForm * pConfigDataWriteForm;
    Ui::ConfigDataWriteForm *ui;
};

#endif // CONFIGDATAWRITEFORM_H
