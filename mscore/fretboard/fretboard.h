#ifndef FretboardDotH
#define FretboardDotH

#include "fingerboard.h"

namespace vg
{
    class Fretboard : public QWidget
    {
        Q_OBJECT
    public:
        struct Highlight
        {
            int stringNumber = 0;
            int fretNumber = 0;
            //Maybe something else, like color or note name
            Highlight(int stringN = 0, int fretN = 0): stringNumber(stringN), fretNumber(fretN){}
        };

    public:
        Fingerboard* fingerboard;
        FretboardModel model;
        QVector<Highlight> highlights;
    public:
        Fretboard(QWidget* parent);
        void setOrientation(Qt::Orientation orientation);
    protected:
        void paintEvent(QPaintEvent *) override;
        void paintHighlights(QPainter& p);
        void paintHighlight(QPainter &painter, const Highlight &highlight);
        void resizeEvent(QResizeEvent *) override;
    public slots:
        void onSwapSides();
        void onRotate();
        void increaseStrings();
        void decreaseStrings();
        void increaseFrets();
        void decreaseFrets();
    };
}


#endif
