#include "LoopBDataForm.h"
#include "ui_LoopBDataForm.h"

LoopBDataForm * LoopBDataForm::pLoopBDataForm=Q_NULLPTR;
LoopBDataForm::LoopBDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoopBDataForm)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    QStringList list;
    list<<QStringLiteral("属性")<<QStringLiteral("值");
    model->setHorizontalHeaderLabels(list);//列表头部
    model->horizontalHeaderItem(0);

    model->setColumnCount(list.size());//列数
    ui->treeViewList->setModel(model);

    ui->treeViewList->setColumnWidth(0,250);//列宽
    ui->treeViewList->setAlternatingRowColors(true);
    ui->treeViewList->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
//    //--

    /*谭创*/

    //TC

    QHeaderView *hv = ui->treeViewList->header();
    hv->setSortIndicatorShown(true); //显示排序三角
    hv->setSectionsClickable(true);     //设置标题可以被单击
//    connect(hv,SIGNAL(sectionClicked(int)),this,SLOT(slotSectionClicked(int)));

}
LoopBDataForm *LoopBDataForm::init()
{
    if(!pLoopBDataForm)
    {
        pLoopBDataForm=new LoopBDataForm;
    }
    return pLoopBDataForm;
}
LoopBDataForm::~LoopBDataForm()
{
    delete ui;
}
InfoMapDataB* LoopBDataForm ::addLevel(const QString &displayText0,
                                          const QString &displayText1,
                                          const QString &toolTip,
                                          InfoMapDataB *parent,
                                          const QString &key,
                                          const QVariant &var
                                          )
{
    InfoMapDataB*data = new InfoMapDataB;
    data->key = key;
    data->displayText0 = displayText0;
    data->value = var;
    data->displayText1 = displayText1;
    if(toolTip.isEmpty())
    {
        //        data->toolTip = data->text;
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
void LoopBDataForm::initTreeView(QMap<int , QVariant> varMap)
{
    QList<InfoMapDataB*>InfoMapDataBList;
    //1 机器人运行信息
    InfoMapDataBList.append(addLevel(QStringLiteral("普通数据信息")));
    addLevel(QStringLiteral("获取命令值 "),QString("%1").arg(varMap.value(0x00).toDouble()),"",InfoMapDataBList.last());
    addLevel(QStringLiteral("电机位置(Count)"),QString("%1").arg(varMap.value(0x01).toInt()),QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataBList.last());
    addLevel(QStringLiteral("电机速度 (Rpm)"),QString("%1").arg(varMap.value(0x02).toFloat()),QStringLiteral("机器人累计运行时间"),InfoMapDataBList.last());
    addLevel(QStringLiteral("电机转矩 (Nm)"),QString("%1").arg(varMap.value(0x03).toFloat()),QStringLiteral("机器人本次运行时间"),InfoMapDataBList.last());
    addLevel(QStringLiteral("无霍尔重定位状态 "),QString("%1").arg(varMap.value(0x04).toInt()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("电机错误状态"),QString("%1").arg(varMap.value(0x10).toInt()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("霍尔值"),QString("%1").arg(varMap.value(0x20).toInt()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("A 相电流偏置"),QString("%1").arg(varMap.value(0x21).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("B 相电流偏置 "),QString("%1").arg(varMap.value(0x22).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("C 相电流偏置 "),QString("%1").arg(varMap.value(0x23).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("A 相实时电流标幺值 "),QString("%1").arg(varMap.value(0x24).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("B 相实时电流标幺值 "),QString("%1").arg(varMap.value(0x25).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("C 相实时电流标幺值 "),QString("%1").arg(varMap.value(0x26).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("三相电流和标幺值  "),QString("%1").arg(varMap.value(0x27).toFloat()),tr(""),InfoMapDataBList.last());
    addLevel(QStringLiteral("母线电压 (V) "),QString("%1").arg(varMap.value(0x28).toFloat()),tr(""),InfoMapDataBList.last());
///2
    InfoMapDataBList.append(addLevel(QStringLiteral("电机错误状态")));
    unsigned int mask=0x8000;
    int errorValue=varMap.value(0x10).toUInt();
     QMap<int,QVariant> errorMap;
        for(int i=0;i<32;i++)
        {
            errorMap.insert(i,(errorValue<<i)&mask);
        }

            addLevel(QStringLiteral("GVDD 过压报警 "),errorMap.value(0).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("C 相下桥臂过流 "),errorMap.value(1).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("B 相下桥臂过流 "),errorMap.value(2).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("A 相下桥臂过流 "),errorMap.value(3).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("C 相上桥臂过流 "),errorMap.value(4).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("B 相上桥臂过流 "),errorMap.value(5).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("A 相上桥臂过流 "),errorMap.value(6).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("温度过高报警 "),errorMap.value(7).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("PVDD 欠压报警  "),errorMap.value(8).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("GVDD 欠压报警  "),errorMap.value(9).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("驱动器硬件错误  "),errorMap.value(10).toBool()>0 ? QStringLiteral("报警"):QStringLiteral("正常") ,"",InfoMapDataBList.last());
            addLevel(QStringLiteral("驱动器被拍下急停  "),errorMap.value(11).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("固件参数尚未烧录  "),errorMap.value(12).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("无霍尔初始化定位失败  "),errorMap.value(13).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
             addLevel(QStringLiteral("霍尔错误报警   "),errorMap.value(14).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("霍尔错误报警 "),errorMap.value(15).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("霍尔校正值错误报警  "),errorMap.value(16).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("三相节点电流和失衡报警   "),errorMap.value(17).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("连锁故障锁定  "),errorMap.value(18).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("驱动器直流母线失压报警   "),errorMap.value(19).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("转速超过最大转速的 10%   "),errorMap.value(20).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());
            addLevel(QStringLiteral("当前编码器相位和霍尔校正相位不匹配  "),errorMap.value(21).toBool()>0? QStringLiteral("报警"):QStringLiteral("正常"),"",InfoMapDataBList.last());


    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeViewList->model());

    if(model->rowCount()==0){
        for(int i=0;i<InfoMapDataBList.size();++i)
        {
            auto var = InfoMapDataBList.at(i);
            QStandardItem *item = new QStandardItem(var->displayText0);
            for(int j=0;j<var->child.size();++j)
            {
                auto childVar = var->child.at(j);
                QStandardItem *childItem0 = new QStandardItem(childVar->displayText0);
                childItem0->setToolTip(childVar->displayText0);
                QStandardItem *childItem1 = new QStandardItem(childVar->displayText1);
                childItem1->setToolTip(childVar->displayText1);
                QList<QStandardItem*>childItemList;
                childItemList<<childItem0<<childItem1;
                item->appendRow(childItemList);
            }
            model->appendRow(item);
            QModelIndex index = ui->treeViewList->model()->index(i,0);
            ui->treeViewList->setExpanded(index,_isExpanded);
        }
    }else{
        for(int i=0;i<InfoMapDataBList.size();++i){
            QModelIndex index = model->index(i,0);
            auto var = InfoMapDataBList.at(i);
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
void LoopBDataForm::closeEvent(QCloseEvent * event)
{
    emit signalCloseStatus(true,1);
    qDebug()<<QStringLiteral("我是关闭B");
 }
