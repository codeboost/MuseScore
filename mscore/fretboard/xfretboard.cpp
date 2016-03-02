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
    XFretboard::XFretboard(QGraphicsItem *parentItem): QGraphicsRectItem(parentItem)
    {
        createFretboardComponents();
        backgroundImage.load(":/wood-texture.jpg");
    }

    void XFretboard::createFretboardComponents()
    {
        nut = new XNut(this);

        createDots();
        createFrets();
        createStrings();
    }

    void XFretboard::addDot(int dotn)
    {
        auto xDot = new XDot(this, dotn);
        dots.push_back(xDot);
        xDot->setRect(0, 0, options.dotRadius * 2, options.dotRadius * 2);
    }

    void XFretboard::createDots()
    {
        for (auto dotn : options.dotPositions)
        {
            if (dotn >= options.numberOfFrets)
                break;

            addDot(dotn);

            if ((dotn % options.octaveFret) == 0)
                addDot(dotn);
        }
    }

    void XFretboard::createFrets()
    {
        for (int k = 0; k < options.numberOfFrets; k++)
        {
            XFret* fret = new XFret(this);
            frets.push_back(fret);
        }
    }

    void XFretboard::createStrings()
    {
        const QString notes = "EADGBeadgbEADGBeadgb";

        for (int k = 0; k < options.numberOfStrings; k++)
        {
            XString* string = new XString(this);
            float ratio = (float)(k + 1) / (float)options.numberOfStrings;
            string->thickness = ratio * options.thickestString;
            string->setNoteText(notes[options.numberOfStrings - k - 1]);
            strings.push_back(string);
        }
    }

    void XFretboard::repositionComponents()
    {
        nut->setRect(options.nutOffset, 0, options.nutThickness, rect().height());
        positionFrets();
        positionStrings();
        positionDots();
    }

    void XFretboard::positionFrets()
    {
        QVector<float> positions;
        positions.push_back(0);

        float originx = options.nutOffset + options.nutThickness;

        float length = rect().width();
        length -= originx;

        for (int n = 1; n <= options.numberOfFrets; n++)
        {
            float root2 = pow(1.05946, n);
            float pos = length - length / root2;
            positions.push_back(pos);
        }

        //Normalize positions
        float last = positions.back();

        std::transform(positions.begin(), positions.end(), positions.begin(), [last, length](float x){
            return (x / last) * length;
        });

        for (int k = 1; k < options.numberOfFrets; k++)
        {
            XFret* fret = frets[k];
            fret->setRect(positions[k] + originx, 0, options.fretThickness, rect().height());
        }
    }

    void XFretboard::positionStrings()
    {
        float usableHeight = rect().height() - options.stringAreaMargin * 2;
        float stringAreaHeight = usableHeight / options.numberOfStrings;
        float y = options.stringAreaMargin;

        for (auto& str : strings)
        {
            str->setRect(0, y, rect().width(), stringAreaHeight);
            y+=stringAreaHeight;
        }
    }

    void XFretboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        //painter->fillRect(rect(), QColor("#551D00"));

        painter->drawImage(rect(), backgroundImage);
    }

    QPointF XFretboard::intersectionPoint(int fretNumber, int stringNumber)
    {
        XFret* fret = frets[fretNumber];
        XString* string = strings[stringNumber];

        QPoint fretOffs(fret->rect().width() / 2, string->rect().height() / 2);

        QPointF fretTopLeft = mapFromItem(fret, fret->rect().topLeft()) + fretOffs;
        QPointF stringTopLeft = mapFromItem(string, string->rect().topLeft()) + fretOffs;

        return QPointF(fretTopLeft.x(), stringTopLeft.y());
    }

    void XFretboard::positionDot(XDot *dot, int fretNumber, int stringNumber)
    {
        QPointF p1 = intersectionPoint(fretNumber - 1, stringNumber - 1);
        QPointF p2 = intersectionPoint(fretNumber, stringNumber);
        QRectF r(p1, p2);
        QPointF fretCenter = r.center() - QPointF(dot->rect().width() / 2, dot->rect().height() / 2);
        dot->setPos(fretCenter);
    }

    void XFretboard::positionDots()
    {
        int dotIndex = 0;

        for (int k = 0; k < options.dotPositions.size(); k++)
        {
            int fretNumber = options.dotPositions[k];
            if (fretNumber < options.numberOfFrets)
            {
                if ((fretNumber % options.octaveFret) == 0)
                {
                    int firstDot = options.numberOfStrings / 3;
                    int secondDot = options.numberOfStrings - firstDot;
                    positionDot(dots[dotIndex++], fretNumber, firstDot);
                    positionDot(dots[dotIndex++], fretNumber, secondDot);
                }
                else
                {
                    positionDot(dots[dotIndex++], fretNumber, options.numberOfStrings / 2);
                }
            } else
                break;
        }
    }

}
