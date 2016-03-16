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

        Impl(XHighlight* parent): QObject(parent), scaleAnimation(parent, "scale"), posAnimation(parent, "pos")
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

    XHighlight::XHighlight(QGraphicsItem *parent): QGraphicsEllipseItem(parent)
    {
        impl = new Impl(this);

        setBrush(QColor("#387EFF"));
        setPen(QColor("#ddd"));

        setFlag(QGraphicsItem::ItemIgnoresTransformations, true);


        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
        effect->setOffset(lightOffset);
        setGraphicsEffect(effect);

        connect(&impl->hideTimer, SIGNAL(timeout()), this, SLOT(hideTimerCallback()));
        connect(&impl->posAnimation, SIGNAL(finished()), this, SLOT(positionFinished()));


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

    void XHighlight::setPosAnimated(const QPointF &thePos)
    {
        setTransformOriginPoint(rect().width() / 2, rect().height() / 2);

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

    void XHighlight::positionFinished()
    {
        impl->runScaleAnimation();
    }
}

