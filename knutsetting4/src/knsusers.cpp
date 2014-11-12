/***************************************************************************
                          knsusers.cpp  -  description
                             -------------------
    begin                : Po srp 16 2004
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

#include "knsusers.h"
#include "knsuseredit.h"

#include <KDE/KLocale>
#include <KDE/KMessageBox>

#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QPixmap>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>

#include <QDebug>


KNSUsers::KNSUsers(KNSCommonData* commonData,  QWidget *parent )
         : QLabel(parent), m_commonData(commonData)  {


  m_usersData = m_commonData->usersDataAddr();
  m_userPix = KIcon("system-users");
  m_monitorPix = KIcon("system-software-update");

  m_activeItemIt=m_usersData->end();


  m_iconWidget = new QListWidget(this);
  m_iconWidget->setViewMode(QListView::IconMode);

  m_addButton = new KPushButton(i18n("Add"),this);
  m_editButton = new KPushButton(i18n("Edit"),this);
  m_deleteButton = new KPushButton(i18n("Delete"),this);
  m_okButton = new KPushButton(i18n("OK"),this);
  m_cancelButton = new KPushButton(i18n("Cancel"),this);

  m_itemWidget = new KNSUserEdit( this);

  m_standardButton = new KPushButton (i18n("Default of users"),this);

  QVBoxLayout* topLayout = new QVBoxLayout(this );
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* widgetLayout = new QHBoxLayout();
  widgetLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* iconLayout = new QVBoxLayout();
  iconLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* buttonLayout = new QVBoxLayout();
  buttonLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* sButtonLayout = new QHBoxLayout ();
  sButtonLayout->setSizeConstraint(QLayout::SetMinimumSize);


  topLayout->addLayout(widgetLayout);
  topLayout->setStretchFactor(m_itemWidget,10);
  topLayout->addLayout(sButtonLayout);
 
  widgetLayout->addLayout(iconLayout);
  widgetLayout ->addLayout(buttonLayout); 

  iconLayout->addWidget(m_iconWidget);
  iconLayout->addWidget(m_itemWidget);

  buttonLayout->addWidget(m_addButton);
  buttonLayout->addWidget(m_editButton);
  buttonLayout->addWidget(m_deleteButton);
  buttonLayout->addWidget(m_okButton);
  buttonLayout->addWidget(m_cancelButton);
  buttonLayout->addStretch(10);


  sButtonLayout->addStretch( 10 );
  sButtonLayout->addWidget(m_standardButton);

  setMainButtons();
  m_itemWidget->setReadOnly(true);

  m_action=noneAction;

  // sends information, when name or password changed , if name and password are empty returns false
  connect(m_itemWidget,SIGNAL(nameAndPasswordAreEmpty ( bool)),this, SLOT(nameAndPasswordAreEmptySlot(bool)));       
  
  connect(m_iconWidget,SIGNAL(itemSelectionChanged ()),this, SLOT(iconSelectedSlot()));       

  connect(m_editButton,SIGNAL(released()),this, SLOT(editItemSlot( )));
  connect(m_addButton,SIGNAL(released()),this, SLOT(addItemSlot( )));
  connect(m_deleteButton,SIGNAL(released()),this, SLOT(deleteItemSlot( )));

  connect(m_cancelButton,SIGNAL(released()),this, SLOT(cancelItemSlot()));
  connect(m_okButton,SIGNAL(released()),this, SLOT(okItemSlot()));

  connect (m_standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));

  }


KNSUsers::~KNSUsers(){
  }

void KNSUsers::loadItems(void) {
    usersPermList::const_iterator usersIt;

  m_iconWidget->clear();
  m_itemWidget->clear();
  for (usersIt=m_usersData->begin();usersIt !=m_usersData->end();++usersIt ) {
    QListWidgetItem* myWidget = new QListWidgetItem((*usersIt).name,m_iconWidget);
    if ((*usersIt).isUser)
      myWidget->setIcon(m_userPix);
    else
      myWidget->setIcon(m_monitorPix);
    }
  }


void KNSUsers::iconSelectedSlot( void ){
  QString nameSection;
  usersPermList::const_iterator usersIt;

  if (!m_iconWidget->selectedItems().isEmpty()) {
    setMainButtons();
    nameSection = (m_iconWidget->selectedItems())[0]->text();
    for (usersIt=m_usersData->begin();usersIt !=m_usersData->end();++usersIt ) {
      if ((*usersIt).name == nameSection ) break;
      }
    if (usersIt == m_usersData->end()) return;
    // we have got iterator for our structure  
    m_activeItemIt = usersIt;
    m_itemWidget->putData(nameSection,(*usersIt).password,(*usersIt).isUser,(*usersIt).actions,(*usersIt).instcmds,(*usersIt).upsmon);
    }
  else {
    m_itemWidget->clear();
    }
  }



void KNSUsers::addItemSlot(void) {

  m_iconWidget->setEnabled(false);
  m_itemWidget->setReadOnly (false);
  m_itemWidget->init();
  setOkCancelButtons();
  m_action=addAction;
  m_iconWidget->clearSelection();
  m_itemWidget->init();
  m_itemWidget->testAreNamePasswordEmpty();
  }



void KNSUsers::editItemSlot(void) {

  m_iconWidget->setEnabled(false);
  m_itemWidget->setReadOnly (false);
  m_itemWidget->setReadOnlyName(true);

  setOkCancelButtons();
  m_itemWidget->testAreNamePasswordEmpty();
  m_action=editAction;
  m_oldPassword=m_itemWidget->getPassword();
  m_oldTypeMonitoring=m_itemWidget->getTypeMon();
  m_oldIsUser=m_itemWidget->getIsUser();
  }

void KNSUsers::deleteItemSlot(void) {

  if (!m_iconWidget->selectedItems().isEmpty()) {
    m_action=deleteAction;
    m_iconWidget->setEnabled(false);
    m_itemWidget->setReadOnly (true);
//    m_itemWidget->setReadOnlyName(true);
    setOkCancelButtons();
    }
  }


void KNSUsers::cancelItemSlot(void) {

  // we must set old values for window
  if ( m_action == addAction ) {
    m_itemWidget->clear();
    }
  if ( m_action == editAction ) {
    m_itemWidget->putData((*m_activeItemIt).name,(*m_activeItemIt).password,(*m_activeItemIt).isUser,
      (*m_activeItemIt).actions,(*m_activeItemIt).instcmds,(*m_activeItemIt).upsmon);
    }
  m_iconWidget->setEnabled(true);
  m_itemWidget->setReadOnly(true);
  setMainButtons();
  }


void KNSUsers::okItemSlot(void) {
  QString name, password, actions;
  bool isUser;
  typeOfMonitoring typeMon;
  usersPermList::iterator it;
  userPermStruct newUserPerm;
  QListWidgetItem* newIcon;
  QStringList instCmds;
  QString userName;

  switch (m_action) {
    case addAction:
    m_itemWidget->getData (name, password, isUser, actions, instCmds, typeMon);
    it = findName(name);
    // new record, we must add one in list
    if (it == m_usersData->end()) {
      newUserPerm.name=name;
      newUserPerm.password=password;
      newUserPerm.isUser=isUser;
      newUserPerm.actions=actions;
      newUserPerm.instcmds=instCmds;
      newUserPerm.upsmon=typeMon;
      m_usersData->push_back(newUserPerm);
      newIcon = new QListWidgetItem(name,m_iconWidget);
      if (isUser)
        newIcon->setIcon(m_userPix);
      else
        newIcon->setIcon(m_monitorPix);
      emit userAdded(name);
      emit changed();
      }
    else {
      KMessageBox::sorry(this,i18n("Sorry. This name of user is existed."));
      return;
      }
    m_itemWidget->clear();
    break;
    case editAction:
    // we rewrites record in list
    if (!m_iconWidget->selectedItems().isEmpty()) {
      userName=(m_iconWidget->selectedItems())[0]->text();
      it = findName(userName);
      m_itemWidget->getData (name, password, isUser, actions, instCmds, typeMon);
      if (it != m_usersData->end()) {
        if ((*it).isUser!=isUser) {
          if (isUser)
            (m_iconWidget->selectedItems())[0]->setIcon(m_userPix);
          else
            (m_iconWidget->selectedItems())[0]->setIcon(m_monitorPix);
          }
        (*it).password=password;
        (*it).isUser=isUser;
        (*it).actions=actions;
        (*it).instcmds=instCmds;
        (*it).upsmon=typeMon;
        if (m_oldPassword != password) emit passwordChanged (name, password);
        if (m_oldTypeMonitoring != typeMon) emit monitoringChanged (name, typeMon);
        if (m_oldIsUser != isUser) emit typeUserChanged (name, isUser);
        emit changed();
        }
      }
    break;
    case deleteAction:
    if (!m_iconWidget->selectedItems().isEmpty()) {


      userName=(m_iconWidget->selectedItems())[0]->text();
      it = findName(userName);
      if (it != m_usersData->end()) m_usersData->erase(it);
      //deletes item from IconView
      m_iconWidget->takeItem(m_iconWidget->row((m_iconWidget->selectedItems()).at(0)));
      m_itemWidget->clear();
      emit userDeleted (userName);
      emit changed();
    //  break;
      }
    break;
    case noneAction:
    break;
    }

  m_iconWidget->setEnabled(true);
  m_itemWidget->setReadOnly(true);
  m_iconWidget->setFlow(QListView::LeftToRight);
  m_action=noneAction;
  setMainButtons();
  }


void KNSUsers::setOkCancelButtons(bool okCancelButtons) {

  setMainButtons(!okCancelButtons);
  }


void KNSUsers::setMainButtons(bool mainButtons) {

  if (mainButtons) {
    m_addButton->show();
    m_editButton->show();
    m_deleteButton->show();
    if (!m_iconWidget->selectedItems().isEmpty()) {
      m_editButton->setEnabled(true);
      m_deleteButton->setEnabled(true);
      }
    else {
      m_editButton->setEnabled(false);
      m_deleteButton->setEnabled(false);
      }
    m_okButton->hide();
    m_cancelButton->hide();
    }
  else {
    m_addButton->hide();
    m_editButton->hide();
    m_deleteButton->hide();
    m_okButton->show();
    m_cancelButton->show();
//    m_okButton->setEnabled(false);
    }
  }

usersPermList::iterator KNSUsers::findName(QString name) {
  usersPermList::iterator it;


  for (it = m_usersData->begin(); it != m_usersData->end(); it++) {
    if ((*it).name == name) {
      return it;
      }
    }
  return m_usersData->end();
}

QString KNSUsers::findLocalhost (void) {
  return "";
  }



//------------SLOTS----------------------

void KNSUsers::nameAndPasswordAreEmptySlot (const bool areEmpty ) {
  m_okButton->setEnabled(! areEmpty);
}


void KNSUsers::setStandardSlot (void) {
  QListWidgetItem* newIcon;
  userPermStruct newUserPerm;
  usersPermList::iterator usersIt;
  QString userName;


  m_itemWidget->clear();
  m_iconWidget->clearSelection();

  int i = 0;
  while (i < m_iconWidget->count()) {

    userName = m_iconWidget->item(i)->text();
    if (userName != "monuser" ) {
      for (usersIt=m_usersData->begin();usersIt !=m_usersData->end();++usersIt ) {
        if ((*usersIt).name == userName) {
          m_usersData->erase(usersIt);
          break;
          }
        }
      m_iconWidget->takeItem(i);
      emit userDeleted (userName);

      }
    else i++;
    } // end of while
    // when iconView is empty, we must add new item. Name of this item is "monuser"
    if (m_iconWidget->count() == 0) {
      newUserPerm.name="monuser";
      newUserPerm.password="pass";
      newUserPerm.isUser=false;
      newUserPerm.actions=""; // no actions
      newUserPerm.instcmds.clear();
      newUserPerm.upsmon=MON_MASTER;
      m_usersData->push_back(newUserPerm);
      newIcon = new QListWidgetItem("monuser",m_iconWidget);
      newIcon->setIcon(m_monitorPix);
      emit userAdded("monuser");
      }
    else {
      // set standard setting
      for (usersIt=m_usersData->begin();usersIt !=m_usersData->end();++usersIt ) {
        (*usersIt).isUser=false;
        (*usersIt).actions=""; // no actions
        (*usersIt).instcmds.clear();
        (*usersIt).upsmon=MON_MASTER;
        }
      }
  m_iconWidget->setFlow(QListView::LeftToRight);

  emit changed();
  }

#include "knsusers.moc"
