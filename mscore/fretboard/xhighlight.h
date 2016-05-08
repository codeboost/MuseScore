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

#ifndef XHIGHLIGHT_H
#define XHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>

namespace vg
{
    class TextItem : public QGraphicsRectItem
    {
    public:
        TextItem(QGraphicsItem* parent);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString text;
    };

    class InnerDot : public QObject, public QGraphicsEllipseItem
    {
        Q_OBJECT
        Q_PROPERTY(qreal scale READ scale WRITE setScale)
    public:
        TextItem textItem;
        InnerDot(QGraphicsItem* parent, const float radius = 25.0f);
        QRectF boundingRect() const;
        float _radius = 25.0f;
    };

    class XHighlight : public QObject, public QGraphicsEllipseItem
    {
        Q_OBJECT
        Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
        Q_PROPERTY(qreal scale READ scale WRITE setScale)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        typedef QSharedPointer<XHighlight> Ptr;
        InnerDot innerDot;
        XHighlight(QGraphicsItem* parent, const float radius = 25.0f);
        void setPosAnimated(const QPointF &pos, bool bAnimated = true);
        void hideAfter(int msecs);
        QRectF boundingRect() const;
        void setText(const QString& text);

        void setRadialColors(const QColor& color1, const QColor& color2);
        void setBorderColor(const QColor& color);

    protected slots:
        void positionFinished();
        void hideTimerCallback();

    private:
        class Impl;
        QSharedPointer<Impl> impl;
        float _radius = 25.0f;
    };
}

#endif // XHIGHLIGHT_H
