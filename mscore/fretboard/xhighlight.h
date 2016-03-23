#ifndef XHIGHLIGHT_H
#define XHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

namespace vg
{

    class TextItem : public QGraphicsRectItem
    {
    public:
        TextItem(QGraphicsItem* parent);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString text;
    };

    class InnerDot : public QObject, public QGraphicsEllipseItem
    {
        Q_OBJECT
        Q_PROPERTY(qreal scale READ scale WRITE setScale)
    public:
        TextItem textItem;
        InnerDot(QGraphicsItem* parent, const float radius = 30.0f);
        QRectF boundingRect() const;

        struct Options
        {
            QColor gradient0 = QColor("#226FFC");
            QColor gradient1 = QColor("#8DB5FF");
        };
        Options options;
        void setOptions(const Options& opts);
        float _radius = 30.0f;
    };

    class XHighlight : public QObject, public QGraphicsEllipseItem
    {
        Q_OBJECT
        Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
        Q_PROPERTY(qreal scale READ scale WRITE setScale)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        InnerDot innerDot;
        float _radius = 30.0f;

        XHighlight(QGraphicsItem* parent, const float radius = 30.0f);
        void setPosAnimated(const QPointF &pos);
        void hideAfter(int msecs);

        QRectF boundingRect() const
        {
           return QRectF(-_radius/2, -_radius/2, _radius, _radius);
        }

        void setText(const QString& text)
        {
            innerDot.textItem.text = text;
            update();
        }

    protected slots:
        void positionFinished();
        void hideTimerCallback()
        {
            hide();
        }

    private:
        class Impl;
        Impl* impl;
    };
}

#endif // XHIGHLIGHT_H
