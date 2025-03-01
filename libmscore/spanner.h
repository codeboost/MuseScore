//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2010-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __SPANNER_H__
#define __SPANNER_H__

#include "element.h"

namespace Ms {

class Segment;
class Spanner;
class System;
class Chord;
class ChordRest;
class Note;

//---------------------------------------------------------
//   SpannerSegmentType
//---------------------------------------------------------

enum class SpannerSegmentType : char {
      SINGLE, BEGIN, MIDDLE, END
      };

//---------------------------------------------------------
//   @@ SpannerSegment
//!    parent: System
//---------------------------------------------------------

class SpannerSegment : public Element {
      Q_OBJECT

      Spanner* _spanner;
      SpannerSegmentType _spannerSegmentType;

   protected:
      QPointF _p2;
      QPointF _userOff2;

   public:
      SpannerSegment(Score* s);
      SpannerSegment(const SpannerSegment&);
      virtual SpannerSegment* clone() const = 0;

      Spanner* spanner() const              { return _spanner;            }
      Spanner* setSpanner(Spanner* val)     { return _spanner = val;      }

      void setSpannerSegmentType(SpannerSegmentType s) { _spannerSegmentType = s;               }
      SpannerSegmentType spannerSegmentType() const    { return _spannerSegmentType;            }
      bool isSingleType() const                        { return spannerSegmentType() == SpannerSegmentType::SINGLE; }
      bool isBeginType() const                         { return spannerSegmentType() == SpannerSegmentType::BEGIN;  }
      bool isSingleBeginType() const                   { return isSingleType() || isBeginType(); }
      bool isSingleEndType() const                     { return isSingleType() || isEndType(); }
      bool isMiddleType() const                        { return spannerSegmentType() == SpannerSegmentType::MIDDLE; }
      bool isEndType() const                           { return spannerSegmentType() == SpannerSegmentType::END;    }

      void setSystem(System* s);
      System* system() const                { return (System*)parent();   }

      const QPointF& userOff2() const       { return _userOff2;       }
      void setUserOff2(const QPointF& o)    { _userOff2 = o;          }
      void setUserXoffset2(qreal x)         { _userOff2.setX(x);      }
      void setPos2(const QPointF& p)        { _p2 = p;                }
      QPointF pos2() const                  { return _p2 + _userOff2; }
      const QPointF& ipos2() const          { return _p2;             }
      qreal& rxpos2()                       { return _p2.rx();        }
      qreal& rypos2()                       { return _p2.ry();        }

      virtual void startEdit(MuseScoreView*, const QPointF&) override;
      virtual void endEdit() override;
      virtual bool isEditable() const override { return true; }

      virtual QVariant getProperty(P_ID id) const override;
      virtual bool setProperty(P_ID id, const QVariant& v) override;
      virtual QVariant propertyDefault(P_ID id) const override;
      void reset() override;
      virtual void setSelected(bool f) override;
      virtual void setVisible(bool f) override;
      virtual void setColor(const QColor& col) override;

      virtual Element* nextElement() override;
      virtual Element* prevElement() override;
      virtual bool isSpannerSegment() const override { return true; }
      virtual QString accessibleInfo() const override;
      virtual void styleChanged() override;
      virtual void triggerLayout() const override;
      };

//----------------------------------------------------------------------------------
//   @@ Spanner
///   Virtual base class for slurs, ties, lines etc.
//
//    @P anchor         enum (Spanner.CHORD, Spanner.MEASURE, Spanner.NOTE, Spanner.SEGMENT)
//    @P endElement     Element           the element the spanner end is anchored to (read-only)
//    @P startElement   Element           the element the spanner start is anchored to (read-only)
//    @P tick           int               tick start position
//    @P tick2          int               tick end position
//----------------------------------------------------------------------------------

class Spanner : public Element {
      Q_OBJECT
      Q_ENUMS(Anchor)

   public:
      enum class Anchor : char {
            SEGMENT, MEASURE, CHORD, NOTE
            };
   private:
      Q_PROPERTY(Ms::Spanner::Anchor      anchor            READ anchor       WRITE setAnchor)
      Q_PROPERTY(Ms::Element*             endElement        READ endElement)
      Q_PROPERTY(Ms::Element*             startElement      READ startElement)
      Q_PROPERTY(int                      tick              READ tick         WRITE setTick)
      Q_PROPERTY(int                      tick2             READ tick2        WRITE setTick2)

      Element* _startElement { 0  };
      Element* _endElement   { 0  };

      Anchor _anchor         { Anchor::SEGMENT };
      int _tick              { -1 };
      int _ticks             {  0 };
      int _track2            { -1 };

      static QList<QPointF> userOffsets;
      static QList<QPointF> userOffsets2;

   protected:
      QList<SpannerSegment*> segments;
      // used to store spanner properties as they were at start of editing
      // and detect edit changes when edit is over
      static int editTick, editTick2, editTrack2;
      static Note * editEndNote, * editStartNote;

   public:
      Spanner(Score* = 0);
      Spanner(const Spanner&);
      ~Spanner();

      virtual Element::Type type() const = 0;
      virtual void setScore(Score* s) override;

      virtual int tick() const override { return _tick;          }
      int tick2() const                 { return _tick + _ticks; }
      int ticks() const                 { return _ticks;         }

      void setTick(int v);
      void setTick2(int v);
      void setTicks(int v);

      int track2() const       { return _track2;        }
      void setTrack2(int v)    { _track2 = v;           }

      Anchor anchor() const    { return _anchor;   }
      void setAnchor(Anchor a) { _anchor = a;      }

      const QList<SpannerSegment*>& spannerSegments() const { return segments; }
      QList<SpannerSegment*>& spannerSegments()             { return segments; }

      virtual SpannerSegment* layoutSystem(System*);

      virtual void triggerLayout() const override;
      virtual void add(Element*) override;
      virtual void remove(Element*) override;
      virtual void scanElements(void* data, void (*func)(void*, Element*), bool all=true) override;
      virtual void startEdit(MuseScoreView*, const QPointF&) override;
      virtual void endEdit() override;
      bool removeSpannerBack();
      virtual void removeUnmanaged();
      virtual void undoInsertTimeUnmanaged(int tick, int len);
      virtual void setYoff(qreal) {}    // used in musicxml import

      QVariant getProperty(P_ID propertyId) const;
      bool setProperty(P_ID propertyId, const QVariant& v);
      QVariant propertyDefault(P_ID propertyId) const;

      void computeStartElement();
      void computeEndElement();
      static Note* endElementFromSpanner(Spanner* sp, Element* newStart);
      static Note* startElementFromSpanner(Spanner* sp, Element* newEnd);
      void setNoteSpan(Note* startNote, Note* endNote);

      Element* startElement() const    { return _startElement; }
      Element* endElement() const      { return _endElement;   }

      Measure* startMeasure() const;
      Measure* endMeasure() const;

      void setStartElement(Element* e);
      void setEndElement(Element* e);

      ChordRest* startCR();
      ChordRest* endCR();

      Chord* startChord();
      Chord* endChord();

      Segment* startSegment() const;
      Segment* endSegment() const;

      virtual void setSelected(bool f) override;
      virtual void setVisible(bool f) override;
      virtual void setColor(const QColor& col) override;
      virtual Element* nextElement() override;
      virtual Element* prevElement() override;

      virtual bool isSpanner() const override { return true; }

      friend class SpannerSegment;
      };

}     // namespace Ms

Q_DECLARE_METATYPE(Ms::Spanner::Anchor);

#endif

