/***************************************************************************
                          knsmonnotify.h  -  description
                             -------------------
    begin                : So ?íj 23 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Danie at prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSMONNOTIFY_H
#define KNSMONNOTIFY_H

#include "knscommondata.h"

#include <klineedit.h>
#include <kpushbutton.h>
#include <ktextedit.h> 

#include <qwidget.h>

/**
  *@author Daniel Prynych,,,
  */

enum notifyCode { online, onbatt, lowbatt, fsb, commok, commbat, shutdown, replbatt, nocomm };


static const char* notifyName[] =
   {"ONLINE","ONBATT","LOWBATT.","FSB","COMMOK","COMMBAD","SHUTDOWN","REPLBATT","NOCOMM"};

static const int notifyNamesCount = 9;

class QCheckBox;
class QListWidget;
class QListWidgetItem;

class KNSMonNotify : public QWidget  {
   Q_OBJECT
public:
  KNSMonNotify(KNSCommonData* tCommonData, QWidget *parent=0);
  ~KNSMonNotify();

  void clear(void);

public slots:
  void setStandardSlot (void);


signals:
  void changed (void);


private slots:
  void itemListBoxSelectedSlot(QListWidgetItem* item);

/**
 * Opravi vybranou sekci.
 * Edits selected section
 *
 * @since  0.1
 **/
  void editItemSlot(void);



/**
 * Opravi vybranou sekci.
 * Sets default values for selected section
 *
 * @since  0.1
 **/
  void defaultItemSlot(void);


/**
 * Potvrdi pridani/opravu sekce.
 * Commits add/edit section
 *
 * @since  0.1
 **/
  void okItemSlot(void);

/**
 * Odmitne pridani/opravu sekce.
 * Rejects add/edit section
 *
 * @since  0.1
 **/
  void cancelItemSlot(void);

/**
 * Odnastavi ostatni znacky (zatrhavaci boxy) syslog, wall exec.
 * Je aktivovan, kdyz je znacka ignore zatrhnuta.
 * Unsets other flags (checkboxs) syslog wall, exec.
 * It activate when ignore flag is checked,
 *
 * @since 0.1
 **/
  void ignoreFlagSlot( void);

/**
 * Odnastavi znacky ignore (zatrhavaci box).
 * Je aktivovan, kdyz je znacka syslog, wall or exec zatrhnuta.
 * Unsets flag ignore (checkbox).
 * It activate when flags syslog, wall or exec are checked.
 *
 * @since 0.1
 **/
  void otherFlagSlot( void);



private:

/**
 * Inicializuje seznam sekci.
 * Inits list of section (On Line, On battery...).
 *
 * @since 0.1
 **/
  void initItemsListBox();


/**
 * Zobrazi/skryje tlacitka pro vyber nebo potvrzeni/odmitnuti akce.
 * type = true zobrazit tlacitka pro vyber, type = false zobrazi tlacitka pro potvrzeni/odmitnuti.
 * Shows/Hides buttons for select or ok/cancel of action..
 * type = true shows buttons for select, type = false show buttons for ok/cancel
 *
 * @since 0.1
 **/
  void setButtons(bool type);

/**
 * Vymaze polozky bez polozek pro popis.
 * Clears items without items of description;
 *
 * @since 0.1
 **/
//  void clearItems();

/**
 * Nastavi/odnastavi widgety.jen pro cteni.
 * type=true nastavi widgety i pro zapis, type=false nastavi widgety jen pro cteni.
 * Sets/Unsets widgets read only;
 * type=true sets widgets read/write, type=false sets widgtes read only.
 *
 * @since 0.1
 **/
  void setReadOnlyWidgets(bool type);



  void clearItem(void);


  monNotifyItemStruct* m_monNotifyItemsFile;

  bool m_enableButton;

  QListWidget* m_itemsListBox;
  KLineEdit* m_messageEdit;

  QCheckBox* m_syslogFlag;
  QCheckBox* m_wallFlag;
  QCheckBox* m_execFlag;
  QCheckBox* m_ignoreFlag;

  KPushButton* m_editButton;
  KPushButton* m_okButton;
  KPushButton* m_cancelButton;
  KPushButton* m_defaultButton;
  KPushButton* m_standardButton;
  KTextEdit* m_notifyDesc;

};

#endif
