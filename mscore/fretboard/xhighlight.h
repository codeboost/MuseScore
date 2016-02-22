#ifndef XHIGHLIGHT_H
#define XHIGHLIGHT_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialog>
#include <QVBoxLayout>
#include <QDebug>
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
public slots:
    void hideAnimated();
    void hideAfter(int msecs);
    void showAnimated();
};



#endif // XHIGHLIGHT_H
