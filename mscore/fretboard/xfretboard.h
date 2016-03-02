#ifndef XFRETBOARD_H
#define XFRETBOARD_H

#include <QVector>
#include "xhighlight.h"
#include "xstring.h"
#include "xfret.h"

namespace vg
{
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
        void positionDot(XDot* dot, int fretNumber, int stringNumber);
        void positionDots();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QPointF intersectionPoint(int fretNumber, int stringNumber);
        QImage backgroundImage;

    };
}



#endif // XFRETBOARD_H
