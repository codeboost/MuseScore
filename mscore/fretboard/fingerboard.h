#ifndef FINGERBOARD_H
#define FINGERBOARD_H

#include <QWidget>
#include "fretboardmodel.h"

namespace vg
{
    class Fingerboard : public QWidget
    {
    public:
        FretboardModel& model;
        Fingerboard(QWidget* parent, FretboardModel& aModel);
    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *) override;

        void paintStrings(QPainter& p);
        void paintFrets(QPainter& p);
        void paintWhiteDots(QPainter& p);
        void paintDotForFret(QPainter &painter, int fretNumber, int stringNumber);
        void paintNut(QPainter &painter);
        void paintFret(QPainter &painter, float position);
    };
}

#endif // FINGERBOARD_H
