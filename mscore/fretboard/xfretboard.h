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
        typedef QSharedPointer<XFretboard> Ptr;
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
        };
        Options options;

        XFretboard(QGraphicsItem* parentItem, const Options& options);
        ~XFretboard();

        //nFret == 0 -> open string
        //nFret == -1 -> highlight is hidden
        void addHighlight(int nString, int nFret);

        // i -> string number
        //v[i] -> contains fret number; v[i] < 0 ? hide
        void setHighlights(const QVector<int> highlights);

        void hideHighlights();

        QRectF boundingRect() const;

        bool _fretNumbersVisible = false;
        void createFretNumbers();
        void setFretNumbersVisible(bool visible);

    private:
        void createFretboardComponents();
        void addDot(int dotn);
        void createDots();
        void createFrets();
        void createStrings();
        void positionFrets();
        void positionDots();
        QPointF intersectionPoint(int fretNumber, int stringNumber);
        QPointF positionForDot(int fretNumber, int stringNumber);
        QImage backgroundImage;
    private:
        QVector<XString::Ptr> strings;
        QVector<XFret::Ptr> frets;
        QVector<XDot::Ptr> dots;
        QVector<XHighlight::Ptr> fretNumbers;
        QSharedPointer<QGraphicsRectItem> nut;
    };
}



#endif // XFRETBOARD_H
