#ifndef XHIGHLIGHT_H
#define XHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

namespace vg
{
    class XHighlight : public QObject, public QGraphicsEllipseItem
    {
        Q_OBJECT
        Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
        Q_PROPERTY(qreal scale READ scale WRITE setScale)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        struct Options
        {
            int strokeSize = 4;
            QColor strokeColor = QColor("#eee");
            QColor gradient0 = QColor("#226FFC");
            QColor gradient1 = QColor("#8DB5FF");
            QString text = "";
        };
        Options options;
        //
        XHighlight(QGraphicsItem* parent);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        void setPosAnimated(const QPointF &pos);
        void hideAfter(int msecs);
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
