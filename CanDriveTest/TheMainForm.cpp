#include "TheMainForm.h"
#include "ui_TheMainForm.h"
#include "Message/UiClass.h"

TheMainForm::TheMainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TheMainForm)
{
    ui->setupUi(this);
    initDriverAB();
    Message_Dialog = new messageDialog(this);
    ControlCanDll::init();
    UiClass::init()->setMainWindow(this);
    LoopADataForm::init();
    LoopBDataForm::init();
    ConfigDataForm::init();
//    connect(ui->comboBox_can_id, SIGNAL(currentIndexChanged(int)),this,SLOT(slotCanIdLoop(int)));
    connect(ui->comboBox_can_id, SIGNAL(activated(int)),this,SLOT(slotCanIdLoop(int)));
    ALoopTimer = new QTimer(this);
    BLoopTimer = new QTimer(this);
    ConfigTimer = new QTimer(this);
    connect(ALoopTimer, SIGNAL(timeout()), this, SLOT(slotALoopData()));
    connect(BLoopTimer, SIGNAL(timeout()), this, SLOT(slotBLoopData()));
    connect(ConfigTimer, SIGNAL(timeout()), this, SLOT(slotConfigData()));
    connect(LoopADataForm::init(), SIGNAL(signalCloseStatus(bool,int)),this,SLOT(slotClose(bool,int)));
    connect(LoopBDataForm::init(),  SIGNAL(signalCloseStatus(bool,int)),this,SLOT(slotClose(bool,int)));
    connect(ConfigDataForm::init(),  SIGNAL(signalCloseStatus(bool,int)),this,SLOT(slotClose(bool,int)));
    ui->comboBox_can_id->setEnabled(false);

    connect(ui->comboBox_A_B,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeElecID(int)));
    connect(ui->checkBox_oneDriver,SIGNAL(stateChanged(int)),this,SLOT(slotOneDriverState(int)));
    connect(ui->checkBox_doubleDriver,SIGNAL(stateChanged(int)),this,SLOT(slotDoubleDriverState(int)));

}

TheMainForm::~TheMainForm()
{
    delete ui;
}
void TheMainForm::initDriverAB()
{
    ui->comboBox_A_B->setEnabled(false);
    ui->lineEdit_vate->setEnabled(false);
    ui->pushButton_send_vate->setEnabled(false);
    ui->pushButton_A->setEnabled(false);
    ui->pushButton_D->setEnabled(false);
    ui->pushButton_S->setEnabled(false);
    ui->pushButton_W->setEnabled(false);
    ui->pushButton_Stop->setEnabled(false);
    ui->pushButton_stopNow->setEnabled(false);
    ui->checkBox_oneDriver->setEnabled(false);
    ui->checkBox_doubleDriver->setEnabled(false);
}
void TheMainForm::on_pushButton_Start_can_clicked()
{
    if( ControlCanDll::init()->StartCAN())
    {
           QString  text_id= "701";
           if( ControlCanDll::init()->Transmit(text_id.toUInt(0,16)))
           {
//               qDebug()<<"701"<<text_id.toUInt(0,16);
             QStringList receivedata =   ControlCanDll::init()->Receive();
             if(receivedata.at(0)=="NULL")
             {
                 UiClass::init()->showToastr(QStringLiteral( "启动失败!"),SCToastr::E_Info,Q_NULLPTR,false);
                 ControlCanDll::init()->CloseDevice();
             }
             else
             {
                 ui->comboBox_can_id->setEnabled(true);
                 ui->label_A_request->setText(receivedata.at(0));
                 ui->label_A_response->setText(receivedata.at(1));
                 ui->label_B_request->setText(receivedata.at(2));
                 ui->label_B_response->setText(receivedata.at(3));
                 ui->label_config_request->setText(receivedata.at(4));
                 ui->label_config_response->setText(receivedata.at(5));
                 UiClass::init()->showToastr(QStringLiteral( "启动成功!"),SCToastr::E_Info,Q_NULLPTR,false);
                 ui->checkBox_oneDriver->setEnabled(true);
                 ui->checkBox_doubleDriver->setEnabled(true);
             }

           }
    }
    else
    {
        UiClass::init()->showToastr(QStringLiteral( "启动失败!"),SCToastr::E_Info,Q_NULLPTR,false);
        ControlCanDll::init()->CloseDevice();
    }
}

void TheMainForm::on_pushButton_start_udp_clicked()
{

}
void TheMainForm::on_pushButton_close_clicked()
{
   if( ControlCanDll::init()->CloseDevice())
   {
        UiClass::init()->showToastr(QStringLiteral( "关闭成功!"),SCToastr::E_Info,Q_NULLPTR,false);        
        ALoopTimer->stop();
        LoopADataForm::init()->close();
        BLoopTimer->stop();
        LoopBDataForm::init()->close();
        ConfigTimer->stop();
        ConfigDataForm::init()->close();
        ui->checkBox_oneDriver->setEnabled(false);
        ui->checkBox_doubleDriver->setEnabled(false);

        if(!ui->checkBox_oneDriver->isEnabled())
        {
             ui->checkBox_oneDriver->setCheckState(Qt::Unchecked);
        }
        if(!ui->checkBox_doubleDriver->isEnabled())
        {
            ui->checkBox_doubleDriver->setCheckState(Qt::Unchecked);
        }
        ui->comboBox_can_id->setEnabled(false);
   }
   else {
        UiClass::init()->showToastr(QStringLiteral( "关闭失败!"),SCToastr::E_Info,Q_NULLPTR,false);
   }
}
void TheMainForm::slotCanIdLoop(int id)
{

    switch (id)
    {
    case Can_ID::ID_A:
        {
        ALoopTimer->start(2000);
        break;
        }
    case Can_ID::ID_B:
        {
        BLoopTimer->start(2000);
        break;
        }
    case Can_ID::ID_Config:
        {
//        ConfigTimer->start(3000);
        if(ControlCanDll::init()->getReciveStatus())
        {
            QMap<int,QVariant> dataMap= ControlCanDll::init()->read_configdata_back(4);
            QMap<int,QVariant> dataMapStatus= ControlCanDll::init()->getStatusMap();
            ConfigDataForm::init()->initTreeView(dataMap,dataMapStatus);
        }
        break;
        }
    default:
        break;
    }
}
void TheMainForm::slotALoopData()
{

    if(ControlCanDll::init()->getReciveStatus())
    {
        QMap<int,QVariant> dataMap= ControlCanDll::init()->Data_Back(0);
        LoopADataForm::init()->initTreeView(dataMap);
    }

    qDebug()<<"timerA";
}
void TheMainForm::slotBLoopData()
{
    if(ControlCanDll::init()->getReciveStatus())
    {
        QMap<int,QVariant> dataMap= ControlCanDll::init()->Data_Back(2);
        LoopBDataForm::init()->initTreeView(dataMap);
    }
    qDebug()<<"timerB";
}
void TheMainForm::slotConfigData()
{
    if(ControlCanDll::init()->getReciveStatus())
    {
        QMap<int,QVariant> dataMap= ControlCanDll::init()->read_configdata_back(4);
        QMap<int,QVariant> dataMapStatus= ControlCanDll::init()->getStatusMap();
        ConfigDataForm::init()->initTreeView(dataMap,dataMapStatus);
    }
    qDebug()<<"timerConfig";
}
void TheMainForm::slotClose(bool status,int type)
{
//     qDebug()<<QStringLiteral( "我是信号")<<status<<"status"<<type<<"type";
    if(status)
    {
        switch (type) {
        case 0:
        {
            ALoopTimer->stop();
            LoopADataForm::init()->close();
        }
            break;
        case 1:
        {
            BLoopTimer->stop();
            LoopBDataForm::init()->close();
        }
            break;
        case 2:
        {
            ConfigTimer->stop();
            ConfigDataForm::init()->close();
        }
            break;
        default:
            break;
        }

    }
}


void TheMainForm::on_pushButton_send_vate_clicked()
{

    QString valueText;
    if(!ui->lineEdit_vate->text().isEmpty())
    {
        ui->comboBox_A_B->setEnabled(false);
        valueText=ui->lineEdit_vate->text();
        int index= ui->comboBox_A_B->currentIndex();
        if(index == 0)
        {
            qDebug()<<valueText<<valueText.toFloat();
            ControlCanDll::init()->WriteDebugData(0,valueText.toFloat());
        }
        if(index == 1)
        {
            ControlCanDll::init()->WriteDebugData(2,valueText.toFloat());
        }
        ui->pushButton_stopNow->setEnabled(true);
    }

}
void TheMainForm::slotChangeElecID(int index)
{
    switch (index)
    {
    case 0:
          {
            if(ui->label_A_request->text()!="NULL")
            {
                QString valueText=ui->label_A_request->text();
                _ID=valueText.toUInt(0,16);
                qDebug()<<"_idA"<<_ID;
            }
          }
        break;
    case 1:
    {
        if(ui->label_B_request->text()!="NULL")
        {
            QString valueText=ui->label_B_request->text();
            _ID=valueText.toUInt(0,16);
            qDebug()<<"_idB"<<_ID;
        }
    }
    default:
        break;
    }
}
void TheMainForm::slotOneDriverState(int index)
{
    if (index == Qt::Checked) // "选中"
        {
            ui->checkBox_doubleDriver->setCheckState(Qt::Unchecked);
            ui->comboBox_A_B->setEnabled(true);
            ui->lineEdit_vate->setEnabled(true);
            ui->pushButton_send_vate->setEnabled(true);
        }
        else if(index == Qt::PartiallyChecked) // "半选"
        {
//            m_pLabel->setText("PartiallyChecked");
        }
        else // 未选中 - Qt::Unchecked
        {

            ui->comboBox_A_B->setEnabled(false);
            ui->lineEdit_vate->setEnabled(false);
            ui->pushButton_send_vate->setEnabled(false);
        }
}
void TheMainForm::slotDoubleDriverState(int index)
{
    if (index == Qt::Checked) // "选中"
        {
        ui->checkBox_oneDriver->setCheckState(Qt::Unchecked);
        ui->pushButton_A->setEnabled(true);
        ui->pushButton_D->setEnabled(true);
        ui->pushButton_S->setEnabled(true);
        ui->pushButton_W->setEnabled(true);
        ui->pushButton_Stop->setEnabled(true);
        }
        else if(index == Qt::PartiallyChecked) // "半选"
        {
//            m_pLabel->setText("PartiallyChecked");
        }
        else // 未选中 - Qt::Unchecked
        {

        ui->pushButton_A->setEnabled(false);
        ui->pushButton_D->setEnabled(false);
        ui->pushButton_S->setEnabled(false);
        ui->pushButton_W->setEnabled(false);
        ui->pushButton_Stop->setEnabled(false);
        }
}

void TheMainForm::on_pushButton_stopNow_clicked()
{
    int index= ui->comboBox_A_B->currentIndex();
    if(index == 0)
    {
        ControlCanDll::init()->WriteDebugData(0,0);
    }
    if(index == 1)
    {
        ControlCanDll::init()->WriteDebugData(2,0);
    }
    ui->comboBox_A_B->setEnabled(true);
    ui->pushButton_stopNow->setEnabled(false);

}
