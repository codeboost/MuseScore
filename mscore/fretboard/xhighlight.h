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
            QColor borderColor = QColor("#eee");
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
        XHighlight(QGraphicsItem* parent, const float radius = 30.0f);
        void setPosAnimated(const QPointF &pos);
        void hideAfter(int msecs);
        QRectF boundingRect() const;
        void setText(const QString& text);

    protected slots:
        void positionFinished();
        void hideTimerCallback();

    private:
        class Impl;
        Impl* impl;
        float _radius = 30.0f;
    };
}

#endif // XHIGHLIGHT_H
