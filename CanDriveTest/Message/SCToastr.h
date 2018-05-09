#ifndef SCToastr_H
#define SCToastr_H

#include <QLabel>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDebug>

/**
 * @brief The SCToastr class 自动消失的提示框
 */
class   SCToastr : public QLabel
{
    Q_OBJECT
public:

    enum SCToastrType{
        E_Waitting = -1,     //一直等待
        E_Info = 0,             //普通
        E_Warning,            //警告
        E_Error                  //错误
    };
    explicit SCToastr(SCToastr::SCToastrType type = E_Info, QWidget *parent = 0,
                      const QString &message = QString(),
                      int w = 200,
                      int h = 40);
    void toastr();

    int getHeight(const QString &text, int width);
    void startTimer(bool isStartTimer, int msc);


    ~SCToastr();

    void updateStyle(SCToastr::SCToastrType type);

    void stopAnimationTimer();
    void setResizeEvent(QResizeEvent *event);
private:
    /**
     * @brief setShadow 设置窗体阴影效果
     * @param w 窗体指针
     * @param blurRadius 阴影清晰度，越小越清晰
     * @param dx x方向阴影位置和偏移量，正值在右方，值越大偏移越大
     * @param dy y方向阴影位置和偏移量，正值在下方，值越大偏移越大
     * @return
     */
    void setShadow(QWidget *w, qreal blurRadius=10, qreal dx=5, qreal dy=5);
public slots:

    void slotWaitTime();
private slots:
    void animationFinished();
signals:
    void sigDeleteSelf();
private:
    int startx;
    int starty;
    int endy;
    bool _isStartTimer = true; //开启定时器
    int _timeOutCount = 3000;
    QTimer *_waitTimer = Q_NULLPTR;//等待时间
    QTimer *animationTimer = Q_NULLPTR;//悬停时间
    int _pointSize = 0;
    QPropertyAnimation *pAnimationPox = Q_NULLPTR;
    SCToastr::SCToastrType _type = SCToastr::E_Info;

};

#endif // SCToastr_H
