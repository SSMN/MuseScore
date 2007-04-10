//=============================================================================
//  MusE Score
//  Linux Music Score Editor
//  $Id: mscore.h,v 1.54 2006/04/12 14:58:10 wschweer Exp $
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

#ifndef __TIMEDIALOG_H__
#define __TIMEDIALOG_H__

#include "ui_timedialog.h"

class Palette;

//---------------------------------------------------------
//   TimeDialog
//---------------------------------------------------------

class TimeDialog : public QWidget, Ui::TimeDialogBase {
      Q_OBJECT

      Palette* sp;

   private slots:
      void addClicked();

   public:
      TimeDialog(QWidget* parent = 0);
      };

#endif
