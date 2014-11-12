/***************************************************************************
                          knsacladdres.cpp  -  description
                             -------------------
    begin                : So kveten 8 2004
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

#include "knsacladdress.h"

#include <KDE/KLocale>
#include <KDE/KMessageBox>

#include <QtGui/QLayout>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QStringList>


#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>




#include <QtDebug>
#include <QDebug>



KNSACLAddress::KNSACLAddress(KNSCommonData* tCommonData, QWidget *parent  )
           : QGroupBox(parent), commonData(tCommonData)  {


  m_listenList = commonData->ListenAPAddr();


  setTitle(i18n("Interface for listening"));

  QVBoxLayout* mainVLayout = new QVBoxLayout();
  this->setLayout(mainVLayout);

  QHBoxLayout* mainLayout = new QHBoxLayout();
  mainLayout->setSpacing(5);
  mainVLayout->addLayout(mainLayout);
  
  QVBoxLayout* viewLayout = new QVBoxLayout();
  mainLayout->addLayout(viewLayout);

  QVBoxLayout* buttonsLayout = new QVBoxLayout();
  buttonsLayout->setSpacing(0);
  mainLayout->addLayout(buttonsLayout);

  mainView = new QTreeWidget(this);
  mainView->setSortingEnabled(false);
  mainView->setSelectionMode(QAbstractItemView::SingleSelection);


  viewLayout->addWidget(mainView);
  mainView->setColumnCount(2);

  QStringList headerList;
  headerList << i18n("IP Address") << i18n("Port");
    
  mainView->setHeaderLabels(headerList);

  QHBoxLayout* lineLayout = new QHBoxLayout();
  viewLayout->addLayout(lineLayout);

  QLabel* label1 = new QLabel(i18n("IP Address")+": ",this);
  lineLayout->addWidget(label1);
//  m_newAddress = new KNSIntAddress(this);
  m_newAddress = new KLineEdit(this);
  lineLayout->addWidget(m_newAddress,1);
  m_newAddress->setMinimumWidth(120);

  QLabel* label2 = new QLabel(i18n("Port")+": ",this);
  lineLayout->addWidget(label2);
  m_newPort = new KLineEdit(this);
  m_newPort->setMinimumWidth(60);
  lineLayout->addWidget(m_newPort,10);
  

  lineLayout->setStretchFactor(m_newAddress,15);
  lineLayout->setStretchFactor(m_newPort,5);
  
  addButton = new KPushButton(i18n("Add"),this);
  buttonsLayout->addWidget(addButton);

  editButton = new KPushButton(i18n("Edit"),this);
  buttonsLayout->addWidget(editButton);

  deleteButton = new KPushButton(i18n("Delete"),this);
  buttonsLayout->addWidget(deleteButton);
  
  standardButton = new KPushButton(i18n("Default"),this);
  buttonsLayout->addWidget(standardButton);


  okButton = new KPushButton(i18n("OK"),this);
  buttonsLayout->addWidget(okButton);

  cancelButton = new KPushButton(i18n("Cancel"),this);
  buttonsLayout->addWidget(cancelButton);
  buttonsLayout->addStretch ( 20 );


  
  setMainButtons(true);
  m_newAddress->setReadOnly(true);
  m_newPort->setReadOnly(true);
  m_action = noneAction;


  connect( addButton,SIGNAL(clicked()),this,SLOT(addButtonSlot()));
  connect( editButton,SIGNAL(clicked()),this,SLOT(editButtonSlot()));
  connect( deleteButton,SIGNAL(clicked()),this,SLOT(deleteButtonSlot()));
  connect( standardButton,SIGNAL(clicked()),this,SLOT(standardButtonSlot()));
  connect( okButton,SIGNAL(clicked()),this,SLOT(okButtonSlot()));
  connect( cancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonSlot()));

  connect (m_newAddress,SIGNAL(textChanged(const QString &)),SLOT(newAddressChangedSlot(const QString &)));
  connect (m_newPort,SIGNAL(textChanged(const QString &)),SLOT(newPortChangedSlot(const QString &)));
  
  connect (mainView,SIGNAL(itemSelectionChanged()),this,SLOT(lineSelelectedSlot()));
}

KNSACLAddress::~KNSACLAddress(){
}

void KNSACLAddress::addItem (QString address, QString port) {

      QTreeWidgetItem *newItem =  new QTreeWidgetItem(mainView);
      newItem->setText(0,address);
      newItem->setText(1,port);
      mainView->clearSelection();

      listenAddressPortStruct newACLAddress;
      newACLAddress.port=port;
      newACLAddress.address=address;
      m_listenList->push_back(newACLAddress);

  }

void KNSACLAddress::loadItems (void) {
  listenAddressPortListDef::const_iterator it;

  mainView->clear();
  for (it=m_listenList->begin(); it != m_listenList->end(); ++it ) {
     QTreeWidgetItem *newItem =  new QTreeWidgetItem(mainView);
     newItem->setText(0,(*it).address);
     newItem->setText(1,(*it).port);

    }
  mainView->clearSelection();
  }


void KNSACLAddress::setOkCancelButtons(bool okCancelButtons) {

  setMainButtons(!okCancelButtons);
  }


void KNSACLAddress::setMainButtons(bool mainButtons) {

  if (mainButtons) {
    addButton->show();
    editButton->show();
    deleteButton->show();
    standardButton->show();

    if  (mainView->selectedItems().count() > 0 ) {
      editButton->setEnabled(true);
      deleteButton->setEnabled(true);
      }
    else {
      editButton->setEnabled(false);
      deleteButton->setEnabled(false);
      }
    okButton->hide();
    cancelButton->hide();
    }
  else {
    addButton->hide();
    editButton->hide();
    deleteButton->hide();
    standardButton->hide();
    okButton->show();
    cancelButton->show();
    setOKButton();
    }
  }


void KNSACLAddress::clear (void) {

  mainView->clear();
  }


bool KNSACLAddress::addressIsExist (const QString address, const QString port) {
  listenAddressPortListDef::const_iterator addrIt;

  for (addrIt=m_listenList->begin();addrIt !=m_listenList->end();++addrIt ) {
    if (((*addrIt).port == port) && ((*addrIt).address == address)) return true;
    }
  return false;
  }


  
void KNSACLAddress::setOKButton (void) {

  if (m_newPort->text().isEmpty() || (m_newAddress->text().isEmpty())) {
    okButton->setEnabled(false);
    return;
    }
  if (m_action == editAction) {
    if (addressIsExist(m_newAddress->text(),m_newPort->text())) {
      okButton->setEnabled(false);
      return;
      }
    }
    
  if (m_action == addAction) {
    if (addressIsExist(m_newAddress->text(),m_newPort->text())) {
      okButton->setEnabled(false);
      return;
      }
    }
  okButton->setEnabled(true);
  }
  
                
// ---------- SLOTS --------------

void KNSACLAddress::addButtonSlot (void) {

  m_action = addAction;
  mainView->setEnabled(false);
  m_newPort->setText("3493");
  m_newAddress->setText("0.0.0.0");
  
  setOkCancelButtons();

  m_newAddress->setReadOnly(false);
  m_newPort->setReadOnly(false);

  m_newAddress->setFocus();
  }



void KNSACLAddress::editButtonSlot (void) {

  m_action = editAction;
  mainView->setEnabled(false);
  setOkCancelButtons();

  m_newAddress->setReadOnly(false);
  m_newPort->setReadOnly(false);

  m_newAddress->setFocus();
  }
  

void KNSACLAddress::deleteButtonSlot (void) {
  QString name;

  m_action = deleteAction;
  mainView->setEnabled(false);
  setOkCancelButtons();

  okButton->setFocus();
  }

void KNSACLAddress::standardButtonSlot (void) {
   listenAddressPortStruct newACLAddress;
   QString address;
   QString port;

   bool local4Found = false;
   bool local6Found = false;

   if (m_action != noneAction) cancelButtonSlot();

     int i = 0;
     while (i < mainView->topLevelItemCount()) {
  
     if ((mainView->topLevelItem(i)->text(0) == "127.0.0.1" ) || (mainView->topLevelItem(i)->text(0) == "::1" )) {

       if (mainView->topLevelItem(i)->text(0) == "127.0.0.1" ) {
         local4Found = true;      
         }
       if (mainView->topLevelItem(i)->text(0) == "::1" ) {
         local6Found = true;      
         }
       mainView->topLevelItem(i)->setText(1,"3493");
       i++;
       }
     else {
       address= mainView->topLevelItem(i)->text(0);
       port=mainView->topLevelItem(i)->text(1);
       mainView->takeTopLevelItem (i);
       for (listenAddressPortListDef::iterator addrIt=m_listenList->begin();addrIt !=m_listenList->end();++addrIt ) {
         if (((*addrIt).address == address) &&  ((*addrIt).port == port)) {
           m_listenList->erase(addrIt);
           break;
         }
       } 
     }
   }

  if (!local6Found) addItem ("::1","3493");
  if (!local4Found) addItem ("127.0.0.1","3493");

  m_newAddress->setText("");
  m_newPort->setText("");
  emit standardSetted();
  emit changed();
  }


void KNSACLAddress::cancelButtonSlot (void) {

  m_action = noneAction;
  mainView->setEnabled(true);
  setMainButtons();
  m_newAddress->setReadOnly(true);
  m_newPort->setReadOnly(true);
  if  (! mainView->selectedItems().isEmpty() ) {
    m_newAddress->setText(mainView->selectedItems().first()->text(0));
    m_newPort->setText(mainView->selectedItems().first()->text(1));
    }
  mainView->setFocus();
  if (mainView->selectedItems().isEmpty()) {
    m_newPort->setText("");
    m_newAddress->setText("");
    }
  }


void KNSACLAddress::okButtonSlot (void) {
  QString oldPort;
  QString oldAddress;
  QString address;
  QString port;
  listenAddressPortStruct newACLAddress;
  listenAddressPortListDef::iterator addrIt;

  switch  (m_action) {

    case addAction:

      if (!addressIsExist (m_newAddress->text(),m_newPort->text())) {
        addItem(m_newAddress->text(),m_newPort->text());
        m_newAddress->setText("");
        m_newPort->setText("");
        emit changed();
        }
      else KMessageBox::sorry(this,i18n("Sorry. This address and port are existed."));
    break;

    case deleteAction:
      address=mainView->selectedItems().first()->text(0);
      port=mainView->selectedItems().first()->text(1);
      mainView->takeTopLevelItem (mainView->indexOfTopLevelItem(mainView->selectedItems().first()));
      for (addrIt=m_listenList->begin();addrIt !=m_listenList->end();++addrIt ) {
        if (((*addrIt).address == address) && ((*addrIt).port == port)) {
          m_listenList->erase(addrIt);
          emit changed(); 
          break;
          }
        }  
    break;

    case editAction:
    oldPort=mainView->selectedItems().first()->text(1);
    oldAddress=mainView->selectedItems().first()->text(0);
    if ((m_newPort->text() != oldPort) || (m_newAddress->text() != oldAddress)) {
      //sets new address
      if (m_newAddress->text() != oldAddress) {
        mainView->selectedItems().first()->setText(0,m_newAddress->text());
        }
      // sets new name of address
      if (m_newPort->text() != oldPort) {
        mainView->selectedItems().first()->setText(1,m_newPort->text());
        }
      //rename item in common data
      for (addrIt=m_listenList->begin();addrIt !=m_listenList->end();++addrIt ) {
        if ((*addrIt).port == oldPort) {
          (*addrIt).port= m_newPort->text();
          (*addrIt).address = m_newAddress->text();
          break;
          }
        }
      emit changed();
      }

    break;
    case noneAction:
    break;
    }
  m_action = noneAction;
  mainView->setEnabled(true);
  setMainButtons(true);
  m_newAddress->setReadOnly(true);
  m_newPort->setReadOnly(true);
  mainView->setFocus();

  listenAddressPortListDef::const_iterator addrIt1;
  for (addrIt1=m_listenList->begin();addrIt1 !=m_listenList->end();++addrIt1 ) {
    }
  }
    
  
void KNSACLAddress::lineSelelectedSlot ( void ) {

  if  (! mainView->selectedItems().isEmpty() ) {
    setMainButtons(true);
    m_newAddress->setText(mainView->selectedItems().first()->text(0));
    m_newPort->setText(mainView->selectedItems().first()->text(1));
    }
  else
    {
     m_newPort->setText("");
     m_newAddress->setText("");
    }
  }

void KNSACLAddress::newPortChangedSlot (const QString &) {

  setOKButton();
  }

void KNSACLAddress::newAddressChangedSlot (const QString &) {

  setOKButton();
  }

#include "knsacladdress.moc"

