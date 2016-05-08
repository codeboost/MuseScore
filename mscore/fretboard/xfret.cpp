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
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing);

        QLinearGradient gradient(rect().topLeft(), rect().topRight());

        gradient.setColorAt(0, QColor("#333"));
        gradient.setColorAt(0.5, QColor("#eee"));
        gradient.setColorAt(1, QColor("#333"));

        painter->setPen(Qt::NoPen);
        painter->fillRect(rect(), gradient);
    }

    XDot::XDot(QGraphicsItem *parent, int n): XHighlight(parent, 15), dotNumber(n)
    {
        setRadialColors("#999", "#eee");
        setBorderColor("#eee");
    }


}
