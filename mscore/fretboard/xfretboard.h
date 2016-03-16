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
            int numberOfStrings = 6;
            int numberOfFrets = 19;
            //int thickestString = 6;   //The thickness of the thickest string :D

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


        QVariant itemChange(GraphicsItemChange change, const QVariant &value);


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
