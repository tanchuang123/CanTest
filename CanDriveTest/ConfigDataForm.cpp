#include "ConfigDataForm.h"
#include "ui_ConfigDataForm.h"
ConfigDataForm * ConfigDataForm::pConfigDataForm = Q_NULLPTR;
ConfigDataForm::ConfigDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigDataForm)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    QStringList list;
    list<<QStringLiteral("属性")<<QStringLiteral("值")<<QStringLiteral("状态");
    model->setHorizontalHeaderLabels(list);//列表头部
    model->horizontalHeaderItem(0);

    model->setColumnCount(list.size());//列数
    ui->treeView->setModel(model);

    ui->treeView->setColumnWidth(0,250);//列宽
    ui->treeView->setAlternatingRowColors(true);
    connect(ui->config_comboBox, SIGNAL(currentIndexChanged(int)),this,SLOT(slotConfigStatus(int)));
//    ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
//    //--

    /*谭创*/

    //TC

    QHeaderView *hv = ui->treeView->header();
    hv->setSortIndicatorShown(true); //显示排序三角
    hv->setSectionsClickable(true);     //设置标题可以被单击
    SystemStatusForm::init();
    HallTimerA = new QTimer(this);
    HallTimerB = new QTimer(this);
    connect(HallTimerA , SIGNAL(timeout()), this, SLOT(slotHallDataA()));
    connect(HallTimerB , SIGNAL(timeout()), this, SLOT(slotHallDataB()));
    ConfigDataWriteForm::init();
}
ConfigDataForm *ConfigDataForm::init()
{
    if(!pConfigDataForm){
        pConfigDataForm = new ConfigDataForm;
    }
    return pConfigDataForm;
}
ConfigDataForm::~ConfigDataForm()
{
    delete ui;
}
InfoMapDataConfig* ConfigDataForm ::addLevel(const QString &displayText0,
                                          const QString &displayText1,
                                          const QString &displayText2,
                                          const QString &toolTip,
                                          InfoMapDataConfig *parent,
                                          const QString &key,
                                          const QVariant &var
                                          )
{
    InfoMapDataConfig*data = new InfoMapDataConfig;
    data->key = key;
    data->displayText0 = displayText0;
    data->value = var;
    data->displayText1 = displayText1;
    data->displayText2 = displayText2;
    if(toolTip.isEmpty())
    {
//                data->toolTip = "data->toolTip";
    }else{
        data->toolTip = toolTip;
    }
    if(parent){
        data->level = 1;
        parent->child.append(data);
    }else{
        data->level = 0;
    }
    return data;
}
void ConfigDataForm::initTreeView(QMap<int , QVariant> varMap,QMap<int , QVariant> varMapStatus)
{
    QList<InfoMapDataConfig*>InfoMapDataList;
    //1 机器人运行信息
    InfoMapDataList.append(addLevel(QStringLiteral("参数配置")));
    addLevel(QStringLiteral("A 路的控制及查询请求 ID  "), QString::number(varMap.value(0).toInt(),16),varMapStatus.value(0).toString(), /*QString("%1").arg(varMap.value(0).toUInt()),*/"",InfoMapDataList.last());
    addLevel(QStringLiteral("A 路的控制及查询请求 ID "),QString("%1").arg(varMap.value(1).toUInt()),varMapStatus.value(1).toString(), QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataList.last());
    addLevel(QStringLiteral("B 路的控制及查询请求 ID "),QString("%1").arg(varMap.value(2).toUInt()),varMapStatus.value(2).toString(), QStringLiteral("机器人累计运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("B 路的控制及查询回复 ID "),QString("%1").arg(varMap.value(3).toUInt()),varMapStatus.value(3).toString(), QStringLiteral("机器人本次运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("配置信息的写入及查询请求 ID "),QString("%1").arg(varMap.value(4).toUInt()),varMapStatus.value(4).toString(),"", InfoMapDataList.last());
    addLevel(QStringLiteral("配置信息的写入及查询回复 ID "),QString("%1").arg(varMap.value(5).toUInt()),varMapStatus.value(5).toString(),"", InfoMapDataList.last());
    addLevel(QStringLiteral("CAN 比特率 "),QString("%1").arg(varMap.value(6).toInt()),varMapStatus.value(6).toString(),"", InfoMapDataList.last());
    addLevel(QStringLiteral("速度控制模式（0：ST，1：PID） "),QString("%1").arg(varMap.value(7).toUInt()),varMapStatus.value(7).toString(),"", InfoMapDataList.last());
    addLevel(QStringLiteral("电机极对数 "),QString("%1").arg(varMap.value(8).toUInt()),varMapStatus.value(8).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("转子电阻"),QString("%1").arg(varMap.value(9).toFloat()),varMapStatus.value(9).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("定子电阻  "),QString("%1").arg(varMap.value(10).toFloat()),varMapStatus.value(10).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("定子 d轴电感  "),QString("%1").arg(varMap.value(11).toFloat()),varMapStatus.value(11).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("定子 q轴电感  "),QString("%1").arg(varMap.value(12).toFloat()),varMapStatus.value(12).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("电机额定磁通 "),QString("%1").arg(varMap.value(13).toFloat()),varMapStatus.value(13).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("电机最大电流 "),QString("%1").arg(varMap.value(14).toFloat()),varMapStatus.value(14).toString(),tr(""), InfoMapDataList.last());
///2
    addLevel(QStringLiteral("电机码盘线数  "),QString("%1").arg(varMap.value(15).toFloat()),varMapStatus.value(15).toString(),"", InfoMapDataList.last());
    addLevel(QStringLiteral("电机最大转速  "),QString("%1").arg(varMap.value(16).toFloat()),varMapStatus.value(16).toString(), QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataList.last());
    addLevel(QStringLiteral("系统旋转标定时的 d 轴直流标幺量  "),QString("%1").arg(varMap.value(17).toFloat()),varMapStatus.value(17).toString(), QStringLiteral("机器人累计运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("系统旋转标定时的超时时间  "),QString("%1").arg(varMap.value(18).toUInt()),varMapStatus.value(18).toString(), QStringLiteral("机器人本次运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("系统旋转标定前，直流拉取的时间  "),QString("%1").arg(varMap.value(19).toUInt()),varMapStatus.value(19).toString(), tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("联动标志（1：true，0：false）  "),QString("%1").arg(varMap.value(20).toUInt()),varMapStatus.value(20).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("运动指令超时（1：enable，0：disable）  "),QString("%1").arg(varMap.value(21).toUInt()),varMapStatus.value(21).toString(), tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("驱动器电流环等级：( 0：没有 PWM 输出 1：定矢量开环测试状态 2：定矢量闭环测试状态 3：常规应用)"),QString("%1").arg(varMap.value(22).toUInt()),varMapStatus.value(22).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("最大加速度  "),QString("%1").arg(varMap.value(23).toUInt()),varMapStatus.value(23).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("直轴电流Kp 乘因子 "),QString("%1").arg(varMap.value(24).toFloat()),varMapStatus.value(24).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("直轴电流Ki 乘因子   "),QString("%1").arg(varMap.value(25).toFloat()),varMapStatus.value(25).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("直轴电流Kd   "),QString("%1").arg(varMap.value(26).toFloat()),varMapStatus.value(26).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("直轴电流输出量正阈值  "),QString("%1").arg(varMap.value(27).toFloat()),varMapStatus.value(27).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("直轴电流输出量负阈值 "),QString("%1").arg(varMap.value(28).toFloat()),varMapStatus.value(28).toString(),tr(""), InfoMapDataList.last());
    addLevel(QStringLiteral("交轴电流Kp 乘因子  "),QString("%1").arg(varMap.value(29).toFloat()),varMapStatus.value(29).toString(),tr(""), InfoMapDataList.last());

     addLevel(QStringLiteral("交轴电流Ki 乘因子  "),QString("%1").arg(varMap.value(30).toFloat()),varMapStatus.value(30).toString(),"", InfoMapDataList.last());
     addLevel(QStringLiteral("交轴电流Kd  "),QString("%1").arg(varMap.value(31).toFloat()),varMapStatus.value(31).toString(), QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataList.last());
     addLevel(QStringLiteral("交轴电流输出量正阈值   "),QString("%1").arg(varMap.value(32).toFloat()),varMapStatus.value(32).toString(), QStringLiteral("机器人累计运行时间"),InfoMapDataList.last());
     addLevel(QStringLiteral("交轴电流输出量负阈值   "),QString("%1").arg(varMap.value(33).toFloat()),varMapStatus.value(33).toString(), QStringLiteral("机器人本次运行时间"),InfoMapDataList.last());
     addLevel(QStringLiteral("速度环 Kp 乘因子   "),QString("%1").arg(varMap.value(34).toFloat()),varMapStatus.value(34).toString(),tr(""), InfoMapDataList.last());
     addLevel(QStringLiteral("速度环 Ki 乘因子   "),QString("%1").arg(varMap.value(35).toFloat()),varMapStatus.value(35).toString(),tr(""), InfoMapDataList.last());
     addLevel(QStringLiteral("速度环 Kd  "),QString("%1").arg(varMap.value(36).toFloat()),varMapStatus.value(36).toString(),tr(""), InfoMapDataList.last());
     addLevel(QStringLiteral("速度环输出量正阈值   "),QString("%1").arg(varMap.value(37).toFloat()),varMapStatus.value(37).toString(), tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("速度环输出量负阈值   "),QString("%1").arg(varMap.value(38).toFloat()),varMapStatus.value(38).toString(), tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("系统带宽  "),QString("%1").arg(varMap.value(39).toFloat()),varMapStatus.value(39).toString(),tr(""), InfoMapDataList.last());
     addLevel(QStringLiteral("转动惯量因子    "),QString("%1").arg(varMap.value(40).toFloat()),varMapStatus.value(40).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("摩擦系数因子    "),QString("%1").arg(varMap.value(41).toFloat()),varMapStatus.value(41).toString(),tr(""), InfoMapDataList.last());
     addLevel(QStringLiteral("启用 A 路电机霍尔参考值（1：启用，0：不启用）  "),QString("%1").arg(varMap.value(42).toUInt()),varMapStatus.value(42).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("A 路电机 hall_51 参考值  "),QString("%1").arg(varMap.value(43).toFloat()),varMapStatus.value(43).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("A 路电机 hall_13 参考值   "),QString("%1").arg(varMap.value(44).toFloat()),varMapStatus.value(44).toString(),tr(""),InfoMapDataList.last());

     addLevel(QStringLiteral("A 路电机 hall_32 参考值   "),QString("%1").arg(varMap.value(45).toFloat()),varMapStatus.value(45).toString(),"",InfoMapDataList.last());
     addLevel(QStringLiteral("A 路电机 hall_26 参考值   "),QString("%1").arg(varMap.value(46).toFloat()),varMapStatus.value(46).toString(),QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataList.last());
     addLevel(QStringLiteral("A 路电机 hall_64 参考值    "),QString("%1").arg(varMap.value(47).toFloat()),varMapStatus.value(47).toString(),QStringLiteral("机器人累计运行时间"),InfoMapDataList.last());
     addLevel(QStringLiteral("A 路电机 hall_45 参考值    "),QString("%1").arg(varMap.value(48).toFloat()),varMapStatus.value(48).toString(),QStringLiteral("机器人本次运行时间"),InfoMapDataList.last());
     addLevel(QStringLiteral("启用 B 路电机霍尔参考值（1：启用，0：不启用）    "),QString("%1").arg(varMap.value(49).toUInt()),varMapStatus.value(49).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_51 参考值   "),QString("%1").arg(varMap.value(50).toFloat()),varMapStatus.value(50).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_13 参考值   "),QString("%1").arg(varMap.value(51).toFloat()),varMapStatus.value(51).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_32 参考值   "),QString("%1").arg(varMap.value(52).toFloat()),varMapStatus.value(52).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_26 参考值    "),QString("%1").arg(varMap.value(53).toFloat()),varMapStatus.value(53).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_64 参考值  "),QString("%1").arg(varMap.value(54).toFloat()),varMapStatus.value(54).toString(),tr(""),InfoMapDataList.last());
     addLevel(QStringLiteral("B 路电机 hall_45 参考值     "),QString("%1").arg(varMap.value(55).toFloat()),varMapStatus.value(55).toString(),tr(""),InfoMapDataList.last());



    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView->model());

    if(model->rowCount()==0){
        for(int i=0;i<InfoMapDataList.size();++i)
        {
            auto var = InfoMapDataList.at(i);
            QStandardItem *item = new QStandardItem(var->displayText0);
            for(int j=0;j<var->child.size();++j)
            {
                auto childVar = var->child.at(j);
                QStandardItem *childItem0 = new QStandardItem(childVar->displayText0);
                childItem0->setToolTip(childVar->displayText0);
                QStandardItem *childItem1 = new QStandardItem(childVar->displayText1);
                childItem1->setToolTip(childVar->displayText1);
                QStandardItem *childItem2 = new QStandardItem(childVar->displayText2);
                childItem1->setToolTip(childVar->displayText2);//扩展
                QList<QStandardItem*>childItemList;
                childItemList<<childItem0<<childItem1<<childItem2;
                item->appendRow(childItemList);
            }
            model->appendRow(item);
            QModelIndex index = ui->treeView->model()->index(i,0);
            ui->treeView->setExpanded(index,_isExpanded);
        }
    }else{
        for(int i=0;i<InfoMapDataList.size();++i){
            QModelIndex index = model->index(i,0);
            auto var = InfoMapDataList.at(i);
            for(int j=0;j<var->child.size();++j){
                auto childVar = var->child.at(j);
                QModelIndex childIndex = index.child(j,1);
                QStandardItem *childItem1 = model->itemFromIndex(childIndex);
                if(!childItem1){
//                    SCWarning<<"error: QStandardItem is empty."<<i<<j<<childIndex;
                    continue;
                }
                childItem1->setText(childVar->displayText1);
                childItem1->setToolTip(childVar->displayText1);
            }
        }
    }
   show();
}
void ConfigDataForm::closeEvent(QCloseEvent * event)
{

    emit signalCloseStatus(true,2);
   qDebug()<<QStringLiteral("我是关闭Config");
}
void ConfigDataForm::slotConfigStatus(int index)
{
    switch (index) {
    case ConfigStatus::System_Command :
    {
        SystemStatusForm::init()->GetSystemStatus();
    }
        break;
    case ConfigStatus::Write_Command    :
    {
         HallTimerA->stop();
         HallTimerB->stop();
         ConfigDataWriteForm::init()->initTableWidget();
         ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
    }
        break;
    case ConfigStatus::Read_Command     :
    {
        if(ControlCanDll::init()->getReciveStatus())
        {
            QMap<int,QVariant> dataMap= ControlCanDll::init()->read_configdata_back(4);
            QMap<int,QVariant> dataMapStatus= ControlCanDll::init()->getStatusMap();
            ConfigDataForm::init()->initTreeView(dataMap,dataMapStatus);
        }
         ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
        break;
     }
    case ConfigStatus::A_Hall_Command     :
    {
         ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
         if(ControlCanDll::init()->openHallDetectionA(4))
         {

              HallTimerA->start(3000);
         }

    }
        break;
    case ConfigStatus::B_Hall_Command     :
    {
         ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
        if(ControlCanDll::init()->openHallDetectionB(4))
        {

             HallTimerB->start(3000);
        }

    }
        break;
    case ConfigStatus::A_B_Hall_Command:
    {
        ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
       ControlCanDll::init()->openHallDetectionA(4);
       ControlCanDll::init()->openHallDetectionB(4);
       HallTimerA->start(3000);
       HallTimerB->start(3000);
    }
        break;
    default:
        break;
    }
}

void ConfigDataForm::slotHallDataA()
{
      QMap<int,QVariant> varMap = ControlCanDll::init()->HallDetectionA(4);
      for(int i=0;i<varMap.size();i++)
      {

          ui->widget_A->setText(QStringLiteral("A路电机侦测中.....请等待!"));
          if(varMap.value(0x02).toUInt()==1)
          {
              HallTimerA->stop();
              ChMessageOkCancel(QStringLiteral( "A回路霍尔侦测完成，请重新读取参数"));
              ui->widget_A->setText(" ");
              break;
          }
          qDebug()<<varMap.size()<< varMap.value(0x02).toUInt()<<"0X02";
      }
}
 void ConfigDataForm::slotHallDataB()
 {
            QMap<int,QVariant> varMap = ControlCanDll::init()->HallDetectionB(4);
             qDebug()<<varMap.size()<< varMap.value(0x04).toUInt()<<"0X04";
            for(int i=0;i<varMap.size();i++)
            {
                ui->widget_B->setText(QStringLiteral("B路电机侦测中.....请等待!"));
                if(varMap.value(0x04).toUInt()==1)
                {
                    HallTimerB->stop();
                    ChMessageOkCancel(QStringLiteral( "B回路霍尔侦测完成，请重新读取参数"));
                    ui->widget_B->setText(" ");
                    break;
                }
            }

}
