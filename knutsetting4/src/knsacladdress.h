/***************************************************************************
                          knsacladdres.h  -  description
                             -------------------
    begin                : So kv? 8 2004
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

#ifndef KNSACLADDRES_H
#define KNSACLADDRES_H

#include "knscommondata.h"
#include "knsintaddress.h"

#include <KDE/KPushButton>

#include <QtGui/QGroupBox>

class QString;
class QWidget;  
class QTreeWidget;
class QTreeWidgetItem;
  
/**
  *
  *@author Daniel Prynych,,,
  *@since 0.1
  */
class KNSACLAddress : public QGroupBox  {
   Q_OBJECT
public: 
  KNSACLAddress(KNSCommonData* tCommonData, QWidget *parent=0 );
  ~KNSACLAddress();

void loadItems (void);


/**
 *Smaze prvky v QTreeWidget (mainView, list of address).
 *Clears items in QTreeWidget (mainView, sezmam adres).
 *
 * @since  0.1
 **/
  void clear (void);

signals:


  
/**
 * Signal je emitovan pokud je stisknuto tlacitko pro standartni nastaveni.
 * Signal is emited when button for standart setting is pressed
 *
 * @since  0.1
 **/
  void standardSetted(void);

/**
 * Signal je emitovan pokud je zmeneno nastaveni. Tlacitko pridat, opravit, smazat nebo standartni nastaveni je stisknuto.
 * Signal is emited when seting is changed. Button Ad, Edit, Delete or Standart is pressed
 *
 * @since  0.1
 **/
  void changed(void);

public slots:
/**
 * Nastavu standartni hodnoty a posle signal standardSetted.
 * Sets standard values and sends signal changed.
 *
 * @since  0.1
 **/
  void standardButtonSlot (void);
  
            
private:

  enum typeAction {
    noneAction =0,
    addAction =1,
    editAction = 2,
    deleteAction =3
    };

  typeAction m_action;

  KNSCommonData* commonData;
  bool m_enableButton;
  listenAddressPortListDef* m_listenList;

  QTreeWidget* mainView;
  
//  KNSIntAddress* m_newAddress;
  KLineEdit* m_newAddress;
  KLineEdit* m_newPort;

  KPushButton* addButton;
  KPushButton* editButton;
  KPushButton* deleteButton;
  KPushButton* standardButton;
  KPushButton* okButton;
  KPushButton* cancelButton;


  void setMainButtons(bool mainButtons = true);
  void setOkCancelButtons(bool okCancelButtons = true);

  // testing address  for action add and edit
  bool addressIsExist (const QString address, const QString port);

  // enable or disable ok button
  void setOKButton (void);
        

/**
 *Prida prvek (adresu a port) do QTreeView (mainView).
 *Prvek je pridan na konec QTreeView.
 *Adds item (address and port) to QTreeView (mainView).
 *Item is added on end of QTreeView.
 *
 * @since  0.2
 **/
  void addItem (QString address, QString port);

        
private slots:

  void addButtonSlot (void);

  void editButtonSlot (void);

  void deleteButtonSlot (void);

  void cancelButtonSlot (void);

  void okButtonSlot (void);

  void lineSelelectedSlot ( void );
  
  void newAddressChangedSlot (const QString &);
  void newPortChangedSlot (const QString &);

};

#endif
