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

#include "xstringitem.h"
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QGraphicsEffect>

namespace vg
{
    static QColor stringColors[][2] = {
        {"#441401", "#ecb67b"},
        {"#623e5f", "#f6efdf"},
        {"grey", "white"}
    };

    class XStringItem::Impl : public QObject
    {
    public:
        QTimer hideTimer;
        QTimer vibrateTimer;
        bool isUp = false;
        QPointF atPos;
        QRectF vRect;
        XStringItem* _stringItem = nullptr;

        Impl(XStringItem* parent): QObject(parent), _stringItem(parent)
        {
            hideTimer.setSingleShot(true);
            vibrateTimer.setInterval(20);
            connect(&hideTimer, &QTimer::timeout, this, &Impl::onHideTimer);
            connect(&vibrateTimer, &QTimer::timeout, this, &Impl::onVibrate);
        }

        void startVibrating(int duration)
        {
            vRect = _stringItem->vibratingPart.rect();
            _stringItem->vibratingPart.show();
            vibrateTimer.start();
            hideTimer.start(duration);
        }

        void onVibrate()
        {
            qreal delta = isUp ? 2.0f : -2.0f;

            QRectF newRect = vRect;
            newRect.moveTop(newRect.top() + delta);
            _stringItem->vibratingPart.setRect(newRect);
            _stringItem->vibratingPart.update();
            isUp = !isUp;
        }

        void onHideTimer()
        {
            vibrateTimer.stop();
            _stringItem->vibratingPart.hide();
        }
    };

    XStringItem::XStringItem(const QRectF &rect, QGraphicsItem *parent, StringType stringType): QGraphicsRectItem(rect, parent), vibratingPart(rect, this)
    {
        impl = QSharedPointer<Impl>(new Impl(this));
        setPen(Qt::NoPen);

        QLinearGradient gradient(0, rect.top(), 0, rect.bottom());
        QColor color0 = stringColors[stringType][0];
        color0.setAlpha(0);
        gradient.setColorAt(0, color0);
        gradient.setColorAt(0.5, stringColors[stringType][1]);
        gradient.setColorAt(1, stringColors[stringType][0]);

        setBrush(gradient);

        auto *effect = new QGraphicsBlurEffect();
        effect->setBlurRadius(10);
        vibratingPart.setBrush(QColor("#f6efdf"));
        vibratingPart.setGraphicsEffect(effect);
        //vibratingPart.setPen(QPen(QColor("#ff0000")));
        vibratingPart.hide();
    }

    void XStringItem::vibrateAtPos(qreal thePos)
    {
        QRectF vibratingRect = rect();
        vibratingRect.setLeft(thePos);
        vibratingPart.setRect(vibratingRect);
        impl->startVibrating(2000);
    }
}
