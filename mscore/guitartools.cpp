//
//  guitartools.cpp
//  mscore
//
//  Created by Florin Braghis on 2/8/16.
//
//

#include <QVBoxLayout>
#include <QList>
#include <QTimer>

#include "accessibletoolbutton.h"
#include "libmscore/note.h"
#include "libmscore/staff.h"
#include "libmscore/part.h"
#include "libmscore/instrument.h"
#include "libmscore/stringdata.h"
#include "libmscore/score.h"
#include "libmscore/instrument.h"
#include "libmscore/chord.h"

#include "shortcut.h"
#include "scoreview.h"
#include "musescore.h"

#include "guitartools.h"


//---------------------------------------------------------
//   Guitar tools
//---------------------------------------------------------


namespace Ms
{
    extern QAction* getAction(const char*);
    
    class FretContainer : public QWidget
    {
        QToolBar* toolbar;
    public:
        
        
        FretContainer(QWidget* parent, vg::XFretboardView* fretboard): QWidget(parent)
        {
            toolbar = new QToolBar(this);
            toolbar->addWidget(new AccessibleToolButton(toolbar, getAction("fretboard-flip")));
            toolbar->addWidget(new AccessibleToolButton(toolbar, getAction("fretboard-mirror")));
            toolbar->setMaximumHeight(32);
            
            
            QBoxLayout* layout = new QVBoxLayout();
            layout->addWidget(toolbar);
            layout->addWidget(fretboard);
            setLayout(layout);

        }
        
    protected slots:
        
    };
    
    ///////////////////////
    
    GuitarFretboard::GuitarFretboard(QWidget* parent) : QDockWidget(parent)
    {
        instrument = nullptr;
        part = nullptr;
        setObjectName("fretboard");
        //setWindowTitle(tr("Guitar Fretboard"));
        //setAllowedAreas(Qt::DockWidgetAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea));
        //fretboard = new vg::Fretboard(this);
        fretboard = new vg::XFretboardView(this);
        //fretContainer = new FretContainer(this, fretboard);
        fretContainer = new FretContainer(this, fretboard);
    }
    
    void GuitarFretboard::resizeEvent(QResizeEvent *event)
    {
        fretContainer->setGeometry(rect());
    }
    
    void GuitarFretboard::highlightNote(const Note* note)
    {
        highlights = fretboard->fretboard->emptyHighlights();
        fretboard->fretboard->hideHighlights();
        addHighlight(note);
        fretboard->fretboard->setHighlights(highlights);
    }
    
    void GuitarFretboard::addHighlight(const Note* note)
    {
        if (note->string() >=0 && note->fret() >= 0)
        {
            addHighlight(note->string(), note->fret());
        }
        else
        {
            const StringData* stringData = note->staff()->part()->instrument()->stringData();
            int nString = 0;
            int nFret = 0;
            if (stringData->convertPitch(note->pitch(), note->staff(), note->chord()->tick(), &nString, &nFret))
            {
                addHighlight(nString, nFret);
            }
            else
            {
                qDebug() << "No strings/frets for note " << note;
            }
        }

    }
    
    void GuitarFretboard::addHighlight(int nString, int nFret)
    {
        static int counter = 0;
     //   qDebug() << counter++ << ". addHighlight: [" << nString << " : " << nFret << "]";
        
        highlights[nString] = nFret;
    }
    
    void GuitarFretboard::setDisplayedPart(const Part* part)
    {
        
    }
    
    bool isStringedInstrument(const Ms::Instrument* aInstrument)
    {
        const StringData* stringData = aInstrument->stringData();
        return stringData->strings() > 0 && stringData->frets() > 0;
    }

    void GuitarFretboard::setDisplayedInstrument(const Ms::Instrument *aInstrument)
    {
        instrument = aInstrument;
        
        if (instrument && isStringedInstrument(instrument))
        {
            QString instrumentName = instrument->trackName();
            setWindowTitle(instrumentName);

            const StringData* stringData = instrument->stringData();
            
            qDebug() << "Instrument selected: " << instrumentName;
            qDebug() << "minPitchA: " << instrument->minPitchA() << ";minPitchP: " << instrument->minPitchP();
            qDebug() << "Frets: " << stringData->frets() << "; Strings: " << stringData->strings();
            
            vg::XFretboard::Options options;
            options.numberOfFrets = stringData->frets();
            options.numberOfStrings = stringData->strings();
            fretboard->setEnabled(true);
            highlights.clear();
            highlights.resize(options.numberOfStrings);
            
        }
        else
        {
            highlights.clear();
            instrument = nullptr;
            fretboard->setEnabled(false);
            setWindowTitle("");
        }
    }
    
    void GuitarFretboard::changeSelection(Ms::SelState state)
    {
        if (!mscore || !mscore->currentScore())
            return;
        
        const Selection sel = mscore->currentScore()->selection();
        
        if (sel.isNone())
        {
            //TODO: Find first suitable instrument
            
            foreach (Ms::Staff* staff, mscore->currentScore()->staves())
            {
                Ms::Instrument* instrument = staff->part()->instrument();
                
                if (isStringedInstrument(instrument))
                {
                    setDisplayedInstrument(instrument);
                }
                    
            }
            
            return;
        }
        
        if (sel.isSingle())
        {
            setDisplayedInstrument(sel.element()->staff()->part()->instrument());
        }
        else
        {
            qDebug() << "Selection not single. isList = " << sel.isList() << "; isRange = " << sel.isRange();
            
            bool same = true;
            const Element* first = sel.elements().first();
            for (auto element : sel.elements())
            {
                if (first->staff()->part() != element->staff()->part())
                {
                    same = false;
                }
            }
            
            if (!same)
            {
                qDebug() << "There are multiple parts selected";
                setDisplayedInstrument(nullptr);
            }
            else
            {
                setDisplayedInstrument(first->staff()->part()->instrument());
            }
        }
    }
    
    void GuitarFretboard::highlightChord(const Chord* chord)
    {
        qDebug() << "Highlight chord with  " << chord->notes().size() << " notes";

        QList<const Note*> displayNotes;
        auto notes = chord->notes();
        for (const Note* note: notes)
        {
            displayNotes.push_back(note);
        }
        
        heartBeat(displayNotes);
    }
    
    void GuitarFretboard::mirror()
    {
        fretboard->mirrorSides();

    }
    
    void GuitarFretboard::rotate()
    {
        fretboard->toggleOrientation();
    }
    
    void GuitarFretboard::flip()
    {
        fretboard->flipStrings();

    }

    void GuitarFretboard::heartBeat(QList<const Ms::Note *> notes)
    {
        highlights.clear();
        int numStrings = fretboard->fretboard->options.numberOfStrings;
        highlights.resize(numStrings);
        highlights.fill(-1);  //no highlights
        QString fullmsg;
        
        for (const Ms::Note* note : notes)
        {
            if (note->staff()->part()->instrument() == this->instrument)
            {
                addHighlight(note);
            }
        }
        
        fretboard->fretboard->setHighlights(highlights);
    }
}
