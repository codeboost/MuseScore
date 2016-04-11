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
        if (fretboard)
        {
            scene()->removeItem(fretboard.data());
        }

        fretboard = XFretboard::Ptr(new XFretboard(nullptr, options));

        fretboard->setRect(fretboard->boundingRect());
        scene()->addItem(&*fretboard);
        fitInView(&*fretboard);
    }

    void XFretboardView::flipStrings()
    {
        QTransform t;
        t.scale(1, -1);
        t.translate(0, -fretboard->boundingRect().height());
        mirrorAndFlip = t;
        fretboard->setTransform(t, true);
        fitInView(&*fretboard);
    }

    void XFretboardView::mirrorSides()
    {
        QTransform t;
        t.scale(-1, 1);
        t.translate(-fretboard->boundingRect().width(), 0);
        fretboard->setTransform(t, true);
        mirrorAndFlip = t;
        fitInView(&*fretboard);
    }

    void XFretboardView::toggleOrientation()
    {
        if (orientation == Qt::Vertical)
            setOrientation(Qt::Horizontal);
        else
            setOrientation(Qt::Vertical);
    }

    void XFretboardView::setOrientation(Qt::Orientation o)
    {
        if (o == orientation)
            return;

        orientation = o;

        QTransform t;

        if (orientation == Qt::Vertical)
        {
            t.rotate(90);
            t.translate(0, -fretboard->rect().height());
        }

        fretboard->setTransform(t, false);
        fretboard->setTransform(mirrorAndFlip, true);
        fitInView(&*fretboard);

    }

    void XFretboardView::resizeEvent(QResizeEvent *event)
    {
        Q_UNUSED(event);

        float aspect = rect().width() / rect().height();

        if (aspect < 1.0f)
        {
            setOrientation(Qt::Vertical);
        }
        else
        {
            setOrientation(Qt::Horizontal);
        }
        fitInView(&*fretboard);
    }

}
