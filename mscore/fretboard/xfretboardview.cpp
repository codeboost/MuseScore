#include "xfretboardview.h"
#include <QDebug>
#include <QKeyEvent>

namespace vg
{
    const float FretboardWidth = 700.0f;
    const float FretboardHeight = 200.0f;

    XFretboardView::XFretboardView(QWidget *parent) : QGraphicsView(parent)
    {
        setRenderHint(QPainter::Antialiasing);

        setScene(new QGraphicsScene(this));

        QPen pen = QPen(QColor("#ff0000"));
        QBrush brush = QColor("#eee");

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
        scene()->addItem(fretboard);

        fretboard->setRect(0, 0, FretboardWidth, FretboardHeight);
//        fretboard->setTransformOriginPoint(fretboard->rect().center());
        fretboard->repositionComponents();
        if (!first) positionFretboard();

    }


    void XFretboardView::centralize()
    {
        QPointF center = rect().center() - fretboard->rect().center();
        fretboard->setPos(center);

        XFretboard* t = nullptr;
    }

    void XFretboardView::positionFretboard()
    {

        qDebug() << "positionFretboard: " << rect();

        QRect sceneRect = rect();
        scene()->setSceneRect(sceneRect);
        fretboard->setRect(sceneRect);

        if (sceneRect.width() > sceneRect.height())
        {
            fretboard->setRotation(0);
        }
        else
        {

            fretboard->setRotation(90);
        }

        centralize();
    }

    void XFretboardView::resizeEvent(QResizeEvent *event)
    {
        qDebug() << "Resize : " << rect();
        positionFretboard();
    }

    void XFretboardView::keyPressEvent(QKeyEvent *event)
    {
        qDebug() << "keyPressed: " << event->text();

        int eventKey = event->key();
        if (event->text().toLower() == "r")
        {
            qreal rot = fretboard->rotation() + 90.0;
            qDebug() << "setting rotation: " << rot;
            fretboard->setRotation(rot);
            qDebug() << "resulting rect: " << fretboard->rect();
        }
        else if (event->text().toLower() == "m")
        {
            QTransform t;
            t.scale(1, -1);
            t.translate(0, -fretboard->rect().height());
            fretboard->setTransform(t, true);
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
