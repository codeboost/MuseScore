#include "xhighlight.h"

XHighlight::XHighlight(QGraphicsItem *parent): QGraphicsEllipseItem(parent)
{
    setBrush(QColor("#33ee33"));
    setPen(QColor("#ddd"));
    hideAnimation = new QPropertyAnimation(this, "opacity");
    hideAnimation->setStartValue(1.0);
    hideAnimation->setEndValue(0.0);
    hideAnimation->setDuration(50);


    scaleAnimation = new QPropertyAnimation(this, "scale");
    scaleAnimation->setStartValue(0.0);
    scaleAnimation->setEndValue(1.0);
    scaleAnimation->setDuration(50);
}

void XHighlight::hideAnimated()
{
    setTransformOriginPoint(rect().width() / 2, rect().height() / 2);
    scaleAnimation->setDirection(QAbstractAnimation::Backward);
    hideAnimation->start();
    scaleAnimation->start();
}

void XHighlight::hideAfter(int msecs)
{
    QTimer::singleShot(msecs, this, SLOT(hideAnimated()));
}

void XHighlight::showAnimated()
{
    setTransformOriginPoint(rect().width() / 2, rect().height() / 2);
    setOpacity(1.0);
    scaleAnimation->setDirection(QAbstractAnimation::Forward);
    scaleAnimation->start();
}
