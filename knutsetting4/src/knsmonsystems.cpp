/***************************************************************************
                          knsmonsystems.cpp  -  description
                             -------------------
    begin                : ?t lis 25 2004
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

#include "knsmonsystems.h"

#include <KDE/KLocale>

#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtCore/QString>
#include <QtCore/QStringList> 
#include <QtGui/QCheckBox>
#include <QtGui/QRadioButton>


#include <QtDebug>
#include <QDebug>


KNSMonSystems::KNSMonSystems(KNSCommonData* commonData, QWidget *parent)
              : QWidget(parent), m_commonData(commonData)  {

  m_monitorUpsList =commonData->monitorUpsListDataAddr();



  m_listViewWidget = new QTreeWidget(this);
  m_listViewWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_listViewWidget->setSortingEnabled (true);
  m_listViewWidget->setAllColumnsShowFocus(true);
  m_listViewWidget->setColumnCount(6);
  QStringList labelsList;
  labelsList << i18n("UPS name") << i18n("Host name") << i18n("Number of power") << i18n("User name") << i18n("Password") << i18n("Type");
  m_listViewWidget->setHeaderLabels(labelsList);

  m_addButton = new KPushButton(i18n("Add"),this);
  m_editButton = new KPushButton(i18n("Edit"),this);
  m_deleteButton = new KPushButton(i18n("Delete"),this);
  m_standardButton = new KPushButton(i18n("Default for monitoring"),this);
  m_okButton = new KPushButton(i18n("OK"),this);
  m_cancelButton = new KPushButton(i18n("Cancel"),this);


  QGroupBox* itemBox = new QGroupBox(this);

  QLabel* label1 = new QLabel(i18n("UPS name"),itemBox);
  m_upsName = new KLineEdit(itemBox);
  m_upsNameBox = new KComboBox(itemBox);
  label1->setBuddy(m_upsName);

  QLabel* label2 = new QLabel(i18n("Host name"),itemBox);
  m_hostName = new KLineEdit(itemBox);
  label2->setBuddy(m_hostName);

  QLabel* label3 = new QLabel(i18n("Number of power"),itemBox);
  m_powerValue = new KIntNumInput(itemBox);
  m_powerValue->setMinimum(1);
  m_powerValueLabel = new QLabel(itemBox);
  label3->setBuddy(m_powerValue);


  QLabel* label4 = new QLabel(i18n("User name"),itemBox);
  m_userName = new KLineEdit(itemBox);
  m_userNameBox = new KComboBox(itemBox);
  label4->setBuddy(m_userName);


  QLabel* label5 = new QLabel(i18n("Password"),itemBox);
  m_password = new KLineEdit(itemBox);
  label5->setBuddy(m_password); 


  QLabel* label6 = new QLabel(i18n("Type of UPS"),itemBox);
  QLabel* label7 = new QLabel(i18n("Local UPS"),itemBox);
  m_localHostBox = new QCheckBox(itemBox);
  m_masterType = new QRadioButton("Master",itemBox);
  m_slaveType = new QRadioButton("Slave",itemBox);


  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QHBoxLayout* widgetLayout = new QHBoxLayout();
  widgetLayout->setSizeConstraint(QLayout::SetMinimumSize);


  QVBoxLayout* listViewLayout = new QVBoxLayout();
  listViewLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* buttonLayout = new QVBoxLayout();
  buttonLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* itemTopLayout = new QVBoxLayout(itemBox );
  itemTopLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* itemHLayout = new QHBoxLayout();
  itemHLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QGridLayout* itemGridLayout = new QGridLayout();
  itemGridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* powerValueLayout = new QHBoxLayout();
  powerValueLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* upsNameLayout = new QHBoxLayout();
  upsNameLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* userNameLayout = new QHBoxLayout();
  userNameLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* typeLayout = new QHBoxLayout();
  typeLayout->setSizeConstraint(QLayout::SetMinimumSize);


  //topLayout->addLayout(listViewLayout);
  topLayout->addLayout(widgetLayout);
  topLayout->addWidget(itemBox);


  widgetLayout->addLayout(listViewLayout);
  widgetLayout->addLayout(buttonLayout);

  listViewLayout->addWidget(m_listViewWidget);
  listViewLayout->addWidget(itemBox);

  buttonLayout->addWidget(m_addButton);
  buttonLayout->addWidget(m_editButton);
  buttonLayout->addWidget(m_deleteButton);
  buttonLayout->addWidget(m_standardButton);
  buttonLayout->addWidget(m_okButton);
  buttonLayout->addWidget(m_cancelButton);
  buttonLayout->addStretch(10);

  itemTopLayout->addLayout(itemHLayout);
 
  itemGridLayout->addWidget(label1,0,0);
  itemGridLayout->addWidget(label2,1,0);
  itemGridLayout->addWidget(label3,2,0);
  itemGridLayout->addWidget(label4,3,0);
  itemGridLayout->addWidget(label5,4,0);
  itemGridLayout->addWidget(label6,5,0);
  itemGridLayout->addWidget(label7,6,0);
  itemGridLayout->addLayout(upsNameLayout,0,1);
  itemGridLayout->addWidget(m_hostName,1,1);
  itemGridLayout->addLayout(powerValueLayout,2,1);
  itemGridLayout->addLayout(userNameLayout,3,1);
  itemGridLayout->addWidget(m_password,4,1);
  itemGridLayout->addLayout(typeLayout,5,1);
  itemGridLayout->addWidget(m_localHostBox,6,1);

  itemHLayout->addLayout(itemGridLayout);

  powerValueLayout->addWidget(m_powerValue);
  powerValueLayout->addWidget(m_powerValueLabel);

  upsNameLayout->addWidget(m_upsName);
  upsNameLayout->addWidget(m_upsNameBox);

  userNameLayout->addWidget(m_userName);
  userNameLayout->addWidget(m_userNameBox);

  typeLayout->addWidget(m_masterType);
  typeLayout->addWidget(m_slaveType);


  setInputWidgetsReadOnly(true);
  setMainButtons();

  //sets currentItem
  QList<QTreeWidgetItem *> selectedItems =  m_listViewWidget->selectedItems();
  if (selectedItems.count() > 0) {
     m_listViewWidget->setCurrentItem(selectedItems[0]);
    }
  

  m_action = noneAction;
  m_oldUpsName = "";
  m_oldHostName = "";

  connect (m_listViewWidget,SIGNAL(itemClicked(QTreeWidgetItem*, int )),this,SLOT(selectedItemListSlot(QTreeWidgetItem*, int )));

  connect(m_editButton,SIGNAL(released()),this, SLOT(editItemSlot()));
  connect(m_addButton,SIGNAL(released()),this, SLOT(addItemSlot()));
  connect(m_deleteButton,SIGNAL(released()),this, SLOT(deleteItemSlot()));

  connect(m_okButton,SIGNAL(released()),this, SLOT(okItemSlot()));
  connect(m_cancelButton,SIGNAL(released()),this, SLOT(cancelItemSlot()));

  connect(m_localHostBox,SIGNAL(released()),this, SLOT(localHostSelectedSlot()));

  connect(m_userNameBox,SIGNAL(activated(const QString & )),this,SLOT(userChangedSlot (const QString &)));

  connect(m_standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));
  }


KNSMonSystems::~KNSMonSystems(){
}


void KNSMonSystems::setUserComboBox (void) {
  usersPermList::const_iterator it;

  m_userNameBox->clear();
  usersPermList* usersData = m_commonData->usersDataAddr();
  for (it = usersData->begin(); it != usersData->end(); it++) {
    if (!(*it).isUser) m_userNameBox->addItem((*it).name);
    }

  }

void KNSMonSystems::setModulesComboBox (const QString editName) {

  modulesList::const_iterator it;
  int i, currentIndex;

  m_upsNameBox->clear();
  i=0;
  currentIndex = -1;
  modulesList* modulesData = m_commonData->modulesDataAddr();
  for (it = modulesData->begin(); it != modulesData->end(); it++) {

//    if ((!existLocalUps((*it).name)) || ((*it).name == editName )) m_upsNameBox->addItem((*it).name);
//    if (existLocalUps((*it).name)) m_upsNameBox->addItem((*it).name);
     m_upsNameBox->addItem((*it).name);
    if ((*it).name == editName) currentIndex = i;
    i++;
    }
  if (currentIndex != -1) m_upsNameBox->setCurrentIndex(currentIndex);    
  }


bool KNSMonSystems::existLocalUps (const QString name) {


  for (int i = 0; i < m_listViewWidget->topLevelItemCount(); i ++) {
    if ((m_listViewWidget->topLevelItem(i)->text(1) == "localhost") 
         and (m_listViewWidget->topLevelItem(i)->text(0) == name) ) return true; 
    }
  return false;
  }


void KNSMonSystems::clear(void) {
  m_listViewWidget->clear();
  clearAllWidgets();
  }

void KNSMonSystems::clearAllWidgets(void) {

  m_upsName->setText("");
  m_upsNameBox->clear();
  m_hostName->setText("");
  m_powerValue->setValue(m_powerValue->minimum());
  m_powerValueLabel->setText("");

  m_userName->setText("");
  m_userNameBox->clear();
  m_password->setText("");
  m_localHostBox->setChecked(true);
  m_masterType->setChecked(true);

  }


void KNSMonSystems::deleteUser( const QString userName ) {
  monitorUpsList::iterator it;
  bool itemIsDeleted;



  // deleted records from treeWidget

  itemIsDeleted = false;
  int i = 0;
  while (i < m_listViewWidget->topLevelItemCount()) {
    if (m_listViewWidget->topLevelItem(i)->text(3)==userName) {
      if (m_listViewWidget->topLevelItem(i)->isSelected()) {
        clearAllWidgets();
        m_listViewWidget->topLevelItem(i)->setSelected(false);
        setMainButtons();
        }
      m_listViewWidget->takeTopLevelItem(i);
      itemIsDeleted=true;
      }
    else i++; 
    }

  //delete records from commonData
  it=m_monitorUpsList->begin();
  while (it != m_monitorUpsList->end()) {
    if ((*it).userName==userName) it = m_monitorUpsList->erase(it);  // delete item
    else it++;
    }
  setUserComboBox();
  if (itemIsDeleted) emit changed();
  }


void KNSMonSystems::addUser( const QString ) {

  setUserComboBox();
  }


void KNSMonSystems::deleteUps( const QString upsName ) {
  monitorUpsList::iterator it;
  bool itemIsDeleted;


  // deleted record from list view
  itemIsDeleted = false;

  int i = 0;
  while (i < m_listViewWidget->topLevelItemCount()) {
    if ((m_listViewWidget->topLevelItem(i)->text(1)=="localhost")
      && (m_listViewWidget->topLevelItem(i)->text(0)==upsName)) {
      m_listViewWidget->takeTopLevelItem(i);

      m_commonData->deleteMonitorUpsItem (upsName,"localhost");
      itemIsDeleted=true;
      }
    else i++;
    }

  // deleted record from list of items
//  it=m_monitorUpsList->begin();
//  while (it != m_monitorUpsList->end()) {
//    if (((*it).upsName==upsName) && ((*it).hostName=="localhost")) it = m_monitorUpsList->erase(it); // delete item
//    else it++; 
//    }

  setModulesComboBox("");
  if (itemIsDeleted) emit changed();
  }



void KNSMonSystems::loadItems(void) {
   monitorUpsList::const_iterator it;
   QString upsmon;

   m_listViewWidget->clear();

   for (it=m_monitorUpsList->begin(); it != m_monitorUpsList->end(); it++) {
     if ((*it).upsmon == MON_MASTER) upsmon="master"; else upsmon="slave";
     QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(m_listViewWidget);
     treeWidgetItem->setText(0,(*it).upsName);
     treeWidgetItem->setText(1,(*it).hostName);
     treeWidgetItem->setText(2,QString::number((*it).powerValue));
     treeWidgetItem->setText(3,(*it).userName);
     treeWidgetItem->setText(4,(*it).password);
     treeWidgetItem->setText(5,upsmon);
     }
  setMainButtons();
  }


void KNSMonSystems::setOkCancelButtons(const bool buttons) {
  
  setMainButtons (! buttons);
  }



void KNSMonSystems::setMainButtons(const bool buttons) {

  if (buttons) {
    m_addButton->show();
    m_editButton->show();
    m_deleteButton->show();
    m_standardButton->show();
    if (m_listViewWidget->selectedItems().count() != 0) {
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
    m_standardButton->hide();
    m_okButton->show();
    m_cancelButton->show();

    if (m_action == deleteAction )  m_okButton->setEnabled(true);
    else {
      if ((m_upsNameBox->count() == 0) && (m_localHostBox->isChecked())) m_okButton->setEnabled(false);
      else m_okButton->setEnabled(true);
      }
    }
  }


void KNSMonSystems::setInputWidgetsReadOnly(const bool readOnly) {

  if (readOnly) {
    m_hostName->setReadOnly(true);
    m_upsName->setReadOnly(true);
    m_upsNameBox->hide();
    m_upsName->show();
    m_powerValue->setEnabled(false);
    m_powerValue->hide();
    m_powerValueLabel->show();
    m_userName->setReadOnly(true);
    m_userNameBox->hide();
    m_userName->show();
    m_password->setReadOnly(true);
    m_masterType->setEnabled(false);
    m_slaveType->setEnabled(false);
    m_localHostBox->setEnabled(false);
    }
  else {
    m_hostName->setReadOnly(false);
    m_upsName->setReadOnly(false);
    m_upsNameBox->show();
    m_powerValue->setEnabled(true);
    m_powerValue->show();
    m_powerValueLabel->hide();
    m_userName->setReadOnly(false);
    m_userNameBox->show();
    m_password->setReadOnly(false);
    m_masterType->setEnabled(true);
    m_slaveType->setEnabled(true);
    m_localHostBox->setEnabled(true);
    }
  }

void KNSMonSystems::clearROWidgets(void) {

  m_upsName->setText("");
  m_hostName->setText("");
  m_powerValueLabel->setText("");
  m_userName->setText("");
  m_password->setText("");
  m_masterType->setChecked(true);
  }

bool KNSMonSystems::isPrepare(void) {
  if (m_localHostBox->isChecked()) {
    if (m_upsNameBox->currentText().isEmpty()) return false;
    if (m_userNameBox->currentText().isEmpty()) return false;
    }
  else {
    if (m_hostName->text().isEmpty()) return false;
    if (m_upsName->text().isEmpty()) return false;
    if (m_userName->text().isEmpty()) return false;
    if (m_password->text().isEmpty()) return false;
    }
  return true;
  }


void KNSMonSystems::changePassword(const QString newUserName, const QString newPassword) {
  bool nameFound;

  nameFound = false;
  //corrects listView
  for (int i = 0; i < m_listViewWidget->topLevelItemCount (); i++ ) {
    if (m_listViewWidget->topLevelItem(i)->text(3) == newUserName ) {
      m_listViewWidget->topLevelItem(i)->setText(4,newPassword);
      nameFound = true;
      }
    }

  // correct commonData
  for (monitorUpsList::iterator it = m_monitorUpsList->begin(); it != m_monitorUpsList->end(); it++) {
    if ((*it).userName == newUserName) (*it).password = newPassword;
    }

  if (m_userNameBox->isHidden()) {
    if (m_userName->text()==newUserName) m_password->setText(newPassword);
    }
  else {
    if (m_userNameBox->currentText()==newUserName) m_password->setText(newPassword);
    }
  if (nameFound) emit changed();
  }




void KNSMonSystems::changeTypeMon(const QString newUserName, const typeOfMonitoring newTypeMon) {
  bool nameFound;

  nameFound = false;
  //corrects listView
  for (int i = 0; i < m_listViewWidget->topLevelItemCount (); i++ ) {
    if (m_listViewWidget->topLevelItem(i)->text(3) == newUserName ) {
      if (newTypeMon==MON_MASTER) m_listViewWidget->topLevelItem(i)->setText(5,"master");
      else m_listViewWidget->topLevelItem(i)->setText(5,"slave");
      nameFound = true;
      }
    }
 
  // correct commonData
  for (monitorUpsList::iterator it = m_monitorUpsList->begin(); it != m_monitorUpsList->end(); it++) {
    if ((*it).userName == newUserName) (*it).upsmon = newTypeMon;
    }
  if (m_userName->text()==newUserName) {
    if (newTypeMon==MON_MASTER) m_masterType->setChecked(true); else m_slaveType->setChecked(true);
    }
  if (nameFound) emit changed();
  }




void KNSMonSystems::setInputForm (void) {

  if (m_localHostBox->isChecked()) {      // local host
    m_upsName->hide();
    m_upsNameBox->show();
    m_hostName->setText("localhost");
    m_hostName->setReadOnly(true);
    m_userName->hide();
    m_userNameBox->show();
    m_password->setReadOnly(true);
    userChangedSlot(m_userNameBox->currentText());
    if (m_upsNameBox->count() == 0) m_okButton->setEnabled(false);
    else m_okButton->setEnabled(true);
    }
  else {     // nolocal host
    m_upsName->show();
    m_upsNameBox->hide();
    m_hostName->setReadOnly(false);
    m_userName->show();
    m_userNameBox->hide();
    m_password->setReadOnly(false);
    switch (m_action) {
      case addAction:
      m_password->setText("");
      if (m_hostName->text() == "localhost") m_hostName->setText("");
      break;
      case editAction:
      if ((m_listViewWidget->selectedItems().count() > 0) && ( (m_listViewWidget->selectedItems())[0]->text(1) != "localhost")) {
        m_hostName->setText((m_listViewWidget->selectedItems())[0]->text(1));
        }
      else m_hostName->setText("");
      break;
      case noneAction:
      break;
      case deleteAction:
      break;
      }
    m_okButton->setEnabled(true);
    }
  }



//*********************** SLOTS ********************        
void KNSMonSystems::selectedItemListSlot(QTreeWidgetItem* item, int ) {

    if (item != 0) {

    setMainButtons();
    if (item->text(1) == "localhost") m_localHostBox->setChecked(true);
    else m_localHostBox->setChecked(false);
    m_hostName->setText(item->text(1));
    m_upsName->setText(item->text(0));
    m_powerValue->setValue(item->text(2).toInt());
    m_powerValueLabel->setText(item->text(2));
    m_userName->setText(item->text(3));
    m_password->setText(item->text(4));


    if (item->text(5) == "master") m_masterType->setChecked(true);
    else m_slaveType->setChecked(true);
    }
  }


void KNSMonSystems::editItemSlot(void) {

  m_action = editAction;
  // saves old name of ups and host
  m_oldUpsName = m_upsName->text();
  m_oldHostName = m_hostName->text();

  m_listViewWidget->setEnabled(false);
  setInputWidgetsReadOnly(false);
  m_action = editAction;

  // loads names if users to comboBox
  setUserComboBox();

  if (m_listViewWidget->selectedItems().count() > 0) {
    setModulesComboBox((m_listViewWidget->selectedItems())[0]->text(0));
    }
  else setModulesComboBox("");

  // sets userNameBox before setting other widgets
  if (m_listViewWidget->selectedItems().count() > 0) {
    QString text = m_listViewWidget->selectedItems()[0]->text(3);
    int i = m_userNameBox->findText(text);
    if (i != -1) m_userNameBox->setCurrentIndex(i);
    }

  setInputForm();

  setOkCancelButtons();
  }

void KNSMonSystems::addItemSlot(void) {

  m_action = addAction;
  m_listViewWidget->setEnabled(false);
  setInputWidgetsReadOnly(false);

  setUserComboBox();
  setModulesComboBox("");
  m_hostName->setText("");
  m_upsName->setText("");
  m_powerValue->setValue(1);
  m_powerValueLabel->setText("1");
  m_userName->setText("");
  m_password->setText("");
  m_masterType->setChecked(true);
  m_localHostBox->setChecked(true);
  setInputForm();
  setOkCancelButtons();
  }

void KNSMonSystems::deleteItemSlot(void) {

  m_action = deleteAction;
  m_listViewWidget->setEnabled(false);
  setOkCancelButtons();
  }


void KNSMonSystems::okItemSlot(void) {
  QString typeText;
  monitorUpsStruct newMonitorUpsRecord;
  monitorUpsList::iterator it;


  // when date isn't prepered prodedure is ended
  if (((m_action == addAction) || (m_action == editAction)) && (!isPrepare())) return;

  switch (m_action) {
   //DELETE action 
   case deleteAction:
      if (m_listViewWidget->selectedItems().count() > 0) {
        m_listViewWidget->takeTopLevelItem( m_listViewWidget->indexOfTopLevelItem ((m_listViewWidget->selectedItems())[0]));
        }

      m_commonData->deleteMonitorUpsItem (m_upsName->text(),m_hostName->text());

      if (m_listViewWidget->selectedItems().count() > 0) {
        m_listViewWidget->setItemSelected((m_listViewWidget->selectedItems())[0],false);
        }

      clearAllWidgets();
    break;

    // ADD Action
    case addAction:
    if (m_masterType->isChecked()) typeText = "master"; else typeText = "slave";
    if (m_localHostBox->isChecked()) {
      QTreeWidgetItem* newTreeWidgetItem = new QTreeWidgetItem(m_listViewWidget);
      newTreeWidgetItem->setText(0,m_upsNameBox->currentText());
      newTreeWidgetItem->setText(1,"localhost");
      newTreeWidgetItem->setText(2,QString::number(m_powerValue->value()));
      newTreeWidgetItem->setText(3,m_userNameBox->currentText());
      newTreeWidgetItem->setText(4,m_password->text());
      newTreeWidgetItem->setText(5,typeText);

      newMonitorUpsRecord.upsName=m_upsNameBox->currentText();
      newMonitorUpsRecord.hostName="localhost";   
      newMonitorUpsRecord.userName=m_userNameBox->currentText();
      }
    else {
      QTreeWidgetItem* newTreeWidgetItem = new QTreeWidgetItem(m_listViewWidget);
      newTreeWidgetItem->setText(0,m_upsName->text());
      newTreeWidgetItem->setText(1,m_hostName->text());
      newTreeWidgetItem->setText(2,QString::number(m_powerValue->value()));
      newTreeWidgetItem->setText(3,m_userName->text());
      newTreeWidgetItem->setText(4,m_password->text());
      newTreeWidgetItem->setText(5,typeText);
      
      newMonitorUpsRecord.upsName=m_upsName->text();
      newMonitorUpsRecord.hostName=m_hostName->text();
      newMonitorUpsRecord.userName=m_userName->text();
      }
    newMonitorUpsRecord.powerValue=m_powerValue->value();
    newMonitorUpsRecord.password=m_password->text();
    if (m_masterType) newMonitorUpsRecord.upsmon=MON_MASTER;
    else newMonitorUpsRecord.upsmon=MON_SLAVE;
    // adds record in monitorUpsList
    m_monitorUpsList->push_back(newMonitorUpsRecord);
    break;

    // EDIT Action
    case editAction:
    if (m_listViewWidget->selectedItems().count() > 0) {
      QTreeWidgetItem* selectedItem = (m_listViewWidget->selectedItems())[0];

      if (m_localHostBox->isChecked()) {  // UPS is on localhost


        selectedItem->setText(0,m_upsNameBox->currentText());
        selectedItem->setText(1,"localhost");
        selectedItem->setText(3,m_userNameBox->currentText());
        }
    else {
      selectedItem->setText(0,m_upsName->text());
      selectedItem->setText(1,m_hostName->text());
      selectedItem->setText(3,m_userName->text());
      }
    selectedItem->setText(2,QString::number(m_powerValue->value()));
    selectedItem->setText(4,m_password->text());


    if (m_masterType->isChecked()) selectedItem->setText(5,"master");
    else selectedItem->setText(5,"slave");


    // corrects record in monitorUpsList
    for (it=m_monitorUpsList->begin(); it != m_monitorUpsList->end(); it++) {
      if (((*it).upsName == m_oldUpsName) && ((*it).hostName == m_oldHostName)) {
        if (m_localHostBox->isChecked()) {  // UPS is on localhost
          (*it).upsName=m_upsNameBox->currentText();
          (*it).hostName="localhost";
          (*it).userName=m_userNameBox->currentText();
          }
        else {
          (*it).upsName=m_upsName->text();
          (*it).hostName=m_hostName->text();
          (*it).userName=m_userName->text();
          }
        (*it).powerValue=m_powerValue->value();
        (*it).password=m_password->text();
        if (m_masterType->isChecked()) (*it).upsmon=MON_MASTER;
        else (*it).upsmon=MON_SLAVE;
        break;
        }
      }
      }
    break;
    case noneAction:
    break;
    }

  m_action = noneAction;
  setMainButtons();
  m_listViewWidget->setEnabled(true);
  setInputWidgetsReadOnly(true);
  emit changed();
  }

void KNSMonSystems::cancelItemSlot(void) {

  m_action = noneAction;
  setMainButtons();
  m_listViewWidget->setEnabled(true);
  setInputWidgetsReadOnly(true);
  if (m_listViewWidget->selectedItems().count() > 0) {
    selectedItemListSlot ((m_listViewWidget->selectedItems())[0],0);
    }
  else clearROWidgets();

  }

void KNSMonSystems::localHostSelectedSlot(void) {
  setInputForm();
  }



void KNSMonSystems::userChangedSlot (const QString & name) {

  m_password->setText( m_commonData->findUserPassword(name));

//  if (m_commonData->findMonitorUpsmon(1) == MON_MASTER) {
//    m_masterType->setChecked(true);
//    }
//  else {
//    m_slaveType->setChecked(true);
//    }
  }

void KNSMonSystems::setStandardSlot (void) {
  modulesList* modulesItems;
  modulesList::const_iterator modulesIt;
  usersPermList* userItems;
  usersPermList::const_iterator userIt;
  monitorUpsStruct newMonitorUpsRecord;
  QString addressName;
  QString upsmonName;
  QString upsmonTypeString;
  typeOfMonitoring upsmonType;
  QString password;


  upsmonType = MON_SLAVE; // default value
  if (m_action != noneAction ) cancelItemSlot();
  
  if (m_listViewWidget->selectedItems().count() > 0) {
     m_listViewWidget->setItemSelected((m_listViewWidget->selectedItems())[0],false);
     }
  clearAllWidgets();

  // deletes of items in QListWidget and in commonDate
  while (m_listViewWidget->topLevelItemCount()) {
    m_listViewWidget->takeTopLevelItem(0);
    }

  m_monitorUpsList->clear();

  addressName="localhost";

  upsmonName="";
  userItems=m_commonData->usersDataAddr();


// we find monuser
  for (userIt=userItems->begin(); userIt!=userItems->end(); userIt++) {
    if (((*userIt).name=="monuser") && (!(*userIt).isUser)) {
      upsmonName=(*userIt).name;
      password=(*userIt).password;
      upsmonType=(*userIt).upsmon;
      if ((*userIt).upsmon==MON_MASTER) upsmonTypeString="master"; else upsmonTypeString="slave";
      break;
      }
   }


    if (upsmonName=="") {
      // monuser don't exist, we find some item for monitoring
      for (userIt=userItems->begin(); userIt!=userItems->end(); userIt++) {
        if (!(*userIt).isUser) {
          upsmonName=(*userIt).name;
          password=(*userIt).password;
          upsmonType=(*userIt).upsmon;
          if ((*userIt).upsmon==MON_MASTER) upsmonTypeString="master"; else upsmonTypeString="slave";
          break;
          }
        }
      }
    

    if (upsmonName != "") {
    // adds new items
      modulesItems = m_commonData->modulesDataAddr();
      for (modulesIt = modulesItems->begin(); modulesIt!=modulesItems->end(); modulesIt++) {
        if (((*modulesIt).name != "dummyups") && ((*modulesIt).name != "dummycons")) {
          QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(m_listViewWidget);
          treeWidgetItem->setText(0,(*modulesIt).name);
          treeWidgetItem->setText(1,addressName);
          treeWidgetItem->setText(2,"1");
          treeWidgetItem->setText(3,upsmonName);
          treeWidgetItem->setText(4,password);
          treeWidgetItem->setText(5,upsmonTypeString);

          newMonitorUpsRecord.upsName=(*modulesIt).name;
          newMonitorUpsRecord.hostName=addressName;
          newMonitorUpsRecord.userName=upsmonName;
          newMonitorUpsRecord.powerValue=1;
          newMonitorUpsRecord.password=password;
          newMonitorUpsRecord.upsmon=upsmonType;
          m_monitorUpsList->push_back(newMonitorUpsRecord);
          }
        }
      emit changed();
      }
   
  }

#include "knsmonsystems.moc"
