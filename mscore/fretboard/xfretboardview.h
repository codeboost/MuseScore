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
        void centralize();
        vg::XFretboard* fretboard = nullptr;
        void positionFretboard();
    };
}
#endif // XFRETBOARDVIEW_H
