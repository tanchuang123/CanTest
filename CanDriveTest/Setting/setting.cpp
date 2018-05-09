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
void Setting::setIni(QList<double> data,QString IniName)
{
    if(IniName.trimmed() =="")
    {
        return;

    }
    else {
         ininame=IniName;
    }

    Ini=new QSettings(QCoreApplication::applicationDirPath()+tr("/%1").arg(ininame)+".ini", QSettings::IniFormat);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");

    for(int i=0;i<data.size();i++)
    {

        Ini->setValue("/setting/time"+tr("%1").arg(i), tr("%1").arg(current_date));
        Ini->setValue("/setting/data"+tr("%1").arg(i), tr("%1").arg(data.at(i)));

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
void Setting::ReadIni()
{
    pIni=new QSettings("B:/work/CanDriveTest/Setting/config.ini",QSettings::IniFormat);
    pIni->setIniCodec(QTextCodec::codecForName("GBK"));
//    for(int i=0;i<55;i++)
//    {
//        QString nMain = pIni->value("/setting/value").toString();
//        QString str = pIni->value("/setting/type").toString();
//        qDebug()<< "value = " << nMain;
//        qDebug()<< "type = " <<str;
////        qDebug() << "country = " << str;
//    }


    QString sArg1 = "";
    QString sArg2 = "";
    for(int i=0;i<56;i++)
    {
    sArg1 = pIni->value("/setting/value"+tr("%1").arg(i)).toString();
        qDebug("value = %s\n", sArg1.toStdString().data());

    sArg2 = pIni->value("/setting/type"+tr("%1").arg(i)).toString();
        qDebug("type= %s\n", sArg2.toStdString().data());
    }
}
