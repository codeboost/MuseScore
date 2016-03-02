#include "xhighlight.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>
#include <QEasingCurve>
#include <QDebug>

static QPointF lightOffset = QPoint(1, 1);

namespace vg
{

    class XHighlight::Impl
    {
    public:
        QTimer hideTimer;
        Impl()
        {
            hideTimer.setSingleShot(true);
        }
    };

    XHighlight::XHighlight(QGraphicsItem *parent): QGraphicsEllipseItem(parent)
    {
        impl = new Impl();

        setBrush(QColor("#387EFF"));
        setPen(QColor("#ddd"));

        scaleAnimation = new QPropertyAnimation(this, "scale");
        scaleAnimation->setStartValue(0.0);
        scaleAnimation->setEndValue(1.0);
        scaleAnimation->setDuration(50);

        hideAnimation = new QPropertyAnimation(this, "opacity");
        hideAnimation->setStartValue(1.0);
        hideAnimation->setEndValue(0.0);
        hideAnimation->setDuration(20);

        QEasingCurve easingCurve = QEasingCurve::InOutBack;
        easingCurve.setOvershoot(5.1); //1.70 ==> 10%
        scaleAnimation->setEasingCurve(easingCurve);


        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
        effect->setOffset(lightOffset);
        setGraphicsEffect(effect);

        connect(&impl->hideTimer, SIGNAL(timeout()), this, SLOT(hideAnimated()));
    }

    void XHighlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);

        //setGradient
        QPointF gradientCenter = rect().center() - lightOffset;
        QRadialGradient gradient(gradientCenter, rect().width());
        gradient.setColorAt(0, options.gradient0);
        gradient.setColorAt(1, options.gradient1);
        painter->setBrush(gradient);

        QPen strokePen = QColor("#eee");
        int strokeSize = options.strokeSize;

        strokePen.setWidth(strokeSize);
        painter->setPen(strokePen);
        QRectF r = rect() - QMarginsF(strokeSize, strokeSize, strokeSize, strokeSize);
        painter->drawEllipse(r);

        QTextOption textOption = QTextOption(Qt::AlignHCenter | Qt::AlignVCenter);
        painter->drawText(r, options.text, textOption);
    }

    QVariant XHighlight::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    {
        return QGraphicsItem::itemChange(change, value);
    }

    void XHighlight::hideAnimated()
    {
        hideAnimation->setStartValue(opacity());
        hideAnimation->start();
    }

    void XHighlight::hideAfter(int msecs)
    {
        impl->hideTimer.stop();
        impl->hideTimer.start(msecs);
    }

    void XHighlight::showAnimated()
    {
        setOpacity(1.0);
        setTransformOriginPoint(rect().width() / 2, rect().height() / 2);
        scaleAnimation->setDirection(QAbstractAnimation::Forward);
        scaleAnimation->start();
    }

}
