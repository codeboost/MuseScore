#include "xhighlight.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>
#include <QEasingCurve>
#include <QDebug>

static QPointF lightOffset = QPoint(1, 1);

namespace vg
{
    class XHighlight::Impl : public QObject
    {
    public:
        QPropertyAnimation scaleAnimation;
        QPropertyAnimation posAnimation;
        QTimer hideTimer;
        Impl(XHighlight* parent): QObject(parent), scaleAnimation(&parent->innerDot, "scale"), posAnimation(parent, "pos")
        {
            hideTimer.setSingleShot(true);
            scaleAnimation.setStartValue(1.0);
            scaleAnimation.setKeyValueAt(0.5, 1.5);
            scaleAnimation.setEndValue(1.0);
            posAnimation.setDuration(50);
        }

        void moveAnimated(QPointF to)
        {
            posAnimation.setEndValue(to);
            posAnimation.start();
        }

    public slots:
        void runScaleAnimation()
        {
            scaleAnimation.start();
        }
    };

    XHighlight::XHighlight(QGraphicsItem *parent, const float radius): QGraphicsEllipseItem(parent), innerDot(this, radius)
    {
        _radius = radius;

        impl = new Impl(this);

        setBrush(Qt::NoBrush);
        setPen(Qt::NoPen);
        setZValue(99);

        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
        effect->setOffset(lightOffset);
        setGraphicsEffect(effect);

        connect(&impl->hideTimer, SIGNAL(timeout()), this, SLOT(hideTimerCallback()));
        connect(&impl->posAnimation, SIGNAL(finished()), this, SLOT(positionFinished()));

        innerDot.setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        innerDot.setRect(boundingRect());
    }

    void XHighlight::setPosAnimated(const QPointF &thePos)
    {
        setTransformOriginPoint(boundingRect().center());
        innerDot.setTransformOriginPoint(boundingRect().center());

        if (isVisible())
        {
            impl->moveAnimated(thePos);
        }
        else
        {
            setPos(thePos);
            show();
            impl->runScaleAnimation();
        }
    }

    void XHighlight::hideAfter(int msecs)
    {
        impl->hideTimer.stop();
        impl->hideTimer.start(msecs);
    }

    QRectF XHighlight::boundingRect() const
    {
        return QRectF(-_radius/2, -_radius/2, _radius, _radius);
    }

    void XHighlight::setText(const QString &text)
    {
        innerDot.textItem.text = text;
        update();
    }

    void XHighlight::positionFinished()
    {
        impl->runScaleAnimation();
    }

    void XHighlight::hideTimerCallback()
    {
        hide();
    }

    InnerDot::InnerDot(QGraphicsItem *parent, const float radius): QGraphicsEllipseItem(parent), textItem(this), _radius(radius)
    {
        setOptions(options);
    }

    QRectF InnerDot::boundingRect() const
    {
        return QRectF(-_radius/2, -_radius/2, _radius, _radius);
    }

    void InnerDot::setOptions(const InnerDot::Options &opts)
    {
        options = opts;
        textItem.setRect(boundingRect());
        QPointF gradientCenter = boundingRect().center() - lightOffset;
        QRadialGradient gradient(gradientCenter, boundingRect().width());
        gradient.setColorAt(0, options.gradient0);
        gradient.setColorAt(1, options.gradient1);
        setBrush(gradient);
        QPen pen = options.borderColor;
        pen.setWidth(2);
        setPen(pen);
    }

    TextItem::TextItem(QGraphicsItem *parent): QGraphicsRectItem(parent){}

    void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QTextOption textOption = QTextOption(Qt::AlignHCenter | Qt::AlignVCenter);
        painter->setPen(Qt::white);
        painter->drawText(rect(), text, textOption);
    }

}

