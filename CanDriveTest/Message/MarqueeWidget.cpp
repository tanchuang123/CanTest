#include "MarqueeWidget.h"

MarqueeWidget::MarqueeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_offset = 0;
    m_myTimerId = 0;
}

void MarqueeWidget::setText(const QString &newText)
{
    int length = fontMetrics().width( newText );//文本的像素宽度
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    setPalette(pe);
    length = (length >= width()) ? 45 : 25;
    m_myText = newText.leftJustified(length, ' ');

    update();
    updateGeometry();
}

QSize MarqueeWidget::sizeHint() const
{
    return fontMetrics().size(0, m_myText);
}

void MarqueeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QFont f(font());
    f.setBold(false);
    painter.setFont(f);

    int textWidth = fontMetrics().width(m_myText);
    if(textWidth < 1)
    {
        return;
    }
    int x = -m_offset;
    while(x < width())
    {
        painter.drawText(x, 0, textWidth, height(),
                         Qt::AlignLeft | Qt::AlignVCenter, m_myText);
        x += textWidth;
    }
    painter.end();
}

void MarqueeWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_myTimerId = startTimer(30);
}

void MarqueeWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_myTimerId)
    {
        ++m_offset;
        if (m_offset >= fontMetrics().width(m_myText))
        {
            m_offset = 0;
        }
        scroll(-1, 0);
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void MarqueeWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    killTimer(m_myTimerId);
    m_myTimerId = 0;
}

void MarqueeWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setText( text() );
}
