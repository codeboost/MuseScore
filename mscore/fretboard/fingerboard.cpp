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
        paintNut(painter);
        paintFrets(painter);
        paintWhiteDots(painter);
        paintStrings(painter);
    }

    void Fingerboard::paintStrings(QPainter &painter)
    {
        //Actual paint
        painter.save();
        QPen pen = painter.pen();
        pen.setColor(Qt::darkYellow);
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

    void paintGradientRect(QPainter& painter, QRect& r, Qt::Orientation orientation, const QColor& darkColor, const QColor& lightColor)
    {

        QLinearGradient gradient;

        if (orientation == Qt::Horizontal)
        {
            gradient = QLinearGradient(QPoint(r.x(), r.height() / 2),
                                       QPoint(r.x() + r.width(), r.height() / 2));
        }
        else
        {
            gradient = QLinearGradient(QPoint(r.width()/2, r.y()),
                                       QPoint(r.width()/2, r.y() + r.height()));
        }

        gradient.setColorAt(0, darkColor);
        gradient.setColorAt(0.5, lightColor);
        gradient.setColorAt(1, darkColor);

        gradient.setSpread(QGradient::PadSpread);

        painter.fillRect(r, QBrush(gradient));
    }

    void Fingerboard::paintFret(QPainter& painter, float position)
    {
        painter.save();
        int fretThickness = model.fretThickness;
        QRect r;
        QLinearGradient gradient;

        if (model.orientation == Qt::Horizontal)
            r.setRect(position, 0, fretThickness, rect().height());
        else
            r.setRect(0, position, rect().width(), fretThickness);

        paintGradientRect(painter, r, model.orientation, QColor("#ccc"), QColor("#ddd"));
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
        painter.setPen(pen);

        if (model.orientation == Qt::Horizontal)
            painter.drawLine(0, 0, 0, rect().height());
        else
            painter.drawLine(0, 0, rect().width(), 0);

        painter.restore();
    }

    void Fingerboard::paintDotForFret(QPainter& painter, int fretNumber, int stringNumber)
    {
        QRect r = model.getFretRect(fretNumber,  stringNumber);
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
        painter.setBrush(QColor(180,180,180));
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
}
