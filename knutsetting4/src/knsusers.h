/***************************************************************************
                          knsusers.h  -  description
                             -------------------
    begin                : Po srp 16 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel at Prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSUSERS_H
#define KNSUSERS_H

#include "knscommondata.h"
#include "knsuseredit.h"
#include "knsstring.h"

#include <KDE/KPushButton> 

#include <KDE/KIcon>

#include <QtGui/QLabel>
#include <QtCore/QList>


class QWidget;
class QString;
class QPixmap;
class QLayout;
class QListWidget;
class QIistWidgetItem;

/**
  *@author Daniel Prynych,,,
  */
class KNSUsers : public QLabel  {
   Q_OBJECT
public: 
  KNSUsers(KNSCommonData* commonData, QWidget *parent=0);
  ~KNSUsers();

/**
 * Natahne seznam uzivatelu do iconView z commonData.
 * Load list of users to iconview from commonData.
 *
 * @since  0.1
 **/
  void loadItems(void);



/**
 * Hides/Shows/Enables/Disables buttons.
 * If mainButtons is true, Add, Edit and Delete buttons
 * are showed, and Ok and Cancel buttons are hided;
 * Skryje/Ukaze/Aktivuje/Dezaktivuje tlacitka.
 * Jestlize mainButtons je true Pridej, Oprav a Smaz tlacitka
 * jsou zobrazena a OK a Cancel tlacitka jsou skryta.
 *
 * @since  0.1
 **/
void setMainButtons(bool mainButtons = true);



/**
 * Hides/Shows/Enables/Disables buttons.
 * If okCancelButtons is true, Add, Edit and Delete buttons
 * are hided, and Ok and Cancel buttons are showed;
 * Skryje/Ukaze/Aktivuje/Dezaktivuje tlacitka.
 * Jestlize okCancelButtons je true Pridej, Oprav a Smaz tlacitka
 * jsou skryta a OK a Cancel tlacitka jsou zobrazena.
 *
 * @since  0.1
 **/
void setOkCancelButtons(bool okCancelButtons = true);



public slots:

/**
 * Nastavi zakladni nastaveni.
 * Sets default setting
 *
 * @since  0.1
 **/
void setStandardSlot (void);


signals:
  void changed (void);
  void userDeleted (const QString userName);
  void userAdded (const QString userName);

/**
 * Signal is emited when pasword was changed.
 * Signal je emitovan kdyz bylo zmeneno heslo.
 **/
  void passwordChanged (const QString userName, const QString password);  

/**
 * Signal is emited when typ of monitoring (Master/Slave) was changed.
 * Signal je emitovan kdyz byl zmenen typ sledovani (Master/Slave).
 **/
  void monitoringChanged (const QString userName, const typeOfMonitoring typeMon);


/**
 * Signal is emited when typ of section (User/Monitoring) was changed.
 * Signal je emitovan kdyz byl zmenen typ sekce (Uzivatel/Sledovani).
 **/
  void typeUserChanged (const QString userName, const bool isUser);


private slots:


void nameAndPasswordAreEmptySlot (const bool areEmpty);
/**
 * Enables OKButton when Name and Password aren't empty 
 *
 **/

/**
 * Nastavi editacni prvky pro vybranou sekci.
 * Sets items of edit for choosen section
 *
 * @param item urcuje odkaz na iconu sekce.
 * @param item determine pointer to icon of section.
 *
 * @since  0.1
 **/
void iconSelectedSlot( void );

/**
 * Prida dalsi sekci.
 * Add other section
 *
 * @since  0.1
 **/
void addItemSlot(void);


/**
 * Opravi vybranou sekci.
 * Edit selected section
 *
 * @since  0.1
 **/
void editItemSlot(void);


/**
 * Smaze vybranou sekci.
 * Delete selected section
 *
 * @since  0.1
 **/
void deleteItemSlot(void);



/**
 * Odmitne pridani/opravu sekce.
 * Reject add/edit section
 *
 * @since  0.1
 **/
void cancelItemSlot(void);


/**
 * Potvrdi pridani/opravu sekce.
 * Commit add/edit section
 *
 * @since  0.1
 **/
void okItemSlot(void);



private:

    enum typeAction {
    noneAction =0,
    addAction =1,
    editAction = 2,
    deleteAction =3
    };

  typeAction m_action;


  usersPermList::iterator findName(QString name);

  QString findLocalhost (void);


  KNSCommonData* m_commonData;
  bool m_enableButton;

  QListWidget* m_iconWidget;
  KNSUserEdit* m_itemWidget;

  usersPermList* m_usersData;

  QString m_oldPassword;
  typeOfMonitoring m_oldTypeMonitoring;
  bool m_oldIsUser;

  KIcon m_userPix;
  KIcon m_monitorPix;

  KPushButton* m_addButton;
  KPushButton* m_editButton;
  KPushButton* m_deleteButton;
  KPushButton* m_okButton;
  KPushButton* m_cancelButton;

  KPushButton* m_standardButton;

  // iterator for selected record's item
  usersPermList::const_iterator m_activeItemIt;
};

#endif
