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
        fretboard->createFretboardComponents();
        scene()->addItem(fretboard);
        fitInView(fretboard);
    }

    void XFretboardView::mirrorStrings()
    {
        QTransform t;
        t.scale(1, -1);
        t.translate(0, -fretboard->boundingRect().height());
        fretboard->setTransform(t, true);
        fitInView(fretboard);
    }

    void XFretboardView::mirrorSides()
    {
        QTransform t;
        t.scale(-1, 1);
        t.translate(-fretboard->boundingRect().width(), 0);
        fretboard->setTransform(t, true);
        fitInView(fretboard);
    }

    void XFretboardView::toggleOrientation()
    {
        QTransform t;
        t.rotate(90);
        t.translate(0, -fretboard->rect().height());
        fretboard->setTransform(t, true);
        fitInView(fretboard);
    }

    void XFretboardView::resizeEvent(QResizeEvent *event)
    {
        Q_UNUSED(event);
        fitInView(fretboard);
    }

}
