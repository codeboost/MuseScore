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
        void keyPressEvent(QKeyEvent* event) override;
        void setFretboardOptions(const vg::XFretboard::Options& options);
        vg::XFretboard* getFretboard()
        {
            return fretboard;
        }
        
    protected:
        struct Impl;
        Impl* impl;
        void centralize();
        vg::XFretboard* fretboard = nullptr;
        void positionFretboard();

        qreal rotation = 0.0f;
        qreal translatex = 0.0f;
        qreal translatey = 0.0f;
    };
}
#endif // XFRETBOARDVIEW_H
