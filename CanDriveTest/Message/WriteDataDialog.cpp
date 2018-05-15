#include "WriteDataDialog.h"
#include "ui_WriteDataDialog.h"
#include <QtDebug>
WriteDataDialog *WriteDataDialog::pWriteDataDialog = Q_NULLPTR;
WriteDataDialog::WriteDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteDataDialog)
{
    ui->setupUi(this);

}

WriteDataDialog *WriteDataDialog::init()
{
    if(!pWriteDataDialog)
    {
        pWriteDataDialog =new WriteDataDialog;
    }
    return pWriteDataDialog;
}
WriteDataDialog::~WriteDataDialog()
{
    delete ui;
}

void WriteDataDialog::on_pushButton_WriteOK_clicked()
{
    if(!ui->lineEdit_ConfigData->text().isEmpty())
    {
        QString valueText=ui->lineEdit_ConfigData->text();
        float value = valueText.toFloat();
//        qDebug()<<value <<"ccccccccccccccccccccccccccccccccvalueText.toFloat()";
        QString indexStr= ui->label_value->text();
        int indexValue = indexStr.toInt();
       int valueBack= ControlCanDll::init()->WriteData(4,value,indexValue);
       if( valueBack==0)
       {
           emit signalValue(indexValue, valueText);
       }
       if(valueBack==1)
       {
           emit signalValue(indexValue,QStringLiteral("写入的参数超过最大阈值 "));
       }
       if(valueBack==2)
       {
           emit signalValue(indexValue, QStringLiteral("写入的参数低于最小阈值 "));
       }
       close();
    }

}
void WriteDataDialog::GetStatusValue(const QString &text , int value )
{
     setWindowTitle(QStringLiteral("写入窗"));
     ui->label_attribute->setText(text);
     ui->label_value ->setText(tr("%1").arg(value));
     show();
}
