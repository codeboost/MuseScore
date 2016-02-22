#include "xstring.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

namespace vg
{

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

    void XString::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QPen pen = QColor("#aaa");
        pen.setWidth(thickness);
        QPainterPath path = generatePath();
        painter->strokePath(path, pen);
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
        startVibrationTimer();
    }

    void XString::stopVibrating()
    {
        qDebug() << "Stop vibrating";
        vibrationTimer->stop();
        update();
    }

    void XString::updateAmplitude()
    {
        // this determines the decay of the visible string vibration.
        float decayPerFrame = fps * vibrationDurationMS / 1000.0;

    //        float mult = (1.0f - 1.0f / (maxAmplitude * 30.0f));
    //        amplitude *= mult;

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

    void XString::updatePhase()
    {
        // this determines the visible vibration frequency.
        // just an arbitrary number chosen to look OK:
        const float phaseStep = 800.0f / (rect().width() );

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
