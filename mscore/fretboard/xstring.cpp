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

#include "xstring.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainterPathStroker>

namespace vg
{

    XString::XString(const QRectF &rect, QGraphicsItem *parent, qreal thickness, StringType stringType): QGraphicsRectItem(rect, parent)
    {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemClipsToShape, true);
        setPen(Qt::NoPen);
        setBrush(Qt::NoBrush);

        qreal delta = (rect.height() - thickness) / 2;
        QRectF actualRect = rect.adjusted(0, delta, 0, -delta);
        stringItem = XStringItem::Ptr(new XStringItem(actualRect, this, stringType));

    }


    void XString::pluck(float pos, bool shouldShowHighlight)
    {
        if (shouldShowHighlight)
            showHighlight(pos);

        stringItem->vibrateAtPos(pos);
    }

    void XString::showHighlight(float x)
    {
        QPointF pt (x, rect().center().y());
        highlight()->setPosAnimated(pt, false);
    }

    XHighlight::Ptr XString::highlight()
    {
        if (!_highlight.data())
        {
            _highlight = XHighlight::Ptr(new XHighlight(this));
            _highlight->setRect(_highlight->boundingRect());
            _highlight->setRadialColors("#226FFC", "#8DB5FF");
            _highlight->setBorderColor("#eee");
            QPointF pt = QPointF(0, rect().center().y());
            _highlight->setPos(pt);
        }
        return _highlight;
    }

    void XString::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        pluck(event->pos().x());
    }

    void XString::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        highlight()->hideAfter(2000);
    }

    void XString::setNoteText(const QString &noteText)
    {
        if (!_noteName.data())
        {
            _noteName = XHighlight::Ptr(new XHighlight(this, 25));
            _noteName->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);

            QPointF pt = boundingRect().center();
            pt.setX(boundingRect().left() + noteNameOffset);

            _noteName->setPos(pt);
            _noteName->setRadialColors("#5A5A85", "#03035C");
            _noteName->setBorderColor("#eee");
        }
        _noteName->setText(noteText);
    }
}
