#ifndef SYSTEMSTATUSFORM_H
#define SYSTEMSTATUSFORM_H

#include <QWidget>
#include "ControlCanDll.h"
#include <QMap>
#include <QVariant>
namespace Ui {
class SystemStatusForm;
}

class SystemStatusForm : public QWidget
{
    Q_OBJECT

public:
    static SystemStatusForm*init();
    explicit SystemStatusForm(QWidget *parent = 0);
    ~SystemStatusForm();
    void GetSystemStatus();
private:
    static SystemStatusForm *pSystemStatusForm;
    Ui::SystemStatusForm *ui;
};

#endif // SYSTEMSTATUSFORM_H
