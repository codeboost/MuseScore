#ifndef XFRET_H
#define XFRET_H

#include <QGraphicsItem>
#include "xhighlight.h"

namespace vg
{
    class XFret : public QGraphicsRectItem
    {
    public:
        typedef QSharedPointer<XFret> Ptr;
        XFret(QGraphicsItem* parent);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    };

    //A dot in the center of fret 5, 7, 9...
    class XDot : public XHighlight
    {
    public:
        typedef QSharedPointer<XDot> Ptr;
        int dotNumber = 0;
        XDot(QGraphicsItem* parent, int n);
    };
}
#endif // XFRET_H
