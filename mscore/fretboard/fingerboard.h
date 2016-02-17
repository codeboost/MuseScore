#ifndef FINGERBOARD_H
#define FINGERBOARD_H

#include <QWidget>
#include "fretboardmodel.h"

namespace vg
{

    class Fingerboard : public QWidget
    {
    public:


        bool fancy = false;
        FretboardModel& model;
        Fingerboard(QWidget* parent, FretboardModel& aModel);

    public:
        QColor stringColor = QColor("#333");
        QColor fretColor = QColor("#555");
        QColor nutColor = QColor("#666");
        QColor bgColor = QColor("#eee");
        QColor dotsColor = QColor("#999");

    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *) override;

        void paintStrings(QPainter& p);
        void paintFrets(QPainter& p);
        void paintWhiteDots(QPainter& p);
        void paintDotForFret(QPainter &painter, int fretNumber, int stringNumber);
        void paintNut(QPainter &painter);
        void paintFret(QPainter &painter, float position);
        void paintNoteNames(QPainter &painter);
        void paintHighlights(QPainter &painter);
        void paintHighlight(QPainter &painter, const FingerHighlight &highlight);
    };
}

#endif // FINGERBOARD_H
