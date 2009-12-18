//=============================================================================
//  MusE Score
//  Linux Music Score Editor
//  $Id$
//
//  Copyright (C) 2002-2009 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef __KEYSIG_H__
#define __KEYSIG_H__

#include "element.h"

class Sym;
class Segment;

#define CUSTOM_KEYSIG   0x10000
#define ACCIDENTAL_MASK 0x00ff
#define NATURAL_MASK    0xff00
#define NATURAL_SHIFT   8

//---------------------------------------------------------
//   KeySig
//---------------------------------------------------------

/**
 The KeySig class represents a Key Signature on a staff
*/

struct KeySym {
      int sym;
      QPointF spos;
      QPointF pos;
      };

class KeySig : public Element {
      QList<KeySym*> keySymbols;
      void addLayout(int sym, double x, int y);

   public:
      KeySig(Score*);
      KeySig(const KeySig&);
      virtual KeySig* clone() const { return new KeySig(*this); }
      virtual QPointF canvasPos() const;      ///< position in canvas coordinates
      virtual void draw(QPainter&) const;
      virtual ElementType type() const { return KEYSIG; }
      virtual bool acceptDrop(ScoreView*, const QPointF&, int, int) const;
      virtual Element* drop(const QPointF&, const QPointF&, Element*);
      virtual void layout();
      void setSig(int oldSig, int newSig);
      void setOldSig(int oldSig);
      Segment* segment() const { return (Segment*)parent(); }
      Measure* measure() const { return (Measure*)parent()->parent(); }
      int keySignature() const { return char(subtype() & 0xff); }    // -7 - +7
      Space space() const;
      void setCustom(const QList<KeySym*>& symbols);
      virtual void write(Xml&) const;
      virtual void read(QDomElement);
      };

extern const char* keyNames[15];

#endif

