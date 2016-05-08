//=============================================================================
//  Virtual Guitar Player
//  Copyright (C) 2016 Larry Jones
//  Developed by Florin Braghis
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================


#ifndef guitartools_h
#define guitartools_h
#include "fretboard/xfretboardview.h"

namespace Ms
{
    class FretContainer;
    
    class GuitarFretboard : public QDockWidget {
        Q_OBJECT
    protected:
        
        
    public:
        explicit GuitarFretboard(QWidget* parent = 0);
        void heartBeat(QList<const Note*> notes);
        vg::XFretboardView* fretboard;
        
        void highlightNote(const Note* note);
        void highlightChord(const Chord* chord);
        void addHighlight(const Note* note);
        void addHighlight(int nString, int nFret);
        
        void mirror();
        void rotate();
        void flip(); 
        void toggleFretNumbers();
        void changeSelection(SelState state);
        
        void setDisplayedPart(const Ms::Part* part);
        void setDisplayedInstrument(const Ms::Instrument* instrument);
        
        FretContainer* fretContainer;
        const Part* part;
        const Instrument* instrument; 
        
    protected:
        void resizeEvent(QResizeEvent* event) override;
        vg::FretHighlights highlights;

    };
}


#endif /* guitartools_h */
