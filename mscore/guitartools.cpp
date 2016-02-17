//
//  guitartools.cpp
//  mscore
//
//  Created by Florin Braghis on 2/8/16.
//
//

#include "guitartools.h"
#include "libmscore/staff.h"
#include <QVBoxLayout>
#include "libmscore/part.h"
#include "libmscore/instrument.h"
#include "libmscore/stringdata.h"
#include "accessibletoolbutton.h"
#include "shortcut.h"


//---------------------------------------------------------
//   Guitar tools
//---------------------------------------------------------


namespace Ms
{
    extern QAction* getAction(const char*);
    
    class FretContainer : public QWidget
    {
        vg::Fretboard* fretboard;
        QToolBar* toolbar;
    public:
        
        FretContainer(QWidget* parent, vg::Fretboard* aFretboard): QWidget(parent), fretboard(aFretboard)
        {
            toolbar = new QToolBar(this);
            toolbar->addWidget(new AccessibleToolButton(toolbar, getAction("fretboard-rotate")));
            toolbar->addWidget(new AccessibleToolButton(toolbar, getAction("fretboard-mirror")));
            toolbar->setMaximumHeight(32);
            
            QBoxLayout* layout = new QVBoxLayout();
            layout->addWidget(toolbar);
            layout->addWidget(fretboard);
            setLayout(layout);
        }
        
    };
    
    
    GuitarFretboard::GuitarFretboard(QWidget* parent) : QDockWidget(parent)
    {
        setObjectName("fretboard");
        //setWindowTitle(tr("Guitar Fretboard"));
        //setAllowedAreas(Qt::DockWidgetAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea));
        fretboard = new vg::Fretboard(this);
        fretContainer = new FretContainer(this, fretboard);
    }
    
    void GuitarFretboard::resizeEvent(QResizeEvent *event)
    {
        //fretboard->setGeometry(rect());
        fretContainer->setGeometry(rect());
    }
    
    void GuitarFretboard::addHighlight(const Note* note)
    {
        if (note->string() >=0 && note->fret() >= 0)
        {
            vg::FingerHighlight highlight(note->string(), note->fret());
            fretboard->model.highlights.push_back(highlight);
        }
    }
    
    
    void GuitarFretboard::highlightNote(const Note* note)
    {
        fretboard->model.highlights.clear();
        addHighlight(note);
        fretboard->update();
    }
    
    void GuitarFretboard::heartBeat(QList<const Ms::Note *> notes)
    {
        fretboard->model.highlights.clear();
        QString fullmsg;
        for (const Ms::Note* note : notes)
        {
            bool tablature = note->staff() && note->staff()->isTabStaff();
            
            bool hasStrings = false;
            
            Instrument* instrument = note->staff()->part()->instrument();
            if (instrument)
            {
                const StringData* stringData = instrument->stringData();
                if (stringData)
                {
                    int numberOfStrings = stringData->strings();
                    int numberOfFrets = stringData->frets();
                    
                    if (numberOfStrings > 0 && numberOfFrets > 0)
                    {
                        hasStrings = true;
                        if (fretboard->model.numberOfStrings != numberOfStrings || fretboard->model.numberOfFrets != numberOfFrets)
                        {
                            fretboard->model.numberOfStrings = numberOfStrings;
                            fretboard->model.numberOfFrets = numberOfFrets;
                            fretboard->model.update();
                        }
                    }
                }
            }
            
            if (hasStrings)
            {
                addHighlight(note);
                QString msg = QString("[%1:%2],").arg(note->string()).arg(note->fret());
                fullmsg.append(msg);
            }
        }
        qDebug() << fullmsg;
        fretboard->update();
    }
}
