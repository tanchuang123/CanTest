#ifndef CONTROLCANDLL_H
#define CONTROLCANDLL_H
#include <QObject>
#include "lib/ControlCAN.h"
#include <QString>
#include <QMap>
#include <QThread>
#include <QTimer>
class ControlCanDll
{
//    Q_OBJECT
public:
    ControlCanDll();
    static ControlCanDll*init();
    VCI_INIT_CONFIG init_config();
  _VCI_CAN_OBJ init_Device_can_obj();

    void initMenber();
    bool OpenDevice();
    bool CloseDevice();
    bool InitCAN();
    bool StartCAN();
    bool Transmit(UINT id);
    QStringList Receive();
    bool sendData(int index,int value,int value_command_type,int valueParameter_address,int commandValue);//A路B路数据查询发送
    QMap<int,QVariant> Data_Back(int index);
    VCI_CAN_OBJ Receive_info();
//    QStringList Receive_info();
    int bytesToInt(byte* bytes,int size = 4);
    bool getReciveStatus()
    {
        if(!_isRecive)
        {
            _isRecive=true;
        }
        return _isRecive;
    }
   bool getSendStatus()
   {
       if(!_isSend)
       {
           _isSend=true;
       }
       return _isSend;
   }
     QMap<int,QVariant> ConfigData_Back(int index);
//     bool send_Config_Data(int index, int value_command_type,int value2,int value);
     QMap<int,QVariant> read_configdata_back(int index);
//     QMap<int,QVariant> ReadConfigdataBack(int index);
     QMap<int,QVariant> SystemCommandBack(int index);
     bool openHallDetectionA(int index);
     bool openHallDetectionB(int index);
     QMap<int ,QVariant> HallDetectionA(int index);
     QMap<int,QVariant>  HallDetectionB(int index);
     void WriteData(int index,float value);
     void  getStatusMap_(QMap<int,QVariant> map)
     {
         if(!_varMapStatus.isEmpty())
         {
             _varMapStatus.clear();
             _varMapStatus = map;
         }
         _varMapStatus = map;
     }
     QMap<int,QVariant> getStatusMap()
     {
         return _varMapStatus;
     }
private:
    DWORD   _nDeviceType=4 ; /* USBCAN-2A或USBCAN-2C或CANalyst-II */
    DWORD   _nDeviceInd=0; /* 第1个设备 */
    DWORD   _nCANInd=0;//第一条通道
    unsigned long _size_num=1;
    VCI_INIT_CONFIG _vic;
    struct _VCI_CAN_OBJ p_vco;
    static ControlCanDll *pControlCanDll;
    QStringList _data;
    QThread * idThread ;
    QThread * dataThread ;
    QMap <int ,QVariant>_varMapStatus;
    QTimer *hallTimer;
    bool _isRecive=true;
    bool _isSend   =true;
};

#endif // CONTROLCANDLL_H
