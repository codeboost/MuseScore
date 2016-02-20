#ifndef FretboardDotH
#define FretboardDotH

#include "fingerboard.h"

namespace vg
{
    class Fretboard : public QWidget
    {
        Q_OBJECT
    public:

    public:
        Fingerboard* fingerboard;
        FretboardModel model;
        QTimer* repaintTimer;
    public:
        Fretboard(QWidget* parent);
        void setOrientation(Qt::Orientation orientation);
    protected:
        void paintEvent(QPaintEvent *) override;
        void resizeEvent(QResizeEvent *) override;
        void paintNoteNames(QPainter &painter);
    public slots:
        void onSwapSides();
        void onRotate();
        void increaseStrings();
        void decreaseStrings();
        void increaseFrets();
        void decreaseFrets();
        void repaintTimerCallback();
    };
}


#endif
