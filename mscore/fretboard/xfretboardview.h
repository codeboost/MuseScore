#ifndef XFRETBOARDVIEW_H
#define XFRETBOARDVIEW_H

#include <QGraphicsView>
#include "fretboard/xfretboard.h"
#include <QGraphicsTransform>

namespace vg
{
    class XFretboardView : public QGraphicsView
    {
    public:
        typedef QSharedPointer<XFretboardView> Ptr;
        vg::XFretboard::Ptr fretboard;
        Qt::Orientation orientation = Qt::Horizontal;
    public:
        explicit XFretboardView(QWidget *parent = 0);
        void setFretboardOptions(const vg::XFretboard::Options& options);
        void flipStrings();
        void mirrorSides();
        void toggleOrientation();
        void toggleFretNumbers();

        void setOrientation(Qt::Orientation orientation);
    protected:
        QTransform mirrorAndFlip;
        void resizeEvent(QResizeEvent *event) override;

    };
}
#endif // XFRETBOARDVIEW_H
