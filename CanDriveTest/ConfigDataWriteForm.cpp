#include "ConfigDataWriteForm.h"
#include "ui_ConfigDataWriteForm.h"
ConfigDataWriteForm * ConfigDataWriteForm::pConfigDataWriteForm = Q_NULLPTR;
ConfigDataWriteForm::ConfigDataWriteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigDataWriteForm)
{
    ui->setupUi(this);
    WriteDataDialog::init();
    connect( ui->tableWidget_Write, SIGNAL(cellDoubleClicked (int, int)), this, SLOT( cellSelected( int, int )));
    connect(WriteDataDialog::init(), SIGNAL(signalValue(int,QString)),this ,SLOT(slotTableWidgetItem(int,QString)));
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
      QMap<int,QVariant> valueMap1= Setting::init()->ReadIniAttribute();
      QMap<int,QVariant> valueMap2= Setting::init()->ReadIniValue();
      QHeaderView* headerView =  ui->tableWidget_Write->verticalHeader();
      headerView->setHidden(true);

      ui->tableWidget_Write->setAlternatingRowColors (true);
      ui->tableWidget_Write->setAlternatingRowColors(true);
      ui->tableWidget_Write->setRowCount(valueMap1.size()-1);
      ui->tableWidget_Write->setColumnCount(3);
//      ui->tableWidget_Write->currentIndex()
      QStringList headerString;
      headerString<<QStringLiteral("属性状态")<<QStringLiteral( "参数地址")<<QStringLiteral("属性值");
      ui->tableWidget_Write->setHorizontalHeaderLabels(headerString);

      ui->tableWidget_Write->horizontalHeader()->setStretchLastSection(true);//关键
      ui->tableWidget_Write->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
      ui->tableWidget_Write->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        ui->tableWidget_Write->setShowGrid(false);
      ui->tableWidget_Write->setSelectionBehavior(QAbstractItemView::SelectRows);
      ui->tableWidget_Write->setSelectionMode(QAbstractItemView::SingleSelection);
      for(int i=0;i<valueMap1.size();i++)
      {
           ui->tableWidget_Write->setItem(i,0,new QTableWidgetItem(valueMap1.value(i).toString()));
           ui->tableWidget_Write->setItem(i,1,new QTableWidgetItem(valueMap2.value(i).toString()));
      }
    show();
}
void ConfigDataWriteForm::cellSelected( int nRow, int nCol)
{
     QString text= ui->tableWidget_Write->item(nRow,0)->text();//item(行，列)
     QString str= ui->tableWidget_Write->item(nRow,1)->text();//item(行，列)
     WriteDataDialog::init()->GetStatusValue(text,nRow);
}
void ConfigDataWriteForm::slotTableWidgetItem(int index,QString text)
{
      ui->tableWidget_Write->setItem(index,2,new QTableWidgetItem(text));
}
