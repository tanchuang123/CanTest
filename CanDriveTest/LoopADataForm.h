#ifndef LOOPADATAFORM_H
#define LOOPADATAFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMap>
#include <QCloseEvent>
#include <QtDebug>
struct InfoMapData{
    int level;                                //0为第一层1为第二层依次类推
    QString key;                          //关键字
    QString displayText0;           //字段名
    QString displayText1;           //显示数据
    QString toolTip;                    //提示描述
    QVariant value;                     //对应数据
    QString unit;                         //单位
    QList<InfoMapData*>child;  //子类数据
};
namespace Ui {
class LoopADataForm;
}

class LoopADataForm : public QWidget
{
    Q_OBJECT

public:
    static LoopADataForm*init();
    explicit LoopADataForm(QWidget *parent = 0);
    ~LoopADataForm();
    void initTreeView(QMap<int , QVariant> varMap);
    InfoMapData *addLevel(const QString &displayText0,
                          const QString &displayText1 = "",
                          const QString &toolTip = "",
                          InfoMapData *parent = Q_NULLPTR,
                          const QString &key = "",
                          const QVariant &var = QVariant());

private slots:
    void closeEvent( QCloseEvent * event);
signals:
    void signalCloseStatus(bool,int);
private:
    static  LoopADataForm *pLoopADataForm;
    Ui::LoopADataForm *ui;
    bool _isExpanded = true;
    bool _isPressedSection = true;
};

#endif // LOOPADATAFORM_H
