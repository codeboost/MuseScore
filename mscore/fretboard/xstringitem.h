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

#ifndef XSTRINGITEM_H
#define XSTRINGITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QTimer>
namespace vg
{
    enum StringType
    {
        Golden = 0, // ?
        Steel,
        Nylon
    };

    class XStringItem : public QObject, public QGraphicsRectItem
    {
        Q_OBJECT
    public:
        typedef QSharedPointer<XStringItem> Ptr;
        QGraphicsRectItem vibratingPart;
        XStringItem(const QRectF& rect, QGraphicsItem* parent, StringType stringType);
        void vibrateAtPos(qreal thePos);
    private:
        class Impl;
        friend class Impl;
        QSharedPointer<Impl> impl;
    };
}

#endif // XSTRINGITEM_H
