//
//  guitartools.hpp
//  mscore
//
//  Created by Florin Braghis on 2/8/16.
//
//

#ifndef guitartools_h
#define guitartools_h

#include "libmscore/note.h"
#include "fretboard/fretboard.h"

namespace Ms
{
    class GuitarFretboard : public QDockWidget {
        Q_OBJECT
    public:
        explicit GuitarFretboard(QWidget* parent = 0);
        void heartBeat(QList<const Note*> notes);
        vg::Fretboard* fretboard;
        
    protected:
        void resizeEvent(QResizeEvent* event) override;
    };
}


#endif /* guitartools_h */
