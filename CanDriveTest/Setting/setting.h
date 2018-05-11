#ifndef SETTING_H
#define SETTING_H
#include <QSettings>
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
class Setting : public QSettings
{
public:
    static Setting*init();
    Setting();
    void setIni(QList<double> data,QString IniName);
    QMap<int,QVariant>  ReadIniAttribute();
    QMap<int,QVariant>  ReadIniValue();
    void writeIni();
private:
    static  Setting *pSetting;
    QSettings *Ini;
    QSettings *pIni;
    QString  ininame;
};

#endif // SETTING_H
