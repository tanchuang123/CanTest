#include "ConfigDataWriteForm.h"
#include "ui_ConfigDataWriteForm.h"
ConfigDataWriteForm * ConfigDataWriteForm::pConfigDataWriteForm = Q_NULLPTR;
ConfigDataWriteForm::ConfigDataWriteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigDataWriteForm)
{
    ui->setupUi(this);
}
ConfigDataWriteForm * ConfigDataWriteForm:: init()
{
    if(!pConfigDataWriteForm)
    {
        pConfigDataWriteForm = new ConfigDataWriteForm;
    }
    return pConfigDataWriteForm;
}
ConfigDataWriteForm::~ConfigDataWriteForm()
{
    delete ui;
}
void ConfigDataWriteForm::initTableWidget()
{
    ui->tableWidget_Write->setWindowTitle("QTableWidget & Item");
    ui->tableWidget_Write->resize(350, 200);  //设置表格
    QStringList header;
    header<<"Month"<<"Description";
    ui->tableWidget_Write->setRowCount(3);
    ui->tableWidget_Write->setColumnCount(2);

    ui->tableWidget_Write->setHorizontalHeaderLabels(header);
    ui->tableWidget_Write->setItem(0,0,new QTableWidgetItem("Jan"));
    ui->tableWidget_Write->setItem(1,0,new QTableWidgetItem("Feb"));
    ui->tableWidget_Write->setItem(2,0,new QTableWidgetItem("Mar"));
    ui->tableWidget_Write->setItem(0,1,new QTableWidgetItem(QIcon("images/IED.png"), "Jan's month"));
    ui->tableWidget_Write->setItem(1,1,new QTableWidgetItem(QIcon("images/IED.png"), "Feb's month"));
    ui->tableWidget_Write->setItem(2,1,new QTableWidgetItem(QIcon("images/IED.png"), "Mar's month"));
    ui->tableWidget_Write->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_Write->show();
}
