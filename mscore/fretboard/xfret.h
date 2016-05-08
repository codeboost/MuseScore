//=============================================================================
//  Virtual Guitar Player
//  Copyright (C) 2016 Larry Jones
//  Developed by Florin Braghis
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

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
