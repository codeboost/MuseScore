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
        connect(&vibrator.timer, SIGNAL(timeout()), this, SLOT(vibrationCallback()));
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

        QRectF stringRect(rect().left(), center.y() - thickness/2.0, rect().width(), thickness);

        float y = center.y();
        QLinearGradient gradient(0, y  - thickness/2.0, 0, y + thickness/2.0);


        QColor color0 = stringColors[stringType][0];
        color0.setAlpha(0);
        gradient.setColorAt(0, color0);
        gradient.setColorAt(0.5, stringColors[stringType][1]);
        gradient.setColorAt(1, stringColors[stringType][0]);

        QPainterPathStroker stroker;
        stroker.setWidth(thickness);

        QPainterPath stroked = stroker.createStroke(path);
        painter->fillPath(stroked, gradient);
    }

    void XString::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
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

        stringPath.moveTo(rect().left(), y);
        stringPath.lineTo(pluckPosition, y);
        float halfLength = pluckPosition + curLength / 2.0f;
        stringPath.quadTo(halfLength , y + vibrator.delta(),length * 2, y);
        return stringPath;
    }

    void XString::pluck(float pos, bool shouldShowHighlight)
    {
        pluckPosition = pos;

        //addBlurEffect();
        if (_vibrate)
            vibrator.startVibrating(10.0);

        if (shouldShowHighlight)
            showHighlight(pos);
    }

    void XString::stopVibrating()
    {
        vibrator.stop();
        setGraphicsEffect(nullptr);
        update();
    }

    void XString::addBlurEffect()
    {
        auto blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        blurEffect->setBlurRadius(1.0);
        setGraphicsEffect(blurEffect);
    }

    void XString::vibrationCallback()
    {
        if (vibrator.tick() < 0.2)
        {
            stopVibrating();
        }
        update();
    }

    //-------------------

    QPointF XString::ptForHighlight(float x)
    {
        QPointF center = boundingRect().center();
        QPointF pt(x, center.y());
        return pt;
    }

    void XString::showHighlight(XHighlight* h, float x, bool animated)
    {
        QPointF pt = ptForHighlight(x);

        if (animated)
        {
            h->setPosAnimated(pt);
        }
        else
        {
            h->setPos(pt);
            h->show();
        }
    }

    void XString::showHighlight(float x)
    {
        showHighlight(highlight(), x, true);
    }

    XHighlight* XString::highlight()
    {
        if (!_highlight)
        {
            _highlight = new XHighlight(this);
            _noteName->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
            _highlight->setRect(_highlight->boundingRect());
            _highlight->setRadialColors("#226FFC", "#8DB5FF");
            _highlight->setBorderColor("#eee");

            QPointF pt = ptForHighlight(0);
            _highlight->setPos(pt);
        }
        return _highlight;
    }

    void XString::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        pluck(event->pos().x());
    }

    void XString::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        highlight()->hideAfter(2000);
    }

    void XString::setNoteText(const QString &noteText)
    {
        if (!_noteName)
        {
            _noteName = new XHighlight(this, 25);
            _noteName->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
            _noteName->setRect(_noteName->boundingRect());

            QPointF pt = boundingRect().center();
            pt.setX(boundingRect().left() + _noteName->boundingRect().width() / 2);
            _noteName->setPos(pt);
            _noteName->setRadialColors("#5A5A85", "#03035C");
            _noteName->setBorderColor("#eee");
        }
        _noteName->setText(noteText);
    }
}
