/***************************************************************************
                          knsmodules.h  -  description
                             -------------------
    begin                : ?t ?íj 7 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel at prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSMODULES_H
#define KNSMODULES_H

#include "knscommondata.h"
#include "knsmodulesedit.h"

#include <kpushbutton.h>
#include <KDE/KIcon>
#include <KDE/KPushButton>


#include <QtGui/QLabel>
#include <QtCore/QList>


/**
  *@author Daniel Prynych,,,
  */

class QWidget;
class QPixmap;
class QListWidget;
class QListWidgetItem;

class KNSModules : public QLabel  {
   Q_OBJECT
public: 
  KNSModules(KNSCommonData* commonData, QWidget *parent=0);

 virtual ~KNSModules();

/**
 * Nahraje prvky z commonDate do widgetu grafickeho rozhrani
 * Load items from commonData to GUI widgets
 *
 * @since  0.1
 **/
  void loadItems(void);

signals:
  void changed (void);
  void upsDeleted (const QString upsName);


private slots:
//void iconSelectedSlot( void );
void iconCurrentSlot( QListWidgetItem * newCurrentItem, QListWidgetItem * );
void addItemSlot(void);
void deleteItemSlot(void);
void editItemSlot(void);
void cancelItemSlot(void);
void okItemSlot(void);
void addedNewItemSlot(QString name);

private:

  enum typeAction {
    noneAction =0,
    addAction =1,
    editAction = 2,
    deleteAction =3
    };

/**
 * Aktivuje hlavni sadu tlacitek, kdyz enable je lez aktivuje tlacitka Ok a Cancel
 * Activates main set of buttons, when enable is false activates OK and Cancel buttons
 *
 * @sinec 0.1
 **/


/**
 * Aktivuje druhou sadu tlacitek, kdyz enable je lez aktivuje hlavni sadu tlacitek
 * Activates the second set of buttons, when enable is false activates main set of buttons
 *
 * @sinec 0.1
 **/
  void setMainButtons(bool enable);


  void setOkCancelButtons(bool enable);

//----------- Variables ---------------
  typeAction m_action;


  KNSCommonData* m_commonData;

  bool m_enableButton;

  modulesList* m_modulesData;

  QListWidget* m_iconWidget;

  KIcon m_monitorOnPix;
  KIcon m_monitorOffPix;


  KNSModulesEdit* m_modulesEdit;  


  KPushButton* m_addButton;
  KPushButton* m_editButton;
  KPushButton* m_deleteButton;
  KPushButton* m_okButton;
  KPushButton* m_cancelButton;


};

#endif
