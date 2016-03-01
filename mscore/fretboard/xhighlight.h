#ifndef XHIGHLIGHT_H
#define XHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

class XHighlight : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

    QPropertyAnimation* hideAnimation;
    QPropertyAnimation* scaleAnimation;
public:
    XHighlight(QGraphicsItem* parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
public slots:
    void hideAnimated();
    void hideAfter(int msecs);
    void showAnimated();
};



#endif // XHIGHLIGHT_H
