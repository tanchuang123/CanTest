#ifndef MARQUEEWIDGET_H
#define MARQUEEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimerEvent>
class MarqueeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MarqueeWidget(QWidget *parent = 0);

    void setText(const QString &newText);
    inline QString text() const { return m_myText.trimmed(); }

    QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *event) override;//override是指派生类重写基类的虚函数
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QString m_myText;
    int m_offset, m_myTimerId;


};

#endif // MARQUEEWIDGET_H
