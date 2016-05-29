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

#include "xfretboard.h"
#include <QImage>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <math.h>
#include <QDebug>
#include <QKeyEvent>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QPainter>


namespace vg
{
    static bool texturedBackground = false;

    static bool isOctaveFret(int fretNumber)
    {
        return ((fretNumber % 7) == 0) || ((fretNumber % 12) == 0);
    }

    XFretboard::XFretboard(QGraphicsItem *parentItem, const Options& opt): QGraphicsRectItem(parentItem), options(opt)
    {
        if (texturedBackground)
        {
            backgroundImage.load(":/data/wood-texture.jpg");
            QBrush brush(backgroundImage);
            setBrush(brush);
        }
        else
        {
            QBrush brush(QColor("#222"));
            setBrush(brush);
        }

        setFlag(ItemSendsGeometryChanges, false);
        createFretboardComponents();
    }

    XFretboard::~XFretboard()
    {
        qDebug() << "XFretboard is being destroyed";
    }

    QRectF XFretboard::boundingRect() const
    {
        return QRectF(-320, -100, 640, 200);
    }

    void XFretboard::createFretboardComponents()
    {
        nut = QSharedPointer<QGraphicsRectItem>(new QGraphicsRectItem(this));

        nut->setBrush(QColor("#eee"));
        nut->setRect(boundingRect().left() + options.nutOffset,
                     boundingRect().top(),
                     options.nutThickness,
                     boundingRect().height());

        createFrets();
        createStrings();
        createDots();
    }

    void XFretboard::addHighlight(int nString, int nFret)
    {
        //nFret is 0 ? open string
        //nFret is < 0 ? highlight hidden
        //nFret > 0 ? show highlight on top of string at fret position
        const XString::Ptr& theString = strings[nString];


        if (nFret < 0)
        {
            theString->highlight()->hide();
            return;
        }

        const XFret::Ptr& theFret = frets[nFret];
        float pos = 0;
        if (nFret == 0)
        {
            pos = theFret->pos().x();
        }
        else
        {
            const XFret::Ptr& prevFret = frets[nFret - 1];
            auto delta = theFret->rect().x() - prevFret->rect().x();
            pos = prevFret->rect().x() + delta/2;
        }
        theString->pluck(pos);
    }

    void XFretboard::hideHighlights()
    {
        foreach (const auto& str, strings)
        {
            str->highlight()->hide();
        }
    }


    void XFretboard::setHighlights(const QVector<int> highlights)
    {
        Q_ASSERT(highlights.size() == options.numberOfStrings);

        for (int k = 0; k < options.numberOfStrings; k++)
        {
            addHighlight(k, highlights[k]);
        }
    }

    void XFretboard::addDot(int dotn)
    {
        auto xDot = XDot::Ptr(new XDot(this, dotn));
        dots.push_back(xDot);
        xDot->setRect(0, 0, options.dotRadius * 2, options.dotRadius * 2);
        xDot->setZValue(-99);
    }

    void XFretboard::createDots()
    {
        for (auto dotn : options.dotPositions)
        {
            if (dotn >= options.numberOfFrets)
                break;

            addDot(dotn);

            if (isOctaveFret(dotn))
                addDot(dotn);
        }
        positionDots();
    }

    void XFretboard::createFrets()
    {
        for (int k = 0; k < options.numberOfFrets; k++)
        {
            XFret::Ptr fret = XFret::Ptr(new XFret(this));
            frets.push_back(fret);
        }
        positionFrets();
    }


    void XFretboard::positionFrets()
    {
        QVector<float> positions;
        positions.push_back(0);

        float originx = boundingRect().left() + options.nutOffset + options.nutThickness;

        float length = boundingRect().width();

        for (int n = 0; n < options.numberOfFrets; n++)
        {
            float root2 = pow(1.05946, n + 1);  //magic!
            float pos = length - length / root2;
            positions.push_back(pos);
        }

        //Normalize positions
        float last = positions.back();

        std::transform(positions.begin(), positions.end(), positions.begin(), [last, length](float x){
            return (x / last) * length;
        });

        for (int k = 0; k < options.numberOfFrets; k++)
        {
            const XFret::Ptr& fret = frets[k];
            fret->setRect(positions[k] + originx, boundingRect().top(), options.fretThickness, boundingRect().height());
        }

        //Hide the first fret (next to the nut)
        frets[0]->hide();
    }

    void XFretboard::createStrings()
    {
        const QString noteNames = "EADGBeadgbEADGBeadgb";
        float thickestString = options.thickestString;

        float usableHeight = boundingRect().height() - options.stringAreaMargin * 2;
        float stringAreaHeight = usableHeight / options.numberOfStrings;

        float y = options.stringAreaMargin + boundingRect().top();
        float x = boundingRect().left();

        for (int k = 0; k < options.numberOfStrings; k++)
        {
            QRectF stringRect(x, y, boundingRect().width(), stringAreaHeight);
            float ratio = (float)(k + 1) / (float)options.numberOfStrings;
            qreal thickness = ratio * thickestString;

            auto stringType = k >= 3 ? Golden : Steel;
            XString::Ptr string = XString::Ptr(new XString(stringRect, this, thickness, stringType));
            string->setZValue(2);
            string->setNoteText(noteNames[options.numberOfStrings - k - 1]);
            string->noteNameOffset = (options.nutOffset + nut->rect().width()) / 2;
            y+=stringAreaHeight;
            strings.push_back(string);
        }
    }

    QPointF XFretboard::intersectionPoint(int fretNumber, int stringNumber)
    {
        const XFret::Ptr& fret = frets[fretNumber];
        const XString::Ptr& string = strings[stringNumber];

        QPoint fretOffs(fret->rect().width() / 2, string->rect().height() / 2);

        QPointF fretTopLeft = mapFromItem(fret.data(), fret->rect().topLeft()) + fretOffs;
        QPointF stringTopLeft = mapFromItem(string.data(), string->rect().topLeft()) + fretOffs;

        return QPointF(fretTopLeft.x(), stringTopLeft.y());
    }

    QPointF XFretboard::positionForDot(int fretNumber, int stringNumber)
    {
        if (fretNumber <= 0 || stringNumber <= 0 || stringNumber >= options.numberOfStrings)
        {
            qDebug() << "positionDot: invalid parameters: fret = " << fretNumber << "; string=" << stringNumber;
            return QPointF(0, 0);
        }
        QPointF p1 = intersectionPoint(fretNumber - 1, stringNumber - 1);
        QPointF p2 = intersectionPoint(fretNumber, stringNumber);
        QRectF r(p1, p2);
        return r.center();
    }



    void XFretboard::positionDots()
    {
        int dotIndex = 0;

        for (int k = 0; k < options.dotPositions.size(); k++)
        {
            int fretNumber = options.dotPositions[k];
            if (fretNumber < options.numberOfFrets)
            {
                if (isOctaveFret(fretNumber))
                {
                    int firstDot = options.numberOfStrings / 3;
                    int secondDot = options.numberOfStrings - firstDot;

                    dots[dotIndex++]->setPos(positionForDot(fretNumber, firstDot));
                    dots[dotIndex++]->setPos(positionForDot(fretNumber, secondDot));
                }
                else
                {
                    dots[dotIndex++]->setPos(positionForDot(fretNumber, options.numberOfStrings / 2));
                }
            } else
                break;
        }
    }

    void XFretboard::createFretNumbers()
    {
        if (fretNumbers.size() == 0)
        {
            for (int k = 1; k < options.numberOfFrets; k++)
            {
                auto h = XHighlight::Ptr(new XHighlight(this, 20));

                auto t = QString::number(k, 10);
                h->setText(t);
                h->setRadialColors("#333", "#888");
                fretNumbers.push_back(h);
                QPointF pos = positionForDot(k, options.numberOfStrings / 2);
                h->setPos(pos);
            }
        }
    }

    void XFretboard::setFretNumbersVisible(bool visible)
    {

        createFretNumbers();

        for (auto& h:fretNumbers)
        {
            if (visible)
                h->show();
            else
                h->hide();
        }

        _fretNumbersVisible = visible;
    }



}
