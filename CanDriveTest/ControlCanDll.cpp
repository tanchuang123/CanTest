#include "ControlCanDll.h"
#include <QtDebug>
#include <QTextEdit>
#include <QMessageBox>
ControlCanDll * ControlCanDll::pControlCanDll = Q_NULLPTR;

ControlCanDll::ControlCanDll()
{
     init_config();
     idThread = new QThread;
     dataThread = new QThread;
//     hallTimer = new QTimer(this);
//     connect(hallTimer,SIGNAL(timeout()),this,SLOT(slotHallTest()));
}
ControlCanDll *ControlCanDll::init()
{
    if(!pControlCanDll){
        pControlCanDll = new ControlCanDll;
    }
    return pControlCanDll;
}
VCI_INIT_CONFIG ControlCanDll::init_config()
{
    _vic.AccCode=0x00000000;
    _vic.AccMask=0xFFFFFFFF;//屏蔽码
    _vic.Filter=0;                       //滤波方式
    _vic.Timing0=0x01;            //波特率定时器 0（BTR0）
    _vic.Timing1=0x1c;            //波特率定时器 1（BTR1）
    _vic.Mode=0;                     //模式。=0表示正常模式（相当于正常节点），=1表示只听模式（只接收，不影响总线），=2表示自发自收模式（环回模式）
    _vic.Reserved=0;
    return _vic;
}

_VCI_CAN_OBJ ControlCanDll::init_Device_can_obj()
{

    p_vco.ID = 0x701;

    p_vco.RemoteFlag =0;//是否是远程帧
    p_vco.ExternFlag =0;//是否是扩展帧
    p_vco.DataLen = 8;
    p_vco.SendType=0;//发送类型
    for(int i=0;i<8;i++)
    {
        p_vco.Data[i]=0;
    }

//    qDebug()<<p_vco.ID<<p_vco.DataLen<<"id datalen";
    return p_vco;
}
bool ControlCanDll::OpenDevice()//打开CAN卡
{

    DWORD status_open;
    status_open = VCI_OpenDevice(_nDeviceType, _nDeviceInd, 0);
    if(status_open != 1)
    {
       return false;
    }
    else
    {
        return true;
    }
}
bool ControlCanDll::CloseDevice()//关闭Can卡
{

    DWORD status_close;
    status_close = VCI_CloseDevice(_nDeviceType, _nDeviceInd);
    if(status_close != 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool ControlCanDll::InitCAN()//初始化Can通道
{
    /* 第1个通道 */
    DWORD dwRel;

    if(!OpenDevice())
    {
        return false;
    }

    dwRel = VCI_InitCAN(_nDeviceType, _nDeviceInd, _nCANInd, &_vic);
    if(dwRel !=1)
    {
        CloseDevice();
        return false;
    }
    else
    {
        return true;
    }
}
bool ControlCanDll::StartCAN()
{
    DWORD dwRel;
    if(!InitCAN()||!OpenDevice())
    {
        return false;
    }
    else
    {
        dwRel=VCI_StartCAN(_nDeviceType, _nDeviceInd, _nCANInd);

        if(dwRel!=1)
        {
            CloseDevice();
            return false;
        }
        else {
            return true;
        }

    }

}
bool ControlCanDll::Transmit(UINT id)//发送数据
{

    DWORD dwRel;
    VCI_CAN_OBJ vco[48];
    for(int i=0;i<48;i++)
    {
    vco[i].ID = id;
    vco[i].RemoteFlag = 0;
    vco[i].ExternFlag = 0;
    vco[i].DataLen = 8;
    for(int  j = 0; j < 8; j++)
      {
          vco[i].Data[j] =0;
      }

    }

    dwRel = VCI_Transmit (_nDeviceType, _nDeviceInd, _nCANInd, vco,_size_num);

    return dwRel;

}
QStringList  ControlCanDll::Receive()
{
    idThread->start();
    VCI_CAN_OBJ receivedata;
    QStringList reciveStrList;
    int len ,i;
//    Sleep(1);
   len=VCI_Receive(_nDeviceType, _nDeviceInd, _nCANInd,&receivedata,2500,200);
   if(len<=0)
   {
        reciveStrList<<"NULL"<<"NULL"<<"NULL"<<"NULL"<<"NULL"<<"NULL";
        CloseDevice();
        return reciveStrList;
   }
   else
   {
       for(i=0;i<len;i++)
       {
           for(int j=0;j<receivedata.DataLen;j++)
           {
//               qDebug()<<len<<"len"<<receivedata.ID<< receivedata.DataLen<<"receivedata[a].DataLen"<<receivedata.Data[j]<<"receivedata.Data[j]";
               QString reciveStr = QString("0%1").arg(receivedata.Data[j]);
               reciveStrList.append(reciveStr);
           }
           _data= reciveStrList;
            idThread->quit();
           return reciveStrList;
       }

   }

}

bool ControlCanDll::sendData(int index,int value,int value_command_type,int valueParameter_address,int commandValue)//A路B路数据查询发送
{
    DWORD dwRel;
    VCI_CAN_OBJ vco[48];
    _isSend=false;
   QString id_text= _data.at(index);//启动报文返回的ID
   if(index !=4)
   {
    BYTE byte_arry[15]={0x00,
                        0x01,
                        0x02,
                        0x03,
                        0x04,
                        0x10,
                        0x20,
                        0x21,
                        0x22,
                        0x23,
                        0x24,
                        0x25,
                        0x26,
                        0x27,
                        0x28,};

    for(int i=0;i<48;i++)
    {
        vco[i].ID =  id_text.toUInt();
        vco[i].RemoteFlag = 0;
        vco[i].ExternFlag = 0;
        vco[i].DataLen = 6;

        vco[i].Data[0] = 0x00;
        vco[i].Data[1] = 0x00;
        vco[i].Data[2] = 0x00;
        vco[i].Data[3] = 0x00;
        vco[i].Data[4] = byte_arry[value];//反馈类型
        vco[i].Data[5] = 0x00;
    }
   }
   else
   {
       BYTE byte_arry_command_type[]={
                           0x00,//系统命令0
                           0x01,//写入参数1
                           0x02,//读取参数2
                           0x03,//霍尔侦测3
                                };

       BYTE system_command_Parameter_address[]=//系统命令
       {
           0x00,//空命令
           0x01,//保存参数到Flash,并且复位芯片
           0x02,//查询flash参数使能状态
       };
       BYTE write_read_Parameter_address[]=//写入参数//读取参数
       {
           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x10,0x11,/*int*/0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,/*float*/0x1B,0x1C,
           0x1D,0x1E,0x1F,0x30,/*int*/0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,
           0x41,0x42,/*float*/0x50,/*int*/0x51,0x52,0x53,0x54,0x55,0x56,/*float*/0x60,/*int*/0x61,0x62,0x63,0x64,0x65,0x66,/*float*/
       };
       BYTE hall_test[]=//霍尔侦测
       {
           0x01,//点机A启动霍尔侦测相位侦测
           0x02,//电机A霍尔状态查询
           0x03,//电机B启动霍尔相位侦测
           0x04,//电机B霍尔状态查询
       };

       for(int i=0;i<48;i++)
       {
           vco[i].ID =  id_text.toUInt(0,16);
           vco[i].RemoteFlag = 0;
           vco[i].ExternFlag = 0;
           vco[i].DataLen = 6;

           vco[i].Data[0] = 0x00;//配置数据
           vco[i].Data[1] = 0x00;//配置数据
           vco[i].Data[2] = 0x00;//配置数据
           vco[i].Data[3] = 0x00;//配置数据

           if(value_command_type==0)//系统命令
           {
            vco[i].Data[4] = system_command_Parameter_address[valueParameter_address];//参数地址
           }
           if(value_command_type==1)//写入参数
           {
            vco[i].Data[4] = write_read_Parameter_address[valueParameter_address];//参数地址
           }
           if(value_command_type==2)//读取参数
           {
            vco[i].Data[4] = write_read_Parameter_address[valueParameter_address];//参数地址
           }
           if(value_command_type==3)//霍尔侦测
           {
            vco[i].Data[4] = hall_test[valueParameter_address];//参数地址
//            qDebug()<<vco[i].Data[4]<<"vco[i].Data[4]";
           }
           vco[i].Data[5] = byte_arry_command_type[commandValue];//命令类型
//           qDebug()<<vco[i].Data[4]<<"vco[i].Data[4]"<<vco[i].Data[5]<<"vco[i].Data[5]";
       }
   }

    dwRel = VCI_Transmit(_nDeviceType, _nDeviceInd, _nCANInd, vco,_size_num);
    return dwRel;
}

QMap<int,QVariant> ControlCanDll::Data_Back(int index)
{

    QMap<int,QVariant> varMap;
    for(int i=0;i<15;i++)
    {
        switch (i) {
        case 0:
        {
            if(sendData(index,0,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                int value= bytesToInt(receivedata.Data);
                varMap.insert(0x00,value);
            }
            break;
        }
        case 1:
        {
            if(sendData(index,1,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                int value= bytesToInt(receivedata.Data);
                varMap.insert(0x01,value);
            }
            break;
        }
        case 2:
        {
            if(sendData(index,2,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x02,*p );
            }
            break;
        }
        case 3:
        {
            if(sendData(index,3,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x03,*p );
            }
            break;
        }
        case 4:
        {
            if(sendData(index,4,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                int value= bytesToInt(receivedata.Data);
                varMap.insert(0x04,value );
            }
            break;
        }
        case 5:
        {
            if(sendData(index,5,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                 int value= bytesToInt(receivedata.Data);
                 varMap.insert(0x10,value);
//                 unsigned char buf[4];
//                  buf[0] = value>>24;
//                  buf[1] = value>> 16;
//                  buf[2] = value >> 8;
//                  buf[3] = value;
//                qDebug()<<value<<"0x10"<<receivedata.ID<< buf[0]<<buf[1]<< buf[2]<<buf[3]<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
             }
        break;
        }
        case 6:
        {
            if(sendData(index,6,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                int value=bytesToInt(receivedata.Data);
                varMap.insert(0x20,value );
            }
            break;
        }
        case 7:
        {
            if(sendData(index,7,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x21,*p );
            }
            break;
        }
        case 8:
        {
            if(sendData(index,8,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x22,*p );
            }
            break;
        }
        case 9:
        {
            if(sendData(index,9,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x23,*p );
            }
            break;
        }
        case 10:
        {
            if(sendData(index,10,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x24,*p );
            }
            break;
        }
        case 11:
        {
            if(sendData(index,11,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x25,*p );
            }
            break;
        }
        case 12:
        {
            if(sendData(index,12,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x26,*p );
            }
            break;
        }
        case 13:
        {
            if(sendData(index,13,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x27,*p );
            }
            break;
        }
        case 14:
        {
            if(sendData(index,14,0,0,0))
            {
                VCI_CAN_OBJ receivedata= Receive_info();
                unsigned char pMem[] = {receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)pMem;
                varMap.insert(0x28,*p );
            }
            break;
        }

        default:
            break;
        }

    }

    return  varMap;
}
VCI_CAN_OBJ ControlCanDll::Receive_info()
{
    dataThread->start();
    _isRecive = false;
    VCI_CAN_OBJ receivedata;
    QStringList reciveStrList;
    int len ;
   Sleep(1);
   len=VCI_Receive(_nDeviceType, _nDeviceInd, _nCANInd,&receivedata,2500,200);
   if(len<0)
   {
        reciveStrList<<"NULL"<<"NULL"<<"NULL"<<"NULL"<<"NULL"<<"NULL";
        CloseDevice();
         return receivedata;
   }
   else
   {
              QString debugStr0 = QString("%1").arg(receivedata.Data[0],8,16,QLatin1Char('0'));
              QString debugStr1 = QString("%1").arg(receivedata.Data[1],8,16,QLatin1Char('0'));
              QString debugStr2 = QString("%1").arg(receivedata.Data[2],8,16,QLatin1Char('0'));
              QString debugStr3 = QString("%1").arg(receivedata.Data[3],8,16,QLatin1Char('0'));
              QString debugStr4 = QString("%1").arg(receivedata.Data[4],8,16,QLatin1Char('0'));
              QString debugStr5 = QString("%1").arg(receivedata.Data[5],8,16,QLatin1Char('0'));
              QString debugStr6 = QString("%1").arg(receivedata.Data[6],8,16,QLatin1Char('0'));
              QString debugStr7 = QString("%1").arg(receivedata.Data[7],8,16,QLatin1Char('0'));
//              qDebug()<<debugStr0<<debugStr1<<debugStr2<<debugStr3<<debugStr4<<debugStr5<<"receivedata.ID"<<receivedata.ID<<"receive";
//              qDebug()<<debugStr0.toLower() <<debugStr1.toUInt()<<debugStr2.toUInt()<<debugStr3.toUInt()<<"ERROR";
             return receivedata;
   }

}
QMap<int , QVariant> ControlCanDll::SystemCommandBack(int index)
{
    QMap<int,QVariant> varMap;

    {
                if(sendData(index,0,0,2,0))
                {
                VCI_CAN_OBJ receivedata=Receive_info();
                BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                int value=bytesToInt(BYTES,4);
                varMap.insert(2,value);
                }
     }

    return  varMap;
}
QMap<int,QVariant> ControlCanDll::read_configdata_back(int index)//读取参数
{
    QMap<int,QVariant> varMap;
    QMap<int,QVariant>  varMapStatus;
    for(int i=0;i<9;i++)
    {

           if(sendData(index,0,2,i,2))
           {
               VCI_CAN_OBJ receivedata=Receive_info();
               if(receivedata.Data[4]==0x0)
               {
                   BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                   int values=bytesToInt(BYTES,4);
                   varMap.insert(i,values);
                   varMapStatus.insert(i,QStringLiteral("OK"));
               }
               else {
                   switch (receivedata.Data[4])
                   {
                   case 0x1:
                   {
                        varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                   }
                       break;
                   case 0x2:
                   {
                       varMapStatus.insert(i,QStringLiteral("校验错误"));
                   }
                       break;
                   case 0x3:
                   {
                       varMapStatus.insert(i,QStringLiteral("解包失败"));
                   }
                       break;
                   case 0x4:
                   {
                       varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                   }
                       break;
                   default:
                       break;
                   }
               }
           }

   }
    for(int i=9;i<18;i++)
    {
        if(sendData(index,0,2,i,2))
        {
            VCI_CAN_OBJ receivedata=Receive_info();
            if(receivedata.Data[4]==0x0)
            {
                BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)BYTES;
                varMap.insert(i,*p);
                varMapStatus.insert(i,QStringLiteral("OK"));
            }
            else {
                switch (receivedata.Data[4])
                {
                case 0x1:
                {
                     varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                }
                    break;
                case 0x2:
                {
                     varMapStatus.insert(i,QStringLiteral("校验错误"));
                }
                    break;
                case 0x3:
                {
                     varMapStatus.insert(i,QStringLiteral("解包失败"));
                }
                    break;
                case 0x4:
                {
                     varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                }
                    break;
                default:
                    break;
                }
            }
        }
    }
    for(int i=18;i<24;i++)
    {
        if(sendData(index,0,2,i,2))
        {

            VCI_CAN_OBJ receivedata=Receive_info();
            if(receivedata.Data[4]==0x0)
            {
                BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                int values=bytesToInt(BYTES,4);
                varMap.insert(i,values);
                varMapStatus.insert(i,QStringLiteral("OK"));
            }
            else {
                switch (receivedata.Data[4])
                {
                case 0x1:
                {
                     varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                }
                    break;
                case 0x2:
                {
                    varMapStatus.insert(i,QStringLiteral("校验错误"));
                }
                    break;
                case 0x3:
                {
                    varMapStatus.insert(i,QStringLiteral("解包失败"));
                }
                    break;
                case 0x4:
                {
                    varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                }
                    break;
                default:
                    break;
                }
            }
        }
    }
    for(int i=24;i<42;i++)
    {
        if(sendData(index,0,2,i,2))
        {
            VCI_CAN_OBJ receivedata=Receive_info();
            if(receivedata.Data[4]==0x0)
            {
                BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)BYTES;
                varMap.insert(i,*p);
                varMapStatus.insert(i,QStringLiteral("OK"));
            }
            else {
                switch (receivedata.Data[4])
                {
                case 0x1:
                {
                     varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                }
                    break;
                case 0x2:
                {
                    varMapStatus.insert(i,QStringLiteral("校验错误"));
                }
                    break;
                case 0x3:
                {
                    varMapStatus.insert(i,QStringLiteral("解包失败"));
                }
                    break;
                case 0x4:
                {
                    varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                }
                    break;
                default:
                    break;
                }
            }
        }
    }
    if(sendData(index,0,2,42,2))
    {
        VCI_CAN_OBJ receivedata=Receive_info();
         if(receivedata.Data[4]==0x0)
         {
             BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
             int values=bytesToInt(BYTES,4);
             varMap.insert(42,values);
             varMapStatus.insert(42,QStringLiteral("OK"));
         }
         else {
             switch (receivedata.Data[4])
             {
             case 0x1:
             {
                  varMapStatus.insert(42,QStringLiteral("命令发送超时"));
             }
                 break;
             case 0x2:
             {
                 varMapStatus.insert(42,QStringLiteral("校验错误"));
             }
                 break;
             case 0x3:
             {
                 varMapStatus.insert(42,QStringLiteral("解包失败"));
             }
                 break;
             case 0x4:
             {
                 varMapStatus.insert(42,QStringLiteral("霍尔侦测中，不支持命令读取"));
             }
                 break;
             default:
                 break;
             }
         }

    }
    for(int i=43;i<49;i++)
    {
        if(sendData(index,0,2,i,2))
        {
            VCI_CAN_OBJ receivedata=Receive_info();
             if(receivedata.Data[4]==0x0)
             {
                 BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                 float *p = (float*)BYTES;
                 varMap.insert(i,*p);
                 varMapStatus.insert(i,QStringLiteral("OK"));
             }
             else {
                 switch (receivedata.Data[4])
                 {
                 case 0x1:
                 {
                      varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                 }
                     break;
                 case 0x2:
                 {
                     varMapStatus.insert(i,QStringLiteral("校验错误"));
                 }
                     break;
                 case 0x3:
                 {
                     varMapStatus.insert(i,QStringLiteral("解包失败"));
                 }
                     break;
                 case 0x4:
                 {
                     varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                 }
                     break;
                 default:
                     break;
                 }
             }

        }
    }
    if(sendData(index,0,2,49,2))
    {
        VCI_CAN_OBJ receivedata=Receive_info();
        if(receivedata.Data[4]==0x0)
        {
            BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
            int values=bytesToInt(BYTES,4);
            varMap.insert(49,values);
            varMapStatus.insert(49,QStringLiteral("OK"));
        }

        else {
            switch (receivedata.Data[4])
            {
            case 0x1:
            {
                 varMapStatus.insert(49,QStringLiteral("命令发送超时"));
            }
                break;
            case 0x2:
            {
                varMapStatus.insert(49,QStringLiteral("校验错误"));
            }
                break;
            case 0x3:
            {
                varMapStatus.insert(49,QStringLiteral("解包失败"));
            }
                break;
            case 0x4:
            {
                varMapStatus.insert(49,QStringLiteral("霍尔侦测中，不支持命令读取"));
            }
                break;
            default:
                break;
            }
        }

    }
    for(int i=50;i<56;i++)
    {
        if(sendData(index,0,2,i,2))
        {
            VCI_CAN_OBJ receivedata=Receive_info();
            if(receivedata.Data[4]==0x0)
            {
                BYTE BYTES[4]={receivedata.Data[3],receivedata.Data[2],receivedata.Data[1],receivedata.Data[0]};
                float *p = (float*)BYTES;
                varMap.insert(i,*p);
                varMapStatus.insert(i,QStringLiteral("OK"));
            }
            else {
                switch (receivedata.Data[4])
                {
                case 0x1:
                {
                     varMapStatus.insert(i,QStringLiteral("命令发送超时"));
                }
                    break;
                case 0x2:
                {
                    varMapStatus.insert(i,QStringLiteral("校验错误"));
                }
                    break;
                case 0x3:
                {
                    varMapStatus.insert(i,QStringLiteral("解包失败"));
                }
                    break;
                case 0x4:
                {
                    varMapStatus.insert(i,QStringLiteral("霍尔侦测中，不支持命令读取"));
                }
                    break;
                default:
                    break;
                }
            }

        }
    }
    getStatusMap_(varMapStatus);
    return varMap;
}

bool ControlCanDll::openHallDetectionA(int index)
{
     QMap<int,QVariant> varMap;

      if(sendData(index,0,3,0,3))
      {
           VCI_CAN_OBJ receivedata=Receive_info();
           BYTE BYTES[4]={receivedata.Data[0],receivedata.Data[1],receivedata.Data[2],receivedata.Data[3]};
           int value=bytesToInt(BYTES,4);
           if(value==0)
           {
               return true;
           }
      }
}
bool ControlCanDll::openHallDetectionB(int index)
{
     if(sendData(index,0,3,2,3))
     {
           VCI_CAN_OBJ receivedata=Receive_info();
           BYTE BYTES[4]={receivedata.Data[0],receivedata.Data[1],receivedata.Data[2],receivedata.Data[3]};
           int value=bytesToInt(BYTES,4);
           if(value==0)
           {
               return true;
           }
     }
}

QMap<int,QVariant> ControlCanDll::HallDetectionA(int index)
{
     QMap<int,QVariant> varMap;
     if(sendData(index,0,3,1,3))
     {
           VCI_CAN_OBJ receivedata=Receive_info();
           BYTE BYTES[4]={receivedata.Data[0],receivedata.Data[1],receivedata.Data[2],receivedata.Data[3]};
           int values=bytesToInt(BYTES,4);
           varMap.insert(0x02,values);
     }
     return varMap;
}
QMap<int,QVariant> ControlCanDll::HallDetectionB(int index)
{
    QMap<int,QVariant> varMap;
     if(sendData(index,0,3,3,3))
     {
           VCI_CAN_OBJ receivedata=Receive_info();
           BYTE BYTES[4]={receivedata.Data[0],receivedata.Data[1],receivedata.Data[2],receivedata.Data[3]};
           int values=bytesToInt(BYTES,4);
           varMap.insert(0x04,values);
     }
    return varMap;
}
//byte转INT
int ControlCanDll::bytesToInt(byte* bytes,int size)
{

  int addr  =  bytes[3] & 0xFF;
       addr |= (bytes[2] & 0x00FF);
       addr |= (bytes[1] & 0x0000FF);
       addr |= (bytes[0] & 0x000000FF);
        return addr;

}
int ControlCanDll::WriteData(int index,float value,int valueParameter_address)
{

    BYTE write_read_Parameter_address[]=//写入参数//读取参数
    {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x10,0x11,/*int*/0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,/*float*/0x1B,0x1C,
        0x1D,0x1E,0x1F,0x30,/*int*/0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,
        0x41,0x42,/*float*/0x50,/*int*/0x51,0x52,0x53,0x54,0x55,0x56,/*float*/0x60,/*int*/0x61,0x62,0x63,0x64,0x65,0x66,/*float*/

    };

    union FloatToByte
    {
      unsigned char b[4];
      float value;
    } FtoB;
       DWORD dwRel;
       VCI_CAN_OBJ vco[48];
       QString id_text= _data.at(index);//启动报文返回的ID

       FtoB.value= value;
       for(int i=0;i<48;i++)
       {
           vco[i].ID =  id_text.toUInt(0,16);
           vco[i].RemoteFlag = 0;
           vco[i].ExternFlag = 0;
           vco[i].DataLen = 6;

           vco[i].Data[0] = FtoB.b[3];//配置数据
           vco[i].Data[1] = FtoB.b[2];//配置数据
           vco[i].Data[2] = FtoB.b[1];//配置数据
           vco[i].Data[3] = FtoB.b[0];//配置数据
           vco[i].Data[4] = write_read_Parameter_address[valueParameter_address];//参数地址
           vco[i].Data[5] = 0x01;//命令类型
//           qDebug()<<valueParameter_address<<FtoB.value<<vco[i].Data[0]<<vco[i].Data[1]<<vco[i].Data[2]<<vco[i].Data[3]<<vco[i].Data[4]<<vco[i].Data[5] <<"gggggggggggggggggggggggggggggg";
       }
    if(getSendStatus())
    {

        dwRel = VCI_Transmit(_nDeviceType, _nDeviceInd, _nCANInd, vco,_size_num);
        VCI_CAN_OBJ receivedata=Receive_info();
        BYTE BYTES[4]={receivedata.Data[0],receivedata.Data[1],receivedata.Data[2],receivedata.Data[3]};
        int value=bytesToInt(BYTES,4);
        return value;
//        qDebug()<<dwRel<<"ccccccccccccccccccccccc"<<receivedata.Data[0]<<receivedata.Data[1]<<receivedata.Data[2]<<receivedata.Data[3];
    }
}
