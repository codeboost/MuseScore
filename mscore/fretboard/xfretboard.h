#ifndef XFRETBOARD_H
#define XFRETBOARD_H

#include <QPen>
#include <QBrush>
#include <QVector>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <math.h>
#include <QDebug>
#include <QKeyEvent>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QPainter>
#include "xhighlight.h"
#include "xstring.h"

namespace vg
{
    class XNut : public QGraphicsRectItem
    {
    public:
        XNut(QGraphicsItem* parent): QGraphicsRectItem(parent) {}

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
        {
            painter->fillRect(rect(), QColor("#fff"));
        }
    };

    class XFret : public QGraphicsRectItem
    {
    public:
        XFret(QGraphicsItem* parent): QGraphicsRectItem(parent) {}

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
        {

            painter->setRenderHint(QPainter::Antialiasing);

            QLinearGradient gradient(rect().topLeft(), rect().topRight());

            gradient.setColorAt(0, QColor("#333"));
            gradient.setColorAt(0.5, QColor("#eee"));
            gradient.setColorAt(1, QColor("#333"));

            painter->setPen(Qt::NoPen);
            painter->fillRect(rect(), gradient);
        }

    };

    class XDot : public QGraphicsEllipseItem
    {
    public:
        int dotNumber = 0;
        XDot(QGraphicsItem* parent, int n): QGraphicsEllipseItem(parent), dotNumber(n)
        {
            setBrush(QColor("#aaa"));
            setPen(QPen("#fff"));
        }
    };


    class XFretboard : public QGraphicsRectItem
    {
    public:
        struct Options
        {
            int numberOfStrings = 6;
            int numberOfFrets = 19;
            int thickestString = 6;   //The thickness of the thickest string :D

            int nutOffset = 8;
            int nutThickness = 13;
            int fretThickness = 6;
            int dotRadius = 7;
            int stringAreaMargin = 10; //Top and bottom spacing before the strings

            QVector<int> dotPositions = {3, 5, 7, 9, 12, 15, 17, 19, 21, 24, 27, 30}; //--> model
            int octaveFret = 12; //---> model
        };
        Options options;
        QVector<XString*> strings;
        QVector<XFret*> frets;
        QVector<XDot*> dots;
        XNut* nut;

        void repositionComponents();
        XFretboard(QGraphicsItem* parentItem);

        void createFretboardComponents();

    private:
        void addDot(int dotn);
        void createDots();
        void createFrets();
        void createStrings();
        void positionFrets();
        void positionStrings();

        void putDot(XDot* dot, int fretNumber, int stringNumber);
        void positionDots();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QPointF intersectionPoint(int fretNumber, int stringNumber);

    };
}



#endif // XFRETBOARD_H
