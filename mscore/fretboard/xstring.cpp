#include "xstring.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainterPathStroker>

namespace vg
{
    static QColor stringColors[][2] = {
        {"#441401", "#ecb67b"},
        {"#623e5f", "#f6efdf"},
        {"grey", "white"}
    };

    XString::XString(QGraphicsItem *parent): QGraphicsRectItem(parent)
    {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemClipsToShape, true);
    }

    void XString::paintLineString(QPainter *painter)
    {
        QPen pen = QColor("#aaa");
        pen.setWidth(thickness);
        painter->setPen(pen);

        QPointF center = rect().center();
        painter->drawLine(QPointF(0, center.y()), QPointF(rect().width(), center.y()));
    }


    void XString::fastPaint(QPainter* painter)
    {
        QPen pen = QColor("#ecb67b");
        pen.setWidth(thickness);
        QPainterPath path = generatePath();
        painter->strokePath(path, pen);
    }

    void XString::gradientPaint(QPainter* painter)
    {
        QPen pen = QColor("#ecb67b");
        pen.setWidth(thickness);
        QPainterPath path = generatePath();

        auto center = rect().center();

        QRectF stringRect(0, center.y() - thickness/2.0, rect().width(), thickness);

        QLinearGradient gradient(center.x(), center.y() - thickness/2.0, center.x(), center.y() + thickness/2.0);


//        QColor *colors = stringColors[stringType];

        gradient.setColorAt(0, stringColors[stringType][0]);
        gradient.setColorAt(0.5, stringColors[stringType][1]);
        gradient.setColorAt(1, stringColors[stringType][0]);

//        gradient.setColorAt(0, QColor("#441401"));
//        gradient.setColorAt(0.5, QColor("#ecb67b"));
//        gradient.setColorAt(1, QColor("#441401"));

        QPainterPathStroker stroker;
        stroker.setWidth(thickness);

        QPainterPath stroked = stroker.createStroke(path);
        painter->fillPath(stroked, gradient);
    }

    void XString::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        if (_fastPaint)
        {
            fastPaint(painter);
        }
        else
        {
            gradientPaint(painter);
        }
    }

    QPainterPath XString::generatePath()
    {
        const float y = rect().center().y();
        float length = rect().width();
        float curLength = length - pluckPosition;

        QPainterPath stringPath;

        stringPath.moveTo(0, y);
        stringPath.lineTo(pluckPosition, y);
        float halfLength = pluckPosition + curLength / 2.0f;
        stringPath.quadTo(halfLength , y + (sin (phase) * amplitude),length * 2, y);

        return stringPath;
    }

    void XString::pluck(float pos)
    {
        //amplitude = maxAmplitude * sin(relativePosition * M_PI);
        amplitude = maxAmplitude;
        phase = M_PI / 2.0f;
        pluckPosition = pos;
        qDebug() << "Plucked: " << pluckPosition;

        //addBlurEffect();
        startVibrationTimer();
    }

    void XString::stopVibrating()
    {
        qDebug() << "Stop vibrating";
        vibrationTimer->stop();
        setGraphicsEffect(nullptr);
        update();
    }

    void XString::updateAmplitude()
    {
        // this determines the decay of the visible string vibration
        float decayPerFrame = fps * vibrationDurationMS / 1000.0;

        //Reduce amplitude by a small amount each frame until we reach almost zero
        amplitude -= amplitude/decayPerFrame;

        if (amplitude <= 0.2f)
        {
            stopVibrating();
        }
    }

    void XString::startVibrationTimer()
    {
        if (vibrationTimer == nullptr)
        {
            vibrationTimer = new QTimer(this);
            connect(vibrationTimer, SIGNAL(timeout()), this, SLOT(timerCallback()));
        }

        if (!vibrationTimer->isActive())
            vibrationTimer->start(1000/fps);
    }

    void XString::addBlurEffect()
    {
        auto blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        blurEffect->setBlurRadius(1.0);
        setGraphicsEffect(blurEffect);
    }

    void XString::updatePhase()
    {
        // this determines the visible vibration frequency.
        // just an arbitrary number chosen to look OK:
        //const float phaseStep = 1200.0f / (rect().width() );

        const float phaseStep = M_PI / 2;

        phase += phaseStep;

        if (phase > M_PI)
            phase -= 2.0f * M_PI;
    }

    void XString::timerCallback()
    {
        updateAmplitude();
        updatePhase();
        update();
    }

    XHighlight* XString::highlight()
    {
        if (!_highlight)
        {
            _highlight = new XHighlight(this);
            _highlight->setOpacity(0);
            _highlight->setRect(0, 0, dotSize, dotSize);
        }
        return _highlight;
    }

    void XString::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        QPointF pt = event->pos() - QPointF(dotSize/2, dotSize/2);

        highlight()->setPos(pt);
        highlight()->showAnimated();
        pluck(pt.x());
    }

    void XString::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        highlight()->hideAfter(2000);
    }

}
