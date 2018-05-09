#include "SystemStatusForm.h"
#include "ui_SystemStatusForm.h"

SystemStatusForm*SystemStatusForm::pSystemStatusForm=Q_NULLPTR;

SystemStatusForm::SystemStatusForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemStatusForm)
{
    ui->setupUi(this);

    ControlCanDll::init();
}
SystemStatusForm* SystemStatusForm::init()
{
    if(!pSystemStatusForm)
    {
        pSystemStatusForm=new SystemStatusForm;
    }
    return pSystemStatusForm;
}
SystemStatusForm::~SystemStatusForm()
{
    delete ui;
}
void SystemStatusForm::GetSystemStatus()
{
   if( ControlCanDll::init()->getReciveStatus())
   {
    QMap <int ,QVariant> varMapSystem=  ControlCanDll::init()->SystemCommandBack(4);
    ui->label_enabel->setText(QString::number(varMapSystem.value(2).toUInt()));
    show();
   }
}
