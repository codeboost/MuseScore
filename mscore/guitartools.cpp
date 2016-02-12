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
//---------------------------------------------------------
//   Guitar tools
//---------------------------------------------------------


namespace Ms
{
    GuitarFretboard::GuitarFretboard(QWidget* parent) : QDockWidget(parent)
    {
        setObjectName("fretboard");
        setWindowTitle(tr("Guitar Fretboard"));
        setAllowedAreas(Qt::DockWidgetAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea));
        
        fretboard = new vg::Fretboard(this);
        
//        QBoxLayout* layout = new QVBoxLayout();
//        layout->addWidget(fretboard);
//        
//        setLayout(layout);
        
    }
    
    void GuitarFretboard::resizeEvent(QResizeEvent *event)
    {
        fretboard->setGeometry(rect());
    }
    
    
    
    void GuitarFretboard::heartBeat(QList<const Ms::Note *> notes)
    {
        fretboard->highlights.clear();
        QString fullmsg;
        for (const Ms::Note* note : notes)
        {
            bool tablature = note->staff() && note->staff()->isTabStaff();
            
            if (1 || tablature)
            {
                vg::Fretboard::Highlight highlight(note->string(), note->fret());
                fretboard->highlights.push_back(highlight);
                
                QString msg = QString("[%1:%2],").arg(note->string()).arg(note->fret());
                fullmsg.append(msg);
            }
        }
        qDebug() << fullmsg;
        fretboard->repaint();
    }
}