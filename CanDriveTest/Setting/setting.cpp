#include "Setting/setting.h"
#include <QTextCodec>
#include <QtDebug>
Setting * Setting::pSetting = Q_NULLPTR;
Setting::Setting()
{

}
Setting *Setting::init()
{
    if(!pSetting){
        pSetting = new Setting;
    }
    return pSetting;
}
void Setting::setIni(QMap<int,QVariant> data,QString IniName)
{
    if(IniName.trimmed() =="")
    {
        ininame = "parameter";
    }
    else {
         ininame=IniName;
    }

    Ini=new QSettings(QCoreApplication::applicationDirPath()+tr("/%1").arg(ininame)+".ini", QSettings::IniFormat);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");

    for(int i=0;i<data.size()+1;i++)
    {

        Ini->setValue("/setting/time"+tr("%1").arg(i), tr("%1").arg(current_date));
        Ini->setValue("/setting/dataValue"+tr("%1").arg(i), tr("%1").arg(data.value(i).toFloat()));

    }

     delete Ini;
}
void Setting::writeIni()
{


}
//void Setting::ReadIni()
//{
//        pIni= new QSettings(QCoreApplication::applicationDirPath()+tr("/%1").arg(ininame)+".ini", QSettings::IniFormat);
//        QString sArg = "";
//        for(int i=0;i<=31;i++)
//        {
//        sArg = pIni->value("/setting/time"+tr("%1").arg(i)).toString();
////        qDebug("arg1 = %s\n", sArg.toStdString().data());

//        sArg = "";
//        sArg = pIni->value("/setting/data"+tr("%1").arg(i)).toString();
////        qDebug("arg2 = %s\n", sArg.toStdString().data());
//        }

//        delete pIni;

//}
QMap<int,QVariant>  Setting::ReadIniAttribute()
{
    QMap<int,QVariant> valueMap;
    pIni=new QSettings("config.ini",QSettings::IniFormat);
    pIni->setIniCodec(QTextCodec::codecForName("GBK"));

    QString sArg1 = "";
    QString sArg2 = "";
    for(int i=0;i<56;i++)
    {
     sArg2 = pIni->value("/setting/type"+tr("%1").arg(i)).toString();
        valueMap.insert(i,sArg2);
//        qDebug()<<sArg2;
    }
    return valueMap;
}
QMap<int,QVariant>  Setting::ReadIniValue()
{
    QMap<int,QVariant> valueMap;
    pIni=new QSettings("config.ini",QSettings::IniFormat);
    pIni->setIniCodec(QTextCodec::codecForName("GBK"));

    QString sArg1 = "";
    QString sArg2 = "";
    for(int i=0;i<56;i++)
    {
    sArg1 = pIni->value("/setting/value"+tr("%1").arg(i)).toString();
    valueMap.insert(i,sArg1);
    }
    return valueMap;
}
QMap<int,QVariant>  Setting::ReadIniPar(QString fileName)
{
    QMap<int,QVariant> valueMap;
    pIni=new QSettings(fileName,QSettings::IniFormat);
    pIni->setIniCodec(QTextCodec::codecForName("GBK"));

    QString sArg1 = "";
    for(int i=0;i<56;i++)
    {
      sArg1 = pIni->value("/setting/dataValue"+tr("%1").arg(i)).toString();
      valueMap.insert(i,sArg1);
    }
    return valueMap;
}
QString  Setting::ReadIniDataType(int index)
{
    pIni=new QSettings("config.ini",QSettings::IniFormat);
    pIni->setIniCodec(QTextCodec::codecForName("GBK"));

    QString sArg = "";
    sArg = pIni->value("/setting/dataType"+tr("%1").arg(index)).toString();
    return sArg;
}
