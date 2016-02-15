#include "fretboard.h"
#include <math.h>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QToolBar>
#include <QIcon>
#include <QToolButton>

namespace vg
{
    Fretboard::Fretboard(QWidget *parent): QWidget(parent)
    {
        fingerboard = new Fingerboard(this, model);
        QBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(fingerboard);
          setPalette(QPalette(Qt::white));
          setAutoFillBackground(true);

        setLayout(layout);
    }

    void Fretboard::setOrientation(Qt::Orientation orientation)
    {
        if (model.orientation != orientation)
        {
            model.orientation = orientation;
            model.update();
            fingerboard->repaint();
        }
    }

    void Fretboard::paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        paintHighlights(painter);
    }

    void Fretboard::paintHighlights(QPainter &painter)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::green);
        painter.setRenderHint(QPainter::Antialiasing);

        for (const Highlight& highlight : highlights)
        {
            paintHighlight(painter, highlight);
        }
    }

    void Fretboard::paintHighlight(QPainter& painter, const Highlight& highlight)
    {
        qDebug() << "fret: " << highlight.fretNumber << "; string:" << highlight.stringNumber;

        QRect r = fingerboard->model.getFretRect(highlight.fretNumber + 1,  highlight.stringNumber + 1);

        qDebug() << "rect=" << r;

        int dotSize = 15;
        QPoint fretCenter = fingerboard->mapToParent(r.center());
        QPoint topLeft = fingerboard->mapToParent(r.topLeft());

        if (model.orientation == Qt::Horizontal)
        {
            painter.drawEllipse(fretCenter.x() - dotSize / 2, topLeft.y() - dotSize / 2, dotSize, dotSize);
        }
        else
        {
            painter.drawEllipse(topLeft.x() - dotSize / 2, fretCenter.y() - dotSize / 2, dotSize, dotSize);
        }
    }

    void Fretboard::resizeEvent(QResizeEvent *)
    {
        if (rect().height() > rect().width())
        {
            setOrientation(Qt::Vertical);
        }
        else
        {
            setOrientation(Qt::Horizontal);
        }
    }


    void Fretboard::onRotate()
    {
        qDebug() << "onRotate";
        Qt::Orientation orientation = (model.orientation == Qt::Horizontal) ? Qt::Vertical : Qt::Horizontal;
        setOrientation(orientation);
    }

    void Fretboard::onSwapSides()
    {
        qDebug() << "onSwapSides";
        model.stringOrder = (model.stringOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
        model.update();
        fingerboard->repaint();
    }

    void Fretboard::increaseStrings()
    {
        qDebug() << "increaseStrings";
        model.numberOfStrings++;
        model.update();
        fingerboard->repaint();
    }

    void Fretboard::decreaseStrings()
    {
        qDebug() << "DecreaseStrings";
        if (model.numberOfStrings > 1)
        {
            model.numberOfStrings--;
            model.update();
            fingerboard->repaint();
        }
    }

    void Fretboard::increaseFrets()
    {
        model.numberOfFrets++;
        model.update();
        fingerboard->repaint();
    }

    void Fretboard::decreaseFrets()
    {
        if (model.numberOfFrets > 1)
        {
            model.numberOfFrets--;
            model.update();
            fingerboard->repaint();
        }

    }
}


