#ifndef XFRETBOARDVIEW_H
#define XFRETBOARDVIEW_H

#include <QGraphicsView>
#include "fretboard/xfretboard.h"

namespace vg
{
    class XFretboardView : public QGraphicsView
    {
    public:
        explicit XFretboardView(QWidget *parent = 0);
        void resizeEvent(QResizeEvent *event) override;
        void setFretboardOptions(const vg::XFretboard::Options& options);

        void mirrorStrings();
        void mirrorSides();
        void toggleOrientation();

        vg::XFretboard* getFretboard()
        {
            return fretboard;
        }
        
    protected:
        struct Impl;
        Impl* impl;
        vg::XFretboard* fretboard = nullptr;
    };
}
#endif // XFRETBOARDVIEW_H
