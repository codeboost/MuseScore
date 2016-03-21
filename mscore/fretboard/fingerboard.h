#ifndef FINGERBOARD_H
#define FINGERBOARD_H

#include <QWidget>
#include "fretboardmodel.h"
#include <QPainter>

namespace vg
{

    class HighlightWidget : public QWidget
    {
    public:
        HighlightWidget(QWidget* parent) : QWidget(parent)
        {

        }

        void paintEvent(QPaintEvent* e) override
        {
            Q_UNUSED(e);

            QPainter painter(this);
            QBrush brush(Qt::blue);
            QPen pen(Qt::white);
            painter.setBrush(brush);
            painter.setPen(pen);
            painter.drawEllipse(0, 0, rect().width(), rect().height());
        }
    };

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
        QVector<HighlightWidget*> highlightWidgets;

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
        void positionHighlight(HighlightWidget *widget, const FingerHighlight &highlight);
    };
}

#endif // FINGERBOARD_H
