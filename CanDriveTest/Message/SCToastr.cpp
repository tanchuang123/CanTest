
#include "SCToastr.h"

SCToastr::SCToastr(SCToastr::SCToastrType type, QWidget *parent, const QString &message, int w, int h)
    :QLabel(parent)
{
    _type = type;

    this->setWordWrap(true);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //    setFixedWidth(w);
    //    setFixedSize(w+30, getHeight(message,w));
    this->resize(w+30,getHeight(message,w));
    this->setAlignment(Qt::AlignCenter);
    this->setShadow(this);
    this->setObjectName("SCToastr");
    //更新样式
    updateStyle(type);

    switch (type) {
    case E_Waitting:
    {
        _pointSize = 0;
        _waitTimer = new QTimer(this);
        connect(_waitTimer,SIGNAL(timeout()),this,SLOT(slotWaitTime()));
        startTimer(false,_timeOutCount);
    }
        break;
    case E_Info:
        break;
    case E_Warning:
        break;
    case E_Error:
        break;
    default:
        break;
    }
    this->setText(message);
}
SCToastr::~SCToastr()
{
    stopAnimationTimer();
    if(_waitTimer){
        if(_waitTimer->isActive()){
            _waitTimer->stop();
        }
        delete _waitTimer;
    }
    if(pAnimationPox){
        pAnimationPox->deleteLater();
    }
}

void SCToastr::updateStyle(SCToastr::SCToastrType type)
{
    QString colorStr = "";
    switch (type) {
    case E_Waitting:
        colorStr = "color:green;";
        break;
    case E_Info:
        colorStr = "color:green;";
        break;
    case E_Warning:
        colorStr = "color:yellow;";
        break;
    case E_Error:
        colorStr = "color:red;";
        break;
    default:
        break;
    }

    this->setStyleSheet(QString("QLabel#SCToastr{"
                                "font: 12pt \"\"微软雅黑\"\";"
                                "background:rgba(200,200,200,245);"
                                "%1"
                                "border-radius:3px;"
                                "}").arg(colorStr));
}
void SCToastr::setShadow(QWidget *w, qreal blurRadius, qreal dx, qreal dy)
{
    //    //QGraphicsDropShadowEffect(this):会留下残影
    //    QGraphicsDropShadowEffect *_shadow = new QGraphicsDropShadowEffect;
    //    _shadow->setBlurRadius(blurRadius);
    //    _shadow->setColor(QColor(0, 0, 0, 255));
    //    _shadow->setOffset(dx, dy);
    //    w->setGraphicsEffect(_shadow);
}
void SCToastr::slotWaitTime()
{
    if(!this->text().isEmpty()){
        if(_pointSize>2){
            _pointSize = 0;
            QString text = this->text();
            text = text.replace(".","");
            this->setText(text);
        }else{
            ++_pointSize;
            this->setText(this->text()+".");
        }
    }
}
/** 根据宽度和文本长度计算出高度
 * @brief SCToastr::getHeight
 * @param text
 * @param width
 * @return
 */
int SCToastr::getHeight(const QString&text,int width)
{
    //计算字符串宽度
    QFont font("\"微软雅黑\"", 13);
    QFontMetrics fontMetrics(font);
    int fWidth = fontMetrics.width(text);
    int numberOfLine = fWidth/width+2;
    return fontMetrics.height()*numberOfLine;
}
void SCToastr::toastr()
{
    this->show();
    //-------
    startx = (this->parentWidget()->width()-this->width()) / 2;
    starty = -this->height()+parentWidget()->height()*0.2;
    endy = parentWidget()->height()*0.2+5;
    //--------
    if(!pAnimationPox){
        pAnimationPox = new QPropertyAnimation(this,"pos");
    }
    //    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "windowOpacity");
    //    pAnimation->setDuration(800);
    //    pAnimation->setKeyValueAt(0, 1);
    //    pAnimation->setKeyValueAt(0.5, 0);
    //    pAnimation->setKeyValueAt(1, 1);
    //    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    pAnimationPox->setDuration(300);
    pAnimationPox->setStartValue(QPoint(startx,starty));
    pAnimationPox->setEndValue(QPoint(startx,endy));
    pAnimationPox->setEasingCurve(QEasingCurve::OutCubic);
    connect(pAnimationPox, SIGNAL(finished()), this, SLOT(animationFinished()));

    pAnimationPox->start();

    if(_waitTimer && SCToastr::E_Waitting == _type){//如果定时器new则开启
        _waitTimer->start(1000);
    }else if(_waitTimer){
        _waitTimer->stop();
    }
}
void SCToastr::startTimer(bool isStartTimer,int msc)
{
    _isStartTimer = isStartTimer;
    _timeOutCount = msc;
}
void SCToastr::stopAnimationTimer()
{
    if(animationTimer){
        animationTimer->stop();
        delete animationTimer;
        animationTimer = Q_NULLPTR;
    }
}
void SCToastr::animationFinished()
{
    if(_isStartTimer){
        stopAnimationTimer();
        animationTimer = new QTimer(this);
        animationTimer->setSingleShot(true);
        connect(animationTimer,SIGNAL(timeout()), this, SIGNAL(sigDeleteSelf()));
        animationTimer->start(_timeOutCount);
    }
}
void SCToastr::setResizeEvent(QResizeEvent *event)
{
    if(this && this->parentWidget()){
        startx = (this->parentWidget()->width()-this->width()) / 2;
        this->move(startx,this->y());
    }
}
