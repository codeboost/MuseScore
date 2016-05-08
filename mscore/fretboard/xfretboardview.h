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

#ifndef XFRETBOARDVIEW_H
#define XFRETBOARDVIEW_H

#include <QGraphicsView>
#include "fretboard/xfretboard.h"
#include <QGraphicsTransform>

namespace vg
{
    class XFretboardView : public QGraphicsView
    {
    public:
        typedef QSharedPointer<XFretboardView> Ptr;
        vg::XFretboard::Ptr fretboard;
        Qt::Orientation orientation = Qt::Horizontal;
    public:
        explicit XFretboardView(QWidget *parent = 0);
        void setFretboardOptions(const vg::XFretboard::Options& options);
        void flipStrings();
        void mirrorSides();
        void toggleOrientation();
        void toggleFretNumbers();

        void setOrientation(Qt::Orientation orientation);
    protected:
        QTransform mirrorAndFlip;
        void resizeEvent(QResizeEvent *event) override;

    };
}
#endif // XFRETBOARDVIEW_H
