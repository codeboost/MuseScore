#ifndef XFRETBOARD_H
#define XFRETBOARD_H

#include <QVector>
#include "xhighlight.h"
#include "xstring.h"
#include "xfret.h"

namespace vg
{
    typedef QVector<int> FretHighlights;

    class XFretboard : public QGraphicsRectItem
    {
    public:
        struct Options
        {
            int thickestString = 6;
            int numberOfStrings = 6;
            int numberOfFrets = 19;

            int nutOffset = 2;
            int nutThickness = 16;
            int fretThickness = 4;
            int dotRadius = 5;
            int stringAreaMargin = 5; //Top and bottom spacing before the strings

            QVector<int> dotPositions = {3, 5, 7, 9, 12, 15, 17, 19, 21, 24, 27, 30}; //--> model
            int octaveFret = 12;
        };
        Options options;
        QVector<XString*> strings;
        QVector<XFret*> frets;
        QVector<XDot*> dots;
        XNut* nut;

        void repositionComponents();
        XFretboard(QGraphicsItem* parentItem, const Options& options);
 
        void createFretboardComponents();

        FretHighlights emptyHighlights()
        {
            FretHighlights v(options.numberOfStrings);
            v.fill(-1);
            return v;
        }


        //nFret == 0 -> open string
        //nFret == -1 -> highlight is hidden
        void addHighlight(int nString, int nFret);

        // i -> string number
        //v[i] -> contains fret number; v[i] < 0 ? hide
        void setHighlights(const QVector<int> highlights);

        void hideHighlights();

        //debug
        int selectedString = 1;
        void highlightSelectedAtFret(int nFret)
        {
            addHighlight(selectedString, nFret);
        }

        void pluckSelected(){
            XString* theString = strings[selectedString];
            theString->pluck(0, false);
        }

        QRectF boundingRect() const
        {
            return QRectF(-320, -100, 640, 200);
        }


    private:
        void addDot(int dotn);
        void createDots();
        void createFrets();
        void createStrings();
        void positionFrets();
        void positionDot(XDot* dot, int fretNumber, int stringNumber);
        void positionDots();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QPointF intersectionPoint(int fretNumber, int stringNumber);
        QImage backgroundImage;
    };
}



#endif // XFRETBOARD_H
