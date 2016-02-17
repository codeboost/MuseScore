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
#include <QToolBar>

namespace Ms
{
    class FretContainer;
    
    class GuitarFretboard : public QDockWidget {
        Q_OBJECT
    protected:


    public:
        explicit GuitarFretboard(QWidget* parent = 0);
        void heartBeat(QList<const Note*> notes);
        vg::Fretboard* fretboard;
        
        void highlightNote(const Note* note);
        void addHighlight(const Note* note);
          
          void mirror(){
              fretboard->onSwapSides();
          }
          void rotate()
          {
              fretboard->onRotate();
          }
          
        FretContainer* fretContainer;
        
    protected:
        void resizeEvent(QResizeEvent* event) override;
    };
}


#endif /* guitartools_h */
