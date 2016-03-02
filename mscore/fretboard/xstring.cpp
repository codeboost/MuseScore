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

        QRectF stringRect(0, center.y() - thickness/2.0, rect().width(), thickness);

        float y = center.y();
        QLinearGradient gradient(0, y  - thickness/2.0, 0, y + thickness/2.0);

        gradient.setColorAt(0, stringColors[stringType][0]);
        gradient.setColorAt(0.5, stringColors[stringType][1]);
        gradient.setColorAt(1, stringColors[stringType][0]);

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
        stringPath.quadTo(halfLength , y + vibrator.delta(),length * 2, y);
        return stringPath;
    }

    void XString::pluck(float pos)
    {
        pluckPosition = pos;

        //addBlurEffect();
        if (_vibrate)
            vibrator.startVibrating(7.0);

        showHighlight(pos);
    }

    void XString::showHighlight(float x)
    {
        QPointF pt(x, rect().center().y());
        pt -= QPointF(highlightSize/2, highlightSize/2);
        highlight()->setPos(pt);
        highlight()->showAnimated();
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

    XHighlight* XString::highlight()
    {
        if (!_highlight)
        {
            _highlight = new XHighlight(this);
            _highlight->setRect(0, 0, highlightSize, highlightSize);
        }
        return _highlight;
    }

    void XString::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        pluck(event->pos().x());
    }

    void XString::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        highlight()->hideAfter(2000);
    }

    QVariant XString::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemVisibleChange)
        {
            reposition();
        }
        return QGraphicsItem::itemChange(change, value);
    }

    void XString::reposition()
    {
        qDebug() << "Reposition";
        QPointF center = rect().center();
        QPointF pt(0, rect().center().y() - noteNameSize / 2);
        _noteName->setPos(pt);
    }

    void XString::setNoteText(const QString &noteText)
    {
        _noteName = new XHighlight(this);
        _noteName->setRect(0, 0, noteNameSize, noteNameSize);
        _noteName->options.text = noteText;
        _noteName->options.gradient0 = "#5A5A85";
        _noteName->options.gradient1 = "#03035C";
    }
}
