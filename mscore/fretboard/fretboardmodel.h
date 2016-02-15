#ifndef FRETBOARDMODEL_H
#define FRETBOARDMODEL_H

#include <QRect>
#include <QVector>

namespace vg
{
    class FretboardModel
    {
    public:
        int numberOfFrets = 16;
        int numberOfStrings = 6;
        int nutThickness = 15;
        int fretThickness = 4;
        int maxStringThickness = 4;

        Qt::Orientation orientation = Qt::Horizontal;

        //AscendingOrder: e string on top (or left) , E string on bottom (or right)
        Qt::SortOrder stringOrder = Qt::AscendingOrder;
    public:
        QRect& rect();
        void setRect(const QRect& r);
        //Returns the rect for the fret and string.
        //Fret and strings indices are 1-based.
        QRect getFretRect(int fretNumber, int stringNumber);
        
        //indices are 0-based
        QPoint intersectionPoint(int fretNumber, int stringNumber);

        float widthForString(int stringIndex);
        void update();


        float posForString(int i)
        {
            Q_ASSERT(i>=0 && i < numberOfStrings);

            if (stringOrder == Qt::DescendingOrder)
            {
                return stringPositions[numberOfStrings - i - 1];
            }

            return stringPositions[i];
        }

        float posForFret(int i)
        {
            Q_ASSERT(i>=0 && i < numberOfFrets);
            return fretPositions[i];
        }

    protected:
        QRect fretboardRect;
        QVector<float> fretPositions;
        QVector<float> stringPositions;

        void updateFretPositions();
        void updateStringPositions();
    };
}

#endif // FRETBOARDMODEL_H
