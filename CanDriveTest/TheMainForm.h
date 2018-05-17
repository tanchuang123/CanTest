#ifndef THEMAINFORM_H
#define THEMAINFORM_H

#include <QMainWindow>
#include "lib/ControlCAN.h"
#include "ControlCanDll.h"
#include "Message/messagedialog.h"
#include "LoopADataForm.h"
#include "LoopBDataForm.h"
#include "ConfigDataForm.h"
#include <QtDebug>
#include <QMap>
#include <QTimer>
#include "Setting/setting.h"
namespace Ui {
class TheMainForm;
}

class TheMainForm : public QMainWindow
{
    Q_OBJECT

public:
    enum Can_ID{
        ID_Error=-1,
        ID_A =0 ,     //一直等待
        ID_B = 1,             //普通
        ID_Config=2,            //警告
                         //错误
    };
    explicit TheMainForm(QWidget *parent = 0);
    ~TheMainForm();
    void initDriverAB();
private slots:
    void on_pushButton_Start_can_clicked();

    void on_pushButton_start_udp_clicked();

    void on_pushButton_close_clicked();
    void slotCanIdLoop(int);
    void slotALoopData();
    void slotBLoopData();
    void slotConfigData();
    void slotClose(bool,int);

    void on_pushButton_send_vate_clicked();
    void slotChangeElecID(int index);
    void slotOneDriverState(int);
    void slotDoubleDriverState(int);
    void on_pushButton_stopNow_clicked();

private:
    Ui::TheMainForm *ui;
    messageDialog *Message_Dialog = Q_NULLPTR;
    LoopADataForm *AloopData;
    LoopBDataForm * BloopData;
    QTimer *ALoopTimer;
    QTimer *BLoopTimer;
    QTimer *ConfigTimer;
    UINT _ID;

};

#endif // THEMAINFORM_H
