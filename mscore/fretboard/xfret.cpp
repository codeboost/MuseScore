#include "xfret.h"
#include <QVector>
#include <QPainter>
#include <QLinearGradient>
#include <QPalette>
#include "xhighlight.h"

namespace vg
{

    XFret::XFret(QGraphicsItem *parent): QGraphicsRectItem(parent) {}

    void XFret::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {

        painter->setRenderHint(QPainter::Antialiasing);

        QLinearGradient gradient(rect().topLeft(), rect().topRight());

        gradient.setColorAt(0, QColor("#333"));
        gradient.setColorAt(0.5, QColor("#eee"));
        gradient.setColorAt(1, QColor("#333"));

        painter->setPen(Qt::NoPen);
        painter->fillRect(rect(), gradient);
    }

    ///

    XNut::XNut(QGraphicsItem *parent): QGraphicsRectItem(parent) {}


    void XNut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->fillRect(rect(), QColor("#fff"));

        QPalette pal;
        QBrush fill = QColor("#eee");
        QRect r = rect().toAlignedRect();
    }

    ///

    XDot::XDot(QGraphicsItem *parent, int n): QGraphicsEllipseItem(parent), dotNumber(n)
    {
        setBrush(QColor("#aaa"));
        setPen(QPen("#fff"));
    }

    ///


}
