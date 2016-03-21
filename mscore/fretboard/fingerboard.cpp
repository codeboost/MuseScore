#include "fingerboard.h"
#include <math.h>
#include <QPainter>
#include <QDebug>
#include <QLinearGradient>
#include <QBrush>

namespace vg
{
    Fingerboard::Fingerboard(QWidget *parent, FretboardModel& aModel): QWidget(parent), model(aModel)
    {

    }

    void Fingerboard::resizeEvent(QResizeEvent *)
    {
        model.setRect(rect());
    }

    void Fingerboard::paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        painter.save();

        if (model.orientation == Qt::Horizontal)
        {
            if (model.stringOrder == Qt::AscendingOrder)
                painter.translate(model.noteNameRadius, 0);
            else
                painter.translate(-model.noteNameRadius, 0);
        }
        else
        {
            if (model.stringOrder == Qt::AscendingOrder)
                painter.translate(0, model.noteNameRadius);
            else
                painter.translate(0, model.noteNameRadius);
        }

        paintNut(painter);
        paintFrets(painter);
        paintWhiteDots(painter);
        paintStrings(painter);

        paintNoteNames(painter);

        paintHighlights(painter);

        painter.restore();
    }


    void Fingerboard::paintNoteNames(QPainter& painter)
    {
        painter.save();
        QBrush brush = nutColor;
        QPen pen = QPen(Qt::white);

        painter.setBrush(brush);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing);

        int noteNameDiameter = model.noteNameRadius * 2;

        for (int k = 0; k < model.numberOfStrings; k++)
        {
            QString noteName = model.noteNameForString(k);

            if (model.orientation == Qt::Horizontal)
            {
                int xpos = model.posForNut() - model.noteNameRadius;
                int ypos = model.posForString(k) - model.noteNameRadius;
                painter.drawEllipse(xpos, ypos, noteNameDiameter, noteNameDiameter);
                painter.drawText(xpos, ypos, noteNameDiameter, noteNameDiameter, Qt::AlignHCenter | Qt::AlignVCenter, noteName);
            }
            else
            {
                int ypos = model.posForNut() - model.noteNameRadius;
                int xpos = model.posForString(k) - model.noteNameRadius;
                painter.drawEllipse(xpos, ypos, noteNameDiameter, noteNameDiameter);
                painter.drawText(xpos, ypos, noteNameDiameter, noteNameDiameter, Qt::AlignHCenter | Qt::AlignVCenter, noteName);
            }
        }
        painter.restore();
    }

    void Fingerboard::paintStrings(QPainter &painter)
    {
        //Actual paint
        painter.save();
        QPen pen = painter.pen();
        pen.setColor(stringColor);
        pen.setWidth(2);
        painter.setPen(pen);

        for (int i = 0; i < model.numberOfStrings; i++)
        {
                float thickRate = model.widthForString(i);
                pen.setWidth((int)round(thickRate * 5.0));
                painter.setPen(pen);

            float position = model.posForString(i);

            if (model.orientation == Qt::Horizontal)
                painter.drawLine(0, position, rect().width(), position);
            else
                painter.drawLine(position, 0, position, rect().height());
        }

        painter.restore();
    }

     void Fingerboard::paintFret(QPainter& painter, float position)
    {
        painter.save();
        int fretThickness = model.fretThickness;
        QRect r;

        QPen pen = painter.pen();
        pen.setWidth(fretThickness);
        pen.setColor(fretColor);
        painter.setPen(pen);

        if (model.orientation == Qt::Horizontal){
            r.setRect(position, 0, fretThickness, rect().height());
            painter.drawLine(r.topLeft(), r.bottomLeft());
        }
        else
        {
            r.setRect(0, position, rect().width(), fretThickness);
            painter.drawLine(r.topLeft(), r.topRight());
        }
        painter.restore();
    }

    void Fingerboard::paintFrets(QPainter &painter)
    {
        //Start from 1, do not paint fret 0
        for (int index = 1; index < model.numberOfFrets; index++)
        {
            auto position = model.posForFret(index);
            paintFret(painter, position);
        }
    }

    void Fingerboard::paintNut(QPainter& painter)
    {
        painter.save();

        QPen pen = painter.pen();
        pen.setWidth(model.nutThickness);
        pen.setColor(nutColor);
        painter.setPen(pen);


        int pos = model.posForNut();

        if (model.orientation == Qt::Horizontal)
            painter.drawLine(pos, 0, pos, rect().height());
        else
            painter.drawLine(0, pos, rect().width(), pos);

        painter.restore();
    }


    void Fingerboard::paintDotForFret(QPainter& painter, int fretNumber, int stringNumber)
    {
        Q_ASSERT(fretNumber <= model.numberOfFrets);
        Q_ASSERT(stringNumber > 0 && stringNumber <= model.numberOfStrings);
        QPoint p1 = model.intersectionPoint(fretNumber - 1, stringNumber - 1);
        QPoint p2 = model.intersectionPoint(fretNumber, stringNumber);
        QRect r(p1, p2);
        QPoint fretCenter = r.center();
        int dotSize = 10;
        painter.drawEllipse(fretCenter.x() - dotSize / 2, fretCenter.y() - dotSize / 2, dotSize, dotSize);
    }

    void Fingerboard::paintWhiteDots(QPainter &painter)
    {
        static std::vector<int> dotPositions = {3, 5, 7, 9, 12, 15, 17, 19, 21, 24};
        static const int octaveFret = 12;

        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(dotsColor);
        painter.setRenderHint(QPainter::Antialiasing);

        for (size_t k = 0; k < dotPositions.size(); k++)
        {
            int fretNumber = dotPositions[k];
            if (fretNumber <= model.numberOfFrets)
            {
                if (fretNumber == octaveFret)
                {
                    int firstDot = model.numberOfStrings / 3;
                    int secondDot = model.numberOfStrings - firstDot;
                    paintDotForFret(painter, fretNumber, firstDot);
                    paintDotForFret(painter, fretNumber, secondDot);
                }
                else
                {
                    paintDotForFret(painter, fretNumber, model.numberOfStrings / 2);
                }
            }
        }
        painter.restore();
    }

    void Fingerboard::paintHighlights(QPainter &painter)
    {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        painter.setRenderHint(QPainter::Antialiasing);

        for (const auto& highlight : model.getHighlights())
        {
//            HighlightWidget* widget = highlightWidgets[index++];
//            positionHighlight(widget, highlight);
            paintHighlight(painter, highlight);
        }
    }

    void Fingerboard::positionHighlight(HighlightWidget* widget, const FingerHighlight& highlight)
    {
        QRect r(0, 0, 0, 0);
        if (highlight.fretNumber > 0)
        {
            QPoint p1 = model.intersectionPoint(highlight.fretNumber - 1, highlight.stringNumber);
            QPoint p2 = model.intersectionPoint(highlight.fretNumber, highlight.stringNumber);
            r = QRect(p1, p2);
        }
        else
        {
            QPoint p1 = model.intersectionPoint(0, highlight.stringNumber);
            r = QRect(p1, p1);
        }

        widget->move(r.topLeft());
    }

    void Fingerboard::paintHighlight(QPainter& painter, const FingerHighlight& highlight)
    {

        if (!model.isValidHighlight(highlight))
        {
            //Q_ASSERT(false);
            return;
        }

        qDebug() << "Highlight -> fret: " << highlight.fretNumber << "; string:" << highlight.stringNumber;

        QRect r(0, 0, 0, 0);

        if (highlight.fretNumber > 0)
        {
            QPoint p1 = model.intersectionPoint(highlight.fretNumber - 1, highlight.stringNumber);
            QPoint p2 = model.intersectionPoint(highlight.fretNumber, highlight.stringNumber);
            r = QRect(p1, p2);
        }
        else
        {
            QPoint p1 = model.intersectionPoint(0, highlight.stringNumber);
            r = QRect(p1, p1);
        }

        int dotSize = 20;
        QPoint fretCenter = r.center();
        QPoint topLeft = r.topLeft();

        if (model.orientation == Qt::Horizontal)
        {
            painter.drawEllipse(fretCenter.x() - dotSize / 2, topLeft.y() - dotSize / 2, dotSize, dotSize);
        }
        else
        {
            painter.drawEllipse(topLeft.x() - dotSize / 2, fretCenter.y() - dotSize / 2, dotSize, dotSize);
        }
    }

}
