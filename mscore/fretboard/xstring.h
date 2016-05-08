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

#ifndef XSTRING_H
#define XSTRING_H


#include <QGraphicsEffect>
#include "xhighlight.h"
#include "xstringitem.h"

namespace vg
{
    class XString : public QObject, public QGraphicsRectItem
    {
        Q_OBJECT
    public:
        int highlightSize = 30;
        const int noteNameSize = 30;
        //The horizontal (x) position where to place the noteName item
        float noteNameOffset = 9.0f;

    public:
        typedef QSharedPointer<XString> Ptr;
        XString(const QRectF &rect, QGraphicsItem* parent, qreal theThickness, StringType stringType);
        void pluck(float position, bool shouldShowHighlight = true);
        void showHighlight(float position);
        void setNoteText(const QString& noteText);

        XHighlight::Ptr highlight();
        XHighlight::Ptr _highlight;
        XHighlight::Ptr _noteName;
        XStringItem::Ptr stringItem;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    };
}
#endif // XSTRING_H
