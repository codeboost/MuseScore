#include "xhighlight.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>
#include <QEasingCurve>
#include <QDebug>

static QPointF lightOffset = QPoint(1, 1);

namespace vg
{
#if 0
    //Keep this around for a while
    static QTransform GenerateTranslationOnlyTransform(
        const QTransform &original_transform,
        const QPointF &target_point) {
      // To draw the unscaled, we desire a transform with scaling factors
      // of 1 and shearing factors of 0 and the appropriate translation such that
      // our icon center ends up at the same point. According to the
      // documentation, QTransform transforms a point in the plane to another
      // point using the following formulas:
      // x' = m11*x + m21*y + dx
      // y' = m22*y + m12*x + dy
      //
      // For our new transform, m11 and m22 (scaling) are 1, and m21 and m12
      // (shearing) are 0. Since we want x' and y' to be the same, we have the
      // following equations:
      // m11*x + m21*y + dx = x + dx[new]
      // m22*y + m12*x + dy = y + dy[new]
      //
      // Thus,
      // dx[new] = m11*x - x + m21*y + dx
      // dy[new] = m22*y - y + m12*x + dy
      qreal dx = original_transform.m11() * target_point.x()
                 - target_point.x()
                 + original_transform.m21() * target_point.y()
                 + original_transform.m31();
      qreal dy = original_transform.m22() * target_point.y()
                 - target_point.y()
                 + original_transform.m12() * target_point.x()
                 + original_transform.m32();

      return QTransform::fromTranslate(dx, dy);
    }
#endif

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
        Q_UNUSED(option);
        Q_UNUSED(widget);

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

