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
    XFretboard::XFretboard(QGraphicsItem *parentItem, const Options& opt): QGraphicsRectItem(parentItem), options(opt)
    {
        backgroundImage.load(":/data/wood-texture.jpg");
        setFlag(ItemSendsGeometryChanges, false);
    }

    void XFretboard::createFretboardComponents()
    {
        nut = new XNut(this);
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
        XString* theString = strings[nString];


        if (nFret < 0)
        {
            theString->highlight()->hide();
            return;
        }

        XFret* theFret = frets[nFret];
        float pos = 0;


        if (nFret == 0)
        {
            pos = theFret->pos().x();
        }
        else
        {
            XFret* prevFret = frets[nFret - 1]; 
            auto delta = theFret->rect().x() - prevFret->rect().x();
            pos = prevFret->rect().x() + delta/2;
        }
        theString->pluck(pos);
    }

    void XFretboard::hideHighlights()
    {
        foreach (XString* str, strings)
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
        auto xDot = new XDot(this, dotn);
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

            if ((dotn % options.octaveFret) == 0)
                addDot(dotn);
        }
        positionDots();
    }

    void XFretboard::createFrets()
    {
        for (int k = 0; k < options.numberOfFrets; k++)
        {
            XFret* fret = new XFret(this);
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

        for (int k = 1; k < options.numberOfFrets; k++)
        {
            XFret* fret = frets[k];
            fret->setRect(positions[k] + originx, boundingRect().top(), options.fretThickness, boundingRect().height());
        }
    }

    void XFretboard::createStrings()
    {
        const QString notes = "EADGBeadgbEADGBeadgb";
        float thickestString = options.thickestString;

        float usableHeight = boundingRect().height() - options.stringAreaMargin * 2;
        float stringAreaHeight = usableHeight / options.numberOfStrings;

        float y = options.stringAreaMargin + boundingRect().top();
        float x = boundingRect().left();

        for (int k = 0; k < options.numberOfStrings; k++)
        {
            XString* string = new XString(this);
            string->setRect(x, y, boundingRect().width(), stringAreaHeight);
            float ratio = (float)(k + 1) / (float)options.numberOfStrings;
            string->thickness = ratio * thickestString;
            string->setNoteText(notes[options.numberOfStrings - k - 1]);
            y+=stringAreaHeight;
            strings.push_back(string);
        }
    }

    void XFretboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        painter->drawImage(boundingRect(), backgroundImage);
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
        if (fretNumber <= 0 || stringNumber <= 0 || stringNumber >= options.numberOfStrings)
        {
            qDebug() << "positionDot: invalid parameters: fret = " << fretNumber << "; string=" << stringNumber;
            return;
        }

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
