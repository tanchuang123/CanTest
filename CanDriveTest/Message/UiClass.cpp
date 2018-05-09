#include "UiClass.h"
#include <QDateTime>
#include <QHBoxLayout>
#include <QStyle>
#include <QMutexLocker>


//init static variable
UiClass *UiClass::pUiClass = Q_NULLPTR;

UiClass::UiClass(QObject *parent):QObject(parent)
{

}

UiClass::~UiClass()
{

}
UiClass* UiClass::getThis()
{
    return pUiClass;
}
/** 消息提示框：
   E_Waitting = -1,     //一直等待
   E_Info = 0,             //普通
   E_Warning,            //警告
   E_Error                  //错误
 * @brief UiClass::showToastr
 * @param msg
 * @param type
 * @param w
 * @param showMask 显示遮罩
 */
void UiClass::showToastr(const QString &msg, SCToastr::SCToastrType type, QWidget *w,bool showMask)
{
    _mutex.lock();
    slotDeleteToastr();

    if( !w ){
        if( !_mainW ){
            qWarning()<<QStringLiteral("窗口没有设置父类");
            _mutex.unlock();
            return;
        }
        w = _mainW;
    }
    pSCToastr = new SCToastr(type,w, msg);



    pSCToastr->setAttribute(Qt::WA_AlwaysShowToolTips);
    connect(pSCToastr,SIGNAL(sigDeleteSelf()),this,SLOT(slotDeleteToastr()));

    pSCToastr->toastr();
    _mutex.unlock();
}
/** 提示框存在时，切换文字显示
 * @brief UiClass::setToastrText
 * @param msg
 * @return
 */
bool UiClass::setToastrText(const QString &msg)
{
    if(pSCToastr){
        pSCToastr->setText(msg);
        return true;
    }
    return false;
}
bool UiClass::isToastrShow()
{
    if(pSCToastr && !pSCToastr->isHidden()){
        return true;
    }
    return false;
}
/** 删除抽屉提示窗口
 * @brief UiClass::slotDeleteToastr
 */
void UiClass::slotDeleteToastr()
{
    if(pSCToastr){
        delete pSCToastr;
        pSCToastr = Q_NULLPTR;
    }
}

/** 初始化
 * @brief UiClass::init
 * @return
 */
UiClass* UiClass::init()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if(!pUiClass){
        pUiClass = new UiClass;
    }
    return pUiClass;
}


/** 设置程序主界面
 * @brief UiClass::setMainWindow
 * @param mainW
 */
void UiClass::setMainWindow(QWidget *mainW)
{
    _mainW = mainW;
}


/** 窗口大小
 * @brief UiClass::setResizeEvent
 * @param event
 */
void UiClass::setResizeEvent(QResizeEvent *event)
{
    _mutex.lock();

    if(pSCToastr && !pSCToastr->isHidden()){
        pSCToastr->setResizeEvent(event);
    }

    _mutex.unlock();
}








