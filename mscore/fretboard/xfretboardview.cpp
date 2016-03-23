#include "xfretboardview.h"
#include <QDebug>
#include <QKeyEvent>

namespace vg
{

    XFretboardView::XFretboardView(QWidget *parent) : QGraphicsView(parent)
    {

        setRenderHint(QPainter::Antialiasing);

        setScene(new QGraphicsScene(this));

        QPen pen = QPen(QColor("#ff0000"));
        QBrush brush = QColor("#eee");

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        vg::XFretboard::Options options;
        setFretboardOptions(options);

    }

    void XFretboardView::setFretboardOptions(const XFretboard::Options &options)
    {
        bool first = fretboard == nullptr;
        if (fretboard)
        {
            scene()->removeItem(fretboard);
        }

        fretboard = new XFretboard(nullptr, options);
        fretboard->setRect(fretboard->boundingRect());
        scene()->addItem(fretboard);

        if (first)
        {
            fitInView(fretboard);
            fretboard->createFretboardComponents();
//            fretboard->repositionComponents();
        }
    }

    void XFretboardView::resizeEvent(QResizeEvent *event)
    {
        Q_UNUSED(event);
        fitInView(fretboard);
    }

    void XFretboardView::keyPressEvent(QKeyEvent *event)
    {
        qDebug() << "keyPressed: " << event->text();

        int eventKey = event->key();
        if (event->text().toLower() == "r")
        {
            QTransform t;
            t.rotate(90);
            t.translate(0, -fretboard->rect().height());

            qDebug() << "Rect" << fretboard->rect();
            fretboard->setTransform(t, true);

        }
        else if (event->text().toLower() == "m")
        {
            QTransform t;
            t.scale(1, -1);
            t.translate(0, -fretboard->rect().height());
            fretboard->setTransform(t, true);
            fretboard->update();
        }
        else if (event->text().toLower() == "n")
        {
            QTransform t;
            t.scale(-1, 1);
            t.translate(-fretboard->rect().width(), 0);
            fretboard->setTransform(t, true);
        }
        else if (event->text() == "S")
        {
            vg::XFretboard::Options opts = fretboard->options;
            opts.numberOfStrings = std::min(12, opts.numberOfStrings + 1);
            setFretboardOptions(opts);
        }
        else if (event->text() == "s")
        {
            vg::XFretboard::Options opts = fretboard->options;
            opts.numberOfStrings = std::max(2, opts.numberOfStrings - 1);
            setFretboardOptions(opts);
        }
        else if (event->text().toLower() == "a")
        {
            if (event->text() == "A")
            {
                fretboard->hideHighlights();
                return;
            }

            QVector<int> highlights(fretboard->options.numberOfStrings);
            highlights.fill(-1);
            highlights[0] = 3;
            highlights[4] = 2;
            highlights[5] = 3;
            fretboard->setHighlights(highlights);
        }
        else if (event->text().toLower() == "b")
        {
            if (event->text() == "B")
            {
                fretboard->hideHighlights();
                return;
            }

            QVector<int> highlights(fretboard->options.numberOfStrings);
            highlights.fill(-1);
            highlights[3] = 2;
            highlights[4] = 2;
            fretboard->setHighlights(highlights);
        }

        else if (eventKey >= Qt::Key_0 && eventKey <= Qt::Key_9)
        {
            qDebug() << eventKey << event->modifiers();
            fretboard->highlightSelectedAtFret(eventKey - Qt::Key_0);
        }
        else if (eventKey == Qt::Key_Plus)
        {
            fretboard->selectedString = (fretboard->selectedString + 1) % fretboard->options.numberOfStrings;
            fretboard->pluckSelected();
        }
        else if (eventKey == Qt::Key_Minus)
        {
            fretboard->selectedString = fretboard->selectedString - 1;
            if (fretboard->selectedString < 0)
                fretboard->selectedString = fretboard->options.numberOfStrings;
            fretboard->pluckSelected();
        }

        else if (event->text() == "1")
        {
            fretboard->addHighlight(1, 1);
        }
    }


}
