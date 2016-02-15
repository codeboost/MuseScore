#ifndef FINGERBOARD_H
#define FINGERBOARD_H

#include <QWidget>
#include "fretboardmodel.h"

namespace vg
{
    //Comment?
    class Fingerboard : public QWidget
    {
    public:
        bool fancy = false;
        FretboardModel& model;
        Fingerboard(QWidget* parent, FretboardModel& aModel);

    public:
        QColor stringColor = Qt::darkGray;
        QColor fretColor = Qt::darkGray;
        QColor nutColor = Qt::black;
        QColor bgColor = Qt::white;
        QColor dotsColor = QColor(180,180,180);

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
