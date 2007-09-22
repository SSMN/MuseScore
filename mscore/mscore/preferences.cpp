//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: preferences.cpp,v 1.36 2006/04/06 13:03:11 wschweer Exp $
//
//  Copyright (C) 2002-2007 Werner Schweer and others
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

#include "config.h"
#include "xml.h"
#include "score.h"
#include "mscore.h"
#include "preferences.h"
#include "synti.h"
#include "seq.h"
#include "note.h"
#include "playpanel.h"
#include "pad.h"
#include "icons.h"
#include "shortcutcapturedialog.h"
#include "canvas.h"
#include "sym.h"
#include "palette.h"
#include "pa.h"

extern void writeShortcuts();
extern void readShortcuts();

bool useALSA = false, useJACK = false, usePortaudio = false;

QString appStyleSheet(
      "Pad *             { background-color: rgb(220, 220, 220) }\n"
      "PaletteBoxButton  { background-color: rgb(215, 215, 215) }\n"
      "PaletteBox        { background-color: rgb(230, 230, 230) }\n"
      "PlayPanel QLabel#posLabel { font-size: 28pt; font-family: \"Arial black\" }\n"
      "PlayPanel QLabel#tempoLabel { font-size: 10pt; font-family: \"Arial black\" }\n"
      "PlayPanel QLabel#relTempo { font-size: 10pt; font-family: \"Arial black\" }\n"
      );

//---------------------------------------------------------
//   buttons2stemDir
//    convert checked button to StemDirection
//---------------------------------------------------------

static Direction buttons2stemDir(QRadioButton *up, QRadioButton *down)
      {
      if (up->isChecked()) {
            return UP;
            }
      else if (down->isChecked()) {
            return DOWN;
            }
      else {
            return AUTO;                // default to "auto"
            }
      }

//---------------------------------------------------------
//   stemDir2button
//    convert StemDirection to checked button
//---------------------------------------------------------

static void stemDir2button(Direction dir, QRadioButton *up, QRadioButton *down, QRadioButton *aut)
      {
      if (dir == UP) {
            up->setChecked(true);
            }
      else if (dir == DOWN) {
            down->setChecked(true);
            }
      else {
            aut->setChecked(true);      // default to "auto"
            }
      }

//---------------------------------------------------------
//   Preferences
//---------------------------------------------------------

Preferences preferences;

Preferences::Preferences()
      {
      selectColor[0] = Qt::blue;
      selectColor[1] = Qt::green;
      selectColor[2] = Qt::yellow;
      selectColor[3] = Qt::magenta;
      dropColor      = Qt::red;

      // set fallback defaults:

      cursorBlink        = false;
      fgUseColor         = false;
      bgUseColor         = true;
      fgWallpaper        = ":/data/paper3.png";
      enableMidiInput    = true;
      playNotes          = true;
      bgColor.setRgb(0x76, 0x76, 0x6e);
      fgColor.setRgb(50, 50, 50);
      lPort              = "";
      rPort              = "";
      stemDir[0]         = AUTO;
      stemDir[1]         = AUTO;
      stemDir[2]         = AUTO;
      stemDir[3]         = AUTO;
      showNavigator      = true;
      showPlayPanel      = false;
      showStatusBar      = true;
      showPad            = false;
      padPos             = QPoint(100, 100);
      playPanelPos       = QPoint(100, 300);
#ifdef __MINGW32__
      useAlsaAudio       = false;
      useJackAudio       = false;
      usePortaudioAudio  = true;
#else
      useAlsaAudio       = true;
      useJackAudio       = false;
      usePortaudioAudio  = false;
#endif
      alsaDevice         = "default";
      alsaSampleRate     = 48000;
      alsaPeriodSize     = 1024;
      alsaFragments      = 3;
      portaudioDevice    = -1;
      soundFont          = ":/data/piano1.sf2";
      layoutBreakColor   = Qt::green;
      antialiasedDrawing = true;
      sessionStart       = SCORE_SESSION;
      startScore         = ":/data/demo.msc";
      imagePath          = "~/mscore/images";
      showSplashScreen   = true;
      rewind.type        = -1;
      rewind.type        = -1;
      play.type          = -1;
      stop.type          = -1;
      len1.type          = -1;
      len2.type          = -1;
      len4.type          = -1;
      len8.type          = -1;
      len16.type         = -1;
      len32.type         = -1;
      len3.type          = -1;
      len6.type          = -1;
      len12.type         = -1;
      len24.type         = -1;
      };

//---------------------------------------------------------
//   write
//---------------------------------------------------------

void Preferences::write()
      {
      dirty = false;
      QSettings s;

      s.setValue("cursorBlink",     cursorBlink);
      s.setValue("fgUseColor",      fgUseColor);
      s.setValue("bgUseColor",      bgUseColor);
      s.setValue("fgColor",         fgColor);
      s.setValue("fgWallpaper",     fgWallpaper);
      s.setValue("bgColor",         bgColor);
      s.setValue("bgWallpaper",     bgWallpaper);
      s.setValue("selectColor1",    selectColor[0]);
      s.setValue("selectColor2",    selectColor[1]);
      s.setValue("selectColor3",    selectColor[2]);
      s.setValue("selectColor4",    selectColor[3]);
      s.setValue("enableMidiInput", enableMidiInput);
      s.setValue("playNotes",       playNotes);
      s.setValue("lPort",           lPort);
      s.setValue("rPort",           rPort);
      s.setValue("soundFont",       soundFont);
      s.setValue("stemDirection1",  stemDir[0]);
      s.setValue("stemDirection2",  stemDir[1]);
      s.setValue("stemDirection3",  stemDir[2]);
      s.setValue("stemDirection4",  stemDir[3]);
      s.setValue("showNavigator",   showNavigator);
      s.setValue("showStatusBar",   showStatusBar);
      s.setValue("showPlayPanel",   showPlayPanel);

      s.setValue("showPad",            showPad);
      s.setValue("useAlsaAudio",       useAlsaAudio);
      s.setValue("useJackAudio",       useJackAudio);
      s.setValue("usePortaudioAudio",  usePortaudioAudio);
      s.setValue("alsaDevice",         alsaDevice);
      s.setValue("alsaSampleRate",     alsaSampleRate);
      s.setValue("alsaPeriodSize",     alsaPeriodSize);
      s.setValue("alsaFragments",      alsaFragments);
      s.setValue("portaudioDevice",    portaudioDevice);
      s.setValue("layoutBreakColor",   layoutBreakColor);
      s.setValue("antialiasedDrawing", antialiasedDrawing);
      s.setValue("imagePath",          imagePath);
      s.setValue("showSplashScreen",   showSplashScreen);
      switch(sessionStart) {
            case LAST_SESSION:   s.setValue("sessionStart", "last"); break;
            case NEW_SESSION:    s.setValue("sessionStart", "new"); break;
            case SCORE_SESSION:  s.setValue("sessionStart", "score"); break;
            }
      s.setValue("startScore", startScore);

      s.beginGroup("PlayPanel");
      s.setValue("pos", playPanelPos);
      s.endGroup();

      s.beginGroup("KeyPad");
      s.setValue("pos", padPos);
      s.endGroup();

      writeShortcuts();
      }

//---------------------------------------------------------
//   read
//---------------------------------------------------------

void Preferences::read()
      {
      QSettings s;

      cursorBlink     = s.value("cursorBlink", false).toBool();
      fgUseColor      = s.value("fgUseColor", false).toBool();
      bgUseColor      = s.value("bgUseColor", true).toBool();
      fgColor         = s.value("fgColor", QColor(50,50,50)).value<QColor>();
      fgWallpaper     = s.value("fgWallpaper", ":/data/paper3.png").toString();
      bgColor         = s.value("bgColor", QColor(0x76, 0x76, 0x6e)).value<QColor>();
      bgWallpaper     = s.value("bgWallpaper").toString();
      selectColor[0]  = s.value("selectColor1", QColor(Qt::blue)).value<QColor>();
      selectColor[1]  = s.value("selectColor2", QColor(Qt::green)).value<QColor>();
      selectColor[2]  = s.value("selectColor3", QColor(Qt::yellow)).value<QColor>();
      selectColor[3]  = s.value("selectColor4", QColor(Qt::magenta)).value<QColor>();
      enableMidiInput = s.value("enableMidiInput", true).toBool();
      playNotes       = s.value("playNotes", true).toBool();
      lPort           = s.value("lPort").toString();
      rPort           = s.value("rPort").toString();
      soundFont       = s.value("soundFont", ":/data/piano1.sf2").toString();
      stemDir[0]      = (Direction)s.value("stemDirection1", AUTO).toInt();
      stemDir[1]      = (Direction)s.value("stemDirection2", AUTO).toInt();
      stemDir[2]      = (Direction)s.value("stemDirection3", AUTO).toInt();
      stemDir[3]      = (Direction)s.value("stemDirection4", AUTO).toInt();
      showNavigator   = s.value("showNavigator", true).toBool();
      showStatusBar   = s.value("showStatusBar", true).toBool();
      showPlayPanel   = s.value("showPlayPanel", false).toBool();

      showPad            = s.value("showPad", false).toBool();
      useAlsaAudio       = s.value("useAlsaAudio", true).toBool();
      useJackAudio       = s.value("useJackAudio", false).toBool();
      usePortaudioAudio  = s.value("usePortaudioAudio", false).toBool();
      alsaDevice         = s.value("alsaDevice", "default").toString();
      alsaSampleRate     = s.value("alsaSampleRate", 48000).toInt();
      alsaPeriodSize     = s.value("alsaPeriodSize", 1024).toInt();
      alsaFragments      = s.value("alsaFragments", 3).toInt();
      portaudioDevice    = s.value("portaudioDevice", -1).toInt();
      layoutBreakColor   = s.value("layoutBreakColor", QColor(Qt::green)).value<QColor>();
      antialiasedDrawing = s.value("antialiasedDrawing", true).toBool();
      imagePath          = s.value("imagePath", "~/mscore/images").toString();
      showSplashScreen   = s.value("showSplashScreen", true).toBool();

      QString ss(s.value("sessionStart", "score").toString());
      if (ss == "last")
            sessionStart = LAST_SESSION;
      else if (ss == "new")
            sessionStart = NEW_SESSION;
      else if (ss == "score")
            sessionStart = SCORE_SESSION;

      startScore         = ":/data/demo.msc";
      startScore = s.value("startScore", ":/data/demo.msc").toString();

      s.beginGroup("PlayPanel");
      playPanelPos = s.value("pos", QPoint(100, 300)).toPoint();
      s.endGroup();

      s.beginGroup("KeyPad");
      padPos = s.value("pos", QPoint(100, 300)).toPoint();
      s.endGroup();

      readShortcuts();
      }

//---------------------------------------------------------
//   preferences
//---------------------------------------------------------

void MuseScore::startPreferenceDialog()
      {
      if (!preferenceDialog) {
            preferenceDialog = new PreferenceDialog(this);
            connect(preferenceDialog, SIGNAL(preferencesChanged()),
               SLOT(preferencesChanged()));
            }
      preferenceDialog->show();
      }

//---------------------------------------------------------
//   PreferenceDialog
//---------------------------------------------------------

PreferenceDialog::PreferenceDialog(QWidget* parent)
   : QDialog(parent)
      {
      setupUi(this);

#ifndef USE_JACK
      jackDriver->setEnabled(false);
#endif
#ifndef USE_ALSA
      alsaDriver->setEnabled(false);
#endif
#ifndef USE_PORTAUDIO
      portaudioDriver->setEnabled(false);
#endif
      connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonBoxClicked(QAbstractButton*)));
      cursorBlink->setChecked(preferences.cursorBlink);

      QButtonGroup* fgButtons = new QButtonGroup(this);
      fgButtons->setExclusive(true);
      fgButtons->addButton(fgColorButton);
      fgButtons->addButton(fgWallpaperButton);
      connect(fgColorButton, SIGNAL(toggled(bool)), SLOT(fgClicked(bool)));

      QButtonGroup* bgButtons = new QButtonGroup(this);
      bgButtons->setExclusive(true);
      bgButtons->addButton(bgColorButton);
      bgButtons->addButton(bgWallpaperButton);
      connect(bgColorButton, SIGNAL(toggled(bool)), SLOT(bgClicked(bool)));

      fgWallpaper->setText(preferences.fgWallpaper);
      bgWallpaper->setText(preferences.bgWallpaper);

      fgColorLabel->setColor(preferences.fgColor);
      bgColorLabel->setColor(preferences.bgColor);

      selectColorLabel1->setColor(preferences.selectColor[0]);
      selectColorLabel2->setColor(preferences.selectColor[1]);
      selectColorLabel3->setColor(preferences.selectColor[2]);
      selectColorLabel4->setColor(preferences.selectColor[3]);

      bgColorButton->setChecked(preferences.bgUseColor);
      bgWallpaperButton->setChecked(!preferences.bgUseColor);
      fgColorButton->setChecked(preferences.fgUseColor);
      fgWallpaperButton->setChecked(!preferences.fgUseColor);

      enableMidiInput->setChecked(preferences.enableMidiInput);
      playNotes->setChecked(preferences.playNotes);

      if (!preferences.soundFont.isEmpty())
            soundFont->setText(preferences.soundFont);
      else {
            const char* p = getenv("DEFAULT_SOUNDFONT");
            soundFont->setText(QString(p ? p : ""));
            }

      if (seq->isRunning()) {
            std::list<QString> sl = seq->inputPorts();
            int idx = 0;
            for (std::list<QString>::iterator i = sl.begin(); i != sl.end(); ++i, ++idx) {
                  jackRPort->addItem(*i);
                  jackLPort->addItem(*i);
                  if (preferences.rPort == *i)
                        jackRPort->setCurrentIndex(idx);
                  if (preferences.lPort == *i)
                        jackLPort->setCurrentIndex(idx);
                  }
            }
      else {
            jackRPort->setEnabled(false);
            jackLPort->setEnabled(false);
            }

      stemDir2button(preferences.stemDir[0], upRadioButton1, downRadioButton1, autoRadioButton1);
      stemDir2button(preferences.stemDir[1], upRadioButton2, downRadioButton2, autoRadioButton2);
      stemDir2button(preferences.stemDir[2], upRadioButton3, downRadioButton3, autoRadioButton3);
      stemDir2button(preferences.stemDir[3], upRadioButton4, downRadioButton4, autoRadioButton4);

      navigatorShow->setChecked(preferences.showNavigator);
      keyPadShow->setChecked(preferences.showPad);
      playPanelShow->setChecked(preferences.showPlayPanel);
      keyPadX->setValue(preferences.padPos.x());
      keyPadY->setValue(preferences.padPos.y());
      playPanelX->setValue(preferences.playPanelPos.x());
      playPanelY->setValue(preferences.playPanelPos.y());

      alsaDriver->setChecked(preferences.useAlsaAudio);
      jackDriver->setChecked(preferences.useJackAudio);
      portaudioDriver->setChecked(preferences.usePortaudioAudio);
      alsaDevice->setText(preferences.alsaDevice);

      int index = alsaSampleRate->findText(QString("%1").arg(preferences.alsaSampleRate));
      alsaSampleRate->setCurrentIndex(index);
      index = alsaPeriodSize->findText(QString("%1").arg(preferences.alsaPeriodSize));
      alsaPeriodSize->setCurrentIndex(index);

      alsaFragments->setValue(preferences.alsaFragments);
      drawAntialiased->setChecked(preferences.antialiasedDrawing);
      switch(preferences.sessionStart) {
            case LAST_SESSION:   lastSession->setChecked(true); break;
            case NEW_SESSION:    newSession->setChecked(true); break;
            case SCORE_SESSION:  scoreSession->setChecked(true); break;
            }
      sessionScore->setText(preferences.startScore);
      imagePath->setText(preferences.imagePath);
      showSplashScreen->setChecked(preferences.showSplashScreen);

      //
      // initialize local shortcut table
      //    we need a deep copy to be able to rewind all
      //    changes on "Abort"
      //
      foreach(Shortcut* s, shortcuts) {
            Shortcut* ns = new Shortcut(*s);
            ns->action = 0;
            localShortcuts[s->xml] = ns;
            }
      updateSCListView();


      //
      // initialize portaudio
      //
      if (usePortaudio) {
            Portaudio* audio = (Portaudio*)seq->audioDriver();
            QStringList apis = audio->apiList();
            portaudioApi->addItems(apis);
            QStringList devices = audio->deviceList(0);
            portaudioDevice->addItems(devices);
            connect(portaudioApi, SIGNAL(activated(int)), SLOT(portaudioApiActivated(int)));
            }

      connect(fgColorSelect,      SIGNAL(clicked()), SLOT(selectFgColor()));
      connect(bgColorSelect,      SIGNAL(clicked()), SLOT(selectBgColor()));
      connect(selectColorSelect1, SIGNAL(clicked()), SLOT(selectSelectColor1()));
      connect(selectColorSelect2, SIGNAL(clicked()), SLOT(selectSelectColor2()));
      connect(selectColorSelect3, SIGNAL(clicked()), SLOT(selectSelectColor3()));
      connect(selectColorSelect4, SIGNAL(clicked()), SLOT(selectSelectColor4()));
      connect(fgWallpaperSelect,  SIGNAL(clicked()), SLOT(selectFgWallpaper()));
      connect(bgWallpaperSelect,  SIGNAL(clicked()), SLOT(selectBgWallpaper()));
      connect(sfButton, SIGNAL(clicked()), SLOT(selectSoundFont()));
      connect(imagePathButton, SIGNAL(clicked()), SLOT(selectImagePath()));
      sfChanged = false;

      connect(playPanelCur, SIGNAL(clicked()), SLOT(playPanelCurClicked()));
      connect(keyPadCur, SIGNAL(clicked()), SLOT(padCurClicked()));

      connect(shortcutList, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(defineShortcutClicked()));
      connect(resetShortcut, SIGNAL(clicked()), SLOT(resetShortcutClicked()));
      connect(clearShortcut, SIGNAL(clicked()), SLOT(clearShortcutClicked()));
      connect(defineShortcut, SIGNAL(clicked()), SLOT(defineShortcutClicked()));
      }

//---------------------------------------------------------
//   portaudioApiActivated
//---------------------------------------------------------

void PreferenceDialog::portaudioApiActivated(int idx)
      {
      Portaudio* audio = (Portaudio*)seq->audioDriver();
      QStringList devices = audio->deviceList(idx);
      portaudioDevice->clear();
      portaudioDevice->addItems(devices);
      }

//---------------------------------------------------------
//   updateSCListView
//---------------------------------------------------------

void PreferenceDialog::updateSCListView()
      {
      shortcutList->clear();
      QTreeWidgetItem* stateNormal = new QTreeWidgetItem;
      stateNormal->setFlags(Qt::ItemIsEnabled);
      stateNormal->setText(0, tr("Normal State"));
      stateNormal->setExpanded(true);
      shortcutList->addTopLevelItem(stateNormal);
      QTreeWidgetItem* statePlay = new QTreeWidgetItem;
      statePlay->setFlags(Qt::ItemIsEnabled);
      statePlay->setText(0, tr("Play State"));
      statePlay->setExpanded(true);
      shortcutList->addTopLevelItem(statePlay);

      foreach (Shortcut* s, localShortcuts) {
            if (!s)
                  continue;
            QTreeWidgetItem* newItem;
            if (s->state & STATE_NORMAL)
                  newItem = new QTreeWidgetItem(stateNormal);
            else if (s->state & STATE_PLAY)
                  newItem = new QTreeWidgetItem(statePlay);
            else {
                  printf("PreferenceDialog::updateSCListView(): illegal shortcut state\n");
                  return;
                  }
            newItem->setText(0, s->descr);
            newItem->setIcon(0, *s->icon);
            QKeySequence seq = s->key;
            newItem->setText(1, s->key.toString(QKeySequence::NativeText));
            newItem->setData(0, Qt::UserRole, s->xml);
            }
      shortcutList->resizeColumnToContents(0);
      }

//---------------------------------------------------------
//   resetShortcutClicked
//    reset all shortcuts to buildin defaults
//---------------------------------------------------------

void PreferenceDialog::resetShortcutClicked()
      {
      printf("resetShortcutClicked\n");
      }

//---------------------------------------------------------
//   clearShortcutClicked
//---------------------------------------------------------

void PreferenceDialog::clearShortcutClicked()
      {
      QTreeWidgetItem* active = shortcutList->currentItem();
      if (active) {
            QString str = active->data(0, Qt::UserRole).toString();
            if (!str.isEmpty()) {
                  Shortcut* s = localShortcuts[str];
                  s->key = 0;
                  active->setText(1, s->key.toString(QKeySequence::NativeText));
                  }
            }
      }

//---------------------------------------------------------
//   defineShortcutClicked
//---------------------------------------------------------

void PreferenceDialog::defineShortcutClicked()
      {
      QTreeWidgetItem* active = shortcutList->currentItem();
      QString str = active->data(0, Qt::UserRole).toString();
      if (str.isEmpty())
            return;
      Shortcut* s = localShortcuts[str];
      ShortcutCaptureDialog sc(s, this);
      if (sc.exec()) {
            s->key = sc.getKey();
            active->setText(1, s->key.toString(QKeySequence::NativeText));
            shortcutsChanged = true;
            }
//      clearButton->setEnabled(true);
      }

//---------------------------------------------------------
//   selectFgColor
//---------------------------------------------------------

void PreferenceDialog::selectFgColor()
      {
      QColor c = QColorDialog::getColor(fgColorLabel->color(), this);
      if (c.isValid())
            fgColorLabel->setColor(c);
      }

//---------------------------------------------------------
//   selectBgColor
//---------------------------------------------------------

void PreferenceDialog::selectBgColor()
      {
      QColor c = QColorDialog::getColor(bgColorLabel->color(), this);
      if (c.isValid())
            bgColorLabel->setColor(c);
      }

//---------------------------------------------------------
//   selectSelectColor1
//---------------------------------------------------------

void PreferenceDialog::selectSelectColor1()
      {
      QColor c = QColorDialog::getColor(selectColorLabel1->color(), this);
      if (c.isValid())
            selectColorLabel1->setColor(c);
      }

//---------------------------------------------------------
//   selectSelectColor2
//---------------------------------------------------------

void PreferenceDialog::selectSelectColor2()
      {
      QColor c = QColorDialog::getColor(selectColorLabel2->color(), this);
      if (c.isValid())
            selectColorLabel2->setColor(c);
      }

//---------------------------------------------------------
//   selectSelectColor3
//---------------------------------------------------------

void PreferenceDialog::selectSelectColor3()
      {
      QColor c = QColorDialog::getColor(selectColorLabel3->color(), this);
      if (c.isValid())
            selectColorLabel3->setColor(c);
      }

//---------------------------------------------------------
//   selectSelectColor4
//---------------------------------------------------------

void PreferenceDialog::selectSelectColor4()
      {
      QColor c = QColorDialog::getColor(selectColorLabel4->color(), this);
      if (c.isValid())
            selectColorLabel4->setColor(c);
      }

//---------------------------------------------------------
//   selectFgWallpaper
//---------------------------------------------------------

void PreferenceDialog::selectFgWallpaper()
      {
      QString s = QFileDialog::getOpenFileName(
         this,                            // parent
         tr("Choose Notepaper"),          // caption
         fgWallpaper->text(),             // dir
         tr("Images (*.jpg *.gif *.png)") // filter
         );
      if (!s.isNull())
            fgWallpaper->setText(s);
      }

//---------------------------------------------------------
//   selectBgWallpaper
//---------------------------------------------------------

void PreferenceDialog::selectBgWallpaper()
      {
      QString s = QFileDialog::getOpenFileName(
         this,
         tr("Choose Background Wallpaper"),
         bgWallpaper->text(),
         tr("Images (*.jpg *.gif *.png)")
         );
      if (!s.isNull())
            bgWallpaper->setText(s);
      }

//---------------------------------------------------------
//   selectSoundFont
//---------------------------------------------------------

void PreferenceDialog::selectSoundFont()
      {
      QString s = QFileDialog::getOpenFileName(
         this,
         tr("Choose Synthesizer Sound Font"),
         soundFont->text(),
         tr("Sound Fonds (*.sf2 *.SF2);;All (*)")
         );
      if (!s.isNull()) {
            sfChanged = soundFont->text() != s;
            soundFont->setText(s);
            }
      }

//---------------------------------------------------------
//   selectImagePath
//---------------------------------------------------------

void PreferenceDialog::selectImagePath()
      {
      QString s = QFileDialog::getExistingDirectory(
         this,
         tr("Choose Image Path"),
         imagePath->text()
         );
      if (!s.isNull())
            imagePath->setText(s);
      }

//---------------------------------------------------------
//   fgClicked
//---------------------------------------------------------

void PreferenceDialog::fgClicked(bool id)
      {
      fgColorLabel->setEnabled(id);
      fgColorSelect->setEnabled(id);
      fgWallpaper->setEnabled(!id);
      fgWallpaperSelect->setEnabled(!id);
      }

//---------------------------------------------------------
//   bgClicked
//---------------------------------------------------------

void PreferenceDialog::bgClicked(bool id)
      {
      bgColorLabel->setEnabled(id);
      bgColorSelect->setEnabled(id);
      bgWallpaper->setEnabled(!id);
      bgWallpaperSelect->setEnabled(!id);
      }

//---------------------------------------------------------
//   buttonBoxClicked
//---------------------------------------------------------

void PreferenceDialog::buttonBoxClicked(QAbstractButton* button)
      {
      switch(buttonBox->standardButton(button)) {
            case QDialogButtonBox::Apply:
                  apply();
                  break;
            case QDialogButtonBox::Ok:
                  apply();
            case QDialogButtonBox::Cancel:
            default:
                  hide();
                  break;
            }
      }

//---------------------------------------------------------
//   apply
//---------------------------------------------------------

void PreferenceDialog::apply()
      {
      preferences.selectColor[0] = selectColorLabel1->color();
      preferences.selectColor[1] = selectColorLabel2->color();
      preferences.selectColor[2] = selectColorLabel3->color();
      preferences.selectColor[3] = selectColorLabel4->color();

      preferences.cursorBlink = cursorBlink->isChecked();
      preferences.fgWallpaper = fgWallpaper->text();
      preferences.bgWallpaper = bgWallpaper->text();

      preferences.fgColor = fgColorLabel->color();
      preferences.bgColor = bgColorLabel->color();

      preferences.bgUseColor  = bgColorButton->isChecked();
      preferences.fgUseColor  = fgColorButton->isChecked();
      preferences.enableMidiInput = enableMidiInput->isChecked();
      preferences.playNotes   = playNotes->isChecked();
      preferences.soundFont   = soundFont->text();
      if (preferences.lPort != jackLPort->currentText()
         || preferences.rPort != jackRPort->currentText()) {
            // TODO: change ports
            preferences.lPort       = jackLPort->currentText();
            preferences.rPort       = jackRPort->currentText();
            }
      preferences.stemDir[0] = buttons2stemDir(upRadioButton1, downRadioButton1);
      preferences.stemDir[1] = buttons2stemDir(upRadioButton2, downRadioButton2);
      preferences.stemDir[2] = buttons2stemDir(upRadioButton3, downRadioButton3);
      preferences.stemDir[3] = buttons2stemDir(upRadioButton4, downRadioButton4);

      preferences.showNavigator  = navigatorShow->isChecked();
      preferences.showPad        = keyPadShow->isChecked();
      preferences.showPlayPanel  = playPanelShow->isChecked();
      preferences.padPos         = QPoint(keyPadX->value(), keyPadY->value());
      preferences.playPanelPos   = QPoint(playPanelX->value(), playPanelY->value());

      preferences.useAlsaAudio       = alsaDriver->isChecked();
      preferences.useJackAudio       = jackDriver->isChecked();
      preferences.usePortaudioAudio  = portaudioDriver->isChecked();
      preferences.alsaDevice         = alsaDevice->text();
      preferences.alsaSampleRate     = alsaSampleRate->currentText().toInt();
      preferences.alsaPeriodSize     = alsaPeriodSize->currentText().toInt();
      preferences.alsaFragments      = alsaFragments->value();
      preferences.antialiasedDrawing = drawAntialiased->isChecked();
      Portaudio* audio = (Portaudio*)seq->audioDriver();

      preferences.portaudioDevice = audio->deviceIndex(portaudioApi->currentIndex(),
         portaudioDevice->currentIndex());

      if (lastSession->isChecked())
            preferences.sessionStart = LAST_SESSION;
      else if (newSession->isChecked())
            preferences.sessionStart = NEW_SESSION;
      else if (scoreSession->isChecked())
            preferences.sessionStart = SCORE_SESSION;
      preferences.startScore         = sessionScore->text();
      preferences.imagePath          = imagePath->text();
      preferences.showSplashScreen   = showSplashScreen->isChecked();

      if (shortcutsChanged) {
            shortcutsChanged = false;
            foreach(Shortcut* s, localShortcuts) {
                  Shortcut* os = shortcuts[s->xml];
                  if (os->key != s->key) {
                        os->key = s->key;
                        if (os->action)
                              os->action->setShortcut(s->key);
                        }
                  }
            }
      emit preferencesChanged();
      preferences.write();
      if (sfChanged) {
            if (!seq->isRunning()) {
                  // try to start sequencer
                  seq->init();
                  }
            if (seq->isRunning()) {
                  sfChanged = false;
                  seq->loadSoundFont(preferences.soundFont);
                  }
            }
      }

//---------------------------------------------------------
//   playPanelCurClicked
//---------------------------------------------------------

void PreferenceDialog::playPanelCurClicked()
      {
      PlayPanel* w = mscore->getPlayPanel();
      if (w == 0)
            return;
      QPoint s(w->pos());
      playPanelX->setValue(s.x());
      playPanelY->setValue(s.y());
      }

//---------------------------------------------------------
//   padCurClicked
//---------------------------------------------------------

void PreferenceDialog::padCurClicked()
      {
      Pad* w = mscore->getKeyPad();
      if (w == 0)
            return;
      QPoint s(w->pos());
      keyPadX->setValue(s.x());
      keyPadY->setValue(s.y());
      }

//---------------------------------------------------------
//   writeShortcuts
//---------------------------------------------------------

void writeShortcuts()
      {
      QSettings s;
      s.beginGroup("Shortcuts");

      int n = 0;
      foreach(Shortcut* shortcut, shortcuts) {
            for (unsigned i = 0;; ++i) {
                  if (MuseScore::sc[i].xml == shortcut->xml) {
                        if (MuseScore::sc[i].key != shortcut->key) {
                              QString tag("sc[%1]");
                              s.setValue(tag.arg(n), shortcut->xml);
                              tag = "seq[%1]";
                              s.setValue(tag.arg(n), shortcut->key.toString(QKeySequence::PortableText));
                              ++n;
                              }
                        break;
                        }
                  }
            }
      s.setValue("n", n);
      s.endGroup();
      }

//---------------------------------------------------------
//   readShortcuts
//---------------------------------------------------------

void readShortcuts()
      {
      QSettings s;
      s.beginGroup("Shortcuts");
      int n = s.value("n", 0).toInt();

      for (int i = 0; i < n; ++i) {
            QString tag("sc[%1]");
            QString name = s.value(tag.arg(i)).toString();
            tag = "seq[%1]";
            QString seq = s.value(tag.arg(i)).toString();
            Shortcut* s = shortcuts.value(name);
            if (s)
                  s->key = QKeySequence::fromString(seq, QKeySequence::PortableText);
            else
                  printf("MuseScore:readShortCuts: unknown tag <%s>\n", qPrintable(name));
            }
      s.endGroup();
      }

//---------------------------------------------------------
//   getShortcut
//---------------------------------------------------------

Shortcut* getShortcut(const char* id)
      {
      Shortcut* s = shortcuts.value(id);
      if (s == 0) {
            printf("internal error: shortcut <%s> not found\n", id);
            return 0;
            }
      return s;
      }

//---------------------------------------------------------
//   getAction
//    returns action for shortcut
//---------------------------------------------------------

QAction* getAction(const char* id)
      {
      Shortcut* s = getShortcut(id);
      if (s == 0)
            return 0;
      if (s->action == 0) {
            QAction* a = new QAction(s->xml, mscore);
            s->action  = a;
            a->setData(s->xml);
            a->setShortcut(s->key);
            a->setShortcutContext(s->context);
            if (!s->help.isEmpty()) {
                  a->setToolTip(s->help);
                  a->setWhatsThis(s->help);
                  }
            else {
                  a->setToolTip(s->descr);
                  a->setWhatsThis(s->descr);
                  }
            if (!s->text.isEmpty())
                  a->setText(s->text);
            if (s->icon)
                  a->setIcon(*s->icon);
            }
      return s->action;
      }

