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
        QPropertyAnimation* scaleAnimation;
        QPropertyAnimation* hideAnimation;
    public:
        struct Options
        {
            int strokeSize = 4;
            QColor strokeColor = QColor("#eee");
            QColor gradient0 = QColor("#226FFC");
            QColor gradient1 = QColor("#8DB5FF");
            QString text = "E#";
        };
        XHighlight(QGraphicsItem* parent);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        Options options;

    public slots:
        void hideAnimated();
        void hideAfter(int msecs);
        void showAnimated();
    private:
        class Impl;
        Impl* impl;
    };
}

#endif // XHIGHLIGHT_H
