#ifndef SETTING_H
#define SETTING_H
#include <QSettings>
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QMap>
#include <QVariant>
class Setting : public QSettings
{
public:
    static Setting*init();
    Setting();
    void setIni(QMap<int,QVariant> data,QString IniName);
    QMap<int,QVariant>  ReadIniAttribute();
    QMap<int,QVariant>  ReadIniValue();
    QMap<int,QVariant>  ReadIniPar(QString fileName);
    QString  ReadIniDataType(int index);
    void writeIni();
private:
    static  Setting *pSetting;
    QSettings *Ini;
    QSettings *pIni;
    QString  ininame;
};

#endif // SETTING_H
