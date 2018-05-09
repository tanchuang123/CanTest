#ifndef CONFIGDATAWRITEFORM_H
#define CONFIGDATAWRITEFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "Setting/setting.h"
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
private:
    static ConfigDataWriteForm * pConfigDataWriteForm;
    Ui::ConfigDataWriteForm *ui;
};

#endif // CONFIGDATAWRITEFORM_H
