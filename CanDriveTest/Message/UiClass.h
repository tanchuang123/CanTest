#ifndef UiClass_H
#define UiClass_H

#include <QObject>
#include <QPropertyAnimation>
#include <QMenuBar>
#include <QPushButton>
#include "SCToastr.h"
#include <QTextEdit>
#include <QMutex>

class   UiClass : public QObject
{
    Q_OBJECT
public:
    explicit UiClass(QObject*parent = 0);
    ~UiClass();
    static UiClass *init();
    static UiClass *getThis();


    void showToastr(const QString &msg, SCToastr::SCToastrType type = SCToastr::E_Info , QWidget *w =  Q_NULLPTR, bool showMask = false);


    bool setToastrText(const QString &msg);

    void setResizeEvent(QResizeEvent *event);

    bool isToastrShow();
    void setMainWindow(QWidget *mainW);
signals:

public slots:

    void slotDeleteToastr();
private:
    static UiClass *pUiClass;
    QMutex _mutex;
    QWidget *_mainW = Q_NULLPTR;
    int _iTitleBarHeight = 0;//30
    SCToastr * pSCToastr = Q_NULLPTR;

};

#endif // UiClass_H
