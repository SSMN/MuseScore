//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: trill.cpp,v 1.6 2006/03/28 14:58:58 wschweer Exp $
//
//  Copyright (C) 2002-2006 Werner Schweer (ws@seh.de)
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

#include "trill.h"
#include "style.h"
#include "system.h"
#include "measure.h"
#include "xml.h"
#include "utils.h"
#include "sym.h"

static const QChar TC(0xe161);
static const QChar TLC(0xe16f);

//---------------------------------------------------------
//   draw
//---------------------------------------------------------

void TrillSegment::draw(QPainter& p)
      {
      QRectF b1 = symbols[trillSym].bbox();
//      qreal w1  = symbols[trillSym].width();
      QRectF b2 = symbols[trillelementSym].bbox();
      qreal w2  = symbols[trillelementSym].width();
      int n     = lrint((pos2().x() - (b1.width() - b2.x())) / w2);

      symbols[trillSym].draw(p, -b1.x(), 0);
      symbols[trillelementSym].draw(p, b1.width() - b2.x(), b2.y(), n);
      LineSegment::draw(p);
      }

//---------------------------------------------------------
//   bbox
//---------------------------------------------------------

QRectF TrillSegment::bbox() const
      {
      QRectF rr(symbols[trillSym].bbox());
      QRectF r(0.0, rr.y(), pos2().x(), rr.height());
      if (mode != NORMAL) {
            r |= bbr1;
            r |= bbr2;
            }
      return r;
      }

//---------------------------------------------------------
//   Trill
//---------------------------------------------------------

Trill::Trill(Score* s)
  : SLine(s)
      {
      text = TC;
      }

//---------------------------------------------------------
//   setSubtype
//---------------------------------------------------------

void Trill::setSubtype(int val)
      {
      Element::setSubtype(val);
      }

//---------------------------------------------------------
//   layout
//---------------------------------------------------------

void Trill::layout(ScoreLayout* layout)
      {
      SLine::layout(layout);
      if (!parent())
            return;
#if 0
      qreal trillDistance = _spatium * 2.5;
      Measure* measure = (Measure*)parent();
      System* system   = measure->system();
      SysStaff* sstaff = system->staff(staffIdx());
      qreal y = sstaff->bbox().top() - trillDistance;
      setPos(ipos().x(), y);
#endif
      }

//---------------------------------------------------------
//   write
//---------------------------------------------------------

void Trill::write(Xml& xml) const
      {
      xml.stag("Trill");
      SLine::writeProperties(xml);
      xml.etag();
      }

//---------------------------------------------------------
//   read
//---------------------------------------------------------

void Trill::read(QDomNode node)
      {
      for (node = node.firstChild(); !node.isNull(); node = node.nextSibling()) {
            if (!SLine::readProperties(node))
                  domError(node);
            }
      }

//---------------------------------------------------------
//   createSegment
//---------------------------------------------------------

LineSegment* Trill::createSegment()
      {
      TrillSegment* seg = new TrillSegment(score());
      seg->setParent(this);
      seg->setStaff(staff());
      return seg;
      }


