#ifndef CONFIGDATAFORM_H
#define CONFIGDATAFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMap>
#include <QCloseEvent>
#include <QtDebug>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include "SystemStatusForm.h"
#include "ConfigDataWriteForm.h"
#include "Setting/setting.h"
struct InfoMapDataConfig{
    int level;                                //0为第一层1为第二层依次类推
    QString key;                          //关键字
    QString displayText0;           //字段名
    QString displayText1;           //显示数据
    QString displayText2;           //状态数据
    QString toolTip;                    //提示描述
    QVariant value;                     //对应数据
    QString unit;                         //单位
    QList<InfoMapDataConfig*>child;  //子类数据
};
namespace Ui {
class ConfigDataForm;
}

class ConfigDataForm : public QWidget
{
    Q_OBJECT

public:
    enum ConfigStatus{
        System_Command  =0,      //系统命令
        Write_Command     =1 ,     //写入参数
        Read_Command      =2,      //读取参数
        A_Hall_Command    =3,      //A霍尔侦测
        B_Hall_Command    =4,      //B霍尔侦测
        A_B_Hall_Command  =5,    //同时侦测A/B
                         //错误
    };
    static ConfigDataForm*init();
    explicit ConfigDataForm(QWidget *parent = 0);
    ~ConfigDataForm();

    void initTreeView(QMap<int , QVariant> varMap,QMap<int , QVariant> varMapStatus);
    InfoMapDataConfig *addLevel(const QString &displayText0,
                          const QString &displayText1 = "",
                          const QString &displayText2 = "",
                          const QString &toolTip = "",
                          InfoMapDataConfig *parent = Q_NULLPTR,
                          const QString &key = "",
                          const QVariant &var = QVariant());
    static int ChMessageOkCancel(QString info)
        {
            QMessageBox msg;
            msg.setWindowTitle(QStringLiteral("提示"));
            msg.setText(info);
            msg.setStyleSheet("color:rgb(220, 0, 0);font: 14pt");
            msg.setIcon(QMessageBox::Information);
            msg.addButton(QStringLiteral("确定"),QMessageBox::ActionRole);
//            msg.addButton(tr("取消"),QMessageBox::ActionRole);
            return msg.exec();
        }
private slots:
    void closeEvent( QCloseEvent * event);
    void slotConfigStatus(int);
    void slotHallDataA();
    void slotHallDataB();
    void on_pushButton_OUT_clicked();

    void on_pushButton_IN_clicked();
    void slotTypeData(bool);
signals:
    void signalCloseStatus(bool,int); 
private:
    static  ConfigDataForm *pConfigDataForm;
    Ui::ConfigDataForm *ui;
    bool _isExpanded = true;
    bool _isPressedSection = true;
    QTimer *HallTimerA;
    QTimer *HallTimerB;
    bool _type;
};

#endif // CONFIGDATAFORM_H
