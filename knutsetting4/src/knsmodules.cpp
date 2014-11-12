/***************************************************************************
                          knsmodules.cpp  -  description
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

#include "knsmodules.h"
#include "knsaddups.h"

#include <KDE/KLocale>
#include <KDE/KIcon> 

#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtGui/QPixmap>
#include <QtGui/QListWidget>

#include <QtDebug>
#include <QDebug>


KNSModules::KNSModules(KNSCommonData* commonData, QWidget *parent )
           : QLabel(parent), m_commonData(commonData) {

  m_modulesData = m_commonData->modulesDataAddr();
  m_monitorOnPix=KIcon("kuser");
  m_monitorOffPix=KIcon("agent");


  m_iconWidget = new QListWidget(this);
  m_iconWidget->setViewMode(QListView::IconMode);
  m_iconWidget->setMovement ( QListView::Static );
  m_iconWidget->setFlow(QListView::TopToBottom);
  QVBoxLayout* buttonLayout = new QVBoxLayout();

  m_addButton = new KPushButton (i18n("Add"),this);
  m_editButton = new KPushButton (i18n("Edit"),this);
  m_deleteButton = new KPushButton (i18n("Delete"),this);
  m_okButton = new KPushButton (i18n("OK"),this);
  m_cancelButton = new KPushButton (i18n("Cancel"),this);

  m_modulesEdit = new KNSModulesEdit(commonData, this);


  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* widgetLayout = new QHBoxLayout();
  widgetLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* iconLayout = new QVBoxLayout();
  iconLayout->setSizeConstraint(QLayout::SetMinimumSize);

  topLayout->addLayout(widgetLayout);
  topLayout->addWidget(m_modulesEdit);

  widgetLayout->addLayout(iconLayout);
  widgetLayout->addLayout(buttonLayout);


  iconLayout->addWidget(m_iconWidget);
  iconLayout->setStretchFactor(m_iconWidget,5);
  iconLayout->addWidget(m_modulesEdit);
  iconLayout->setStretchFactor(m_modulesEdit,5);

  buttonLayout->addWidget(m_addButton);
  buttonLayout->addWidget(m_editButton);
  buttonLayout->addWidget(m_deleteButton);
  buttonLayout->addWidget(m_okButton);
  buttonLayout->addWidget(m_cancelButton);
  buttonLayout->addStretch(20);


  setMainButtons(true);
  m_action = noneAction;


  connect(m_iconWidget,SIGNAL(currentItemChanged (QListWidgetItem *, QListWidgetItem *)),this, SLOT(iconCurrentSlot(QListWidgetItem *, QListWidgetItem * )));

  connect(m_editButton,SIGNAL(released()),this, SLOT(editItemSlot( )));
  connect(m_addButton,SIGNAL(released()),this, SLOT(addItemSlot( )));
  connect(m_deleteButton,SIGNAL(released()),this, SLOT(deleteItemSlot( )));


  connect(m_okButton,SIGNAL(released()),this, SLOT(okItemSlot( )));
  connect(m_cancelButton,SIGNAL(released()),this, SLOT(cancelItemSlot( )));
  }



void KNSModules::loadItems(void) {
    modulesList::const_iterator moduleIt;

  m_iconWidget->clear();  
  m_modulesEdit->clear();

  for (moduleIt=m_modulesData->begin(); moduleIt !=m_modulesData->end();++moduleIt ) {
     QListWidgetItem* QLWItem =  new QListWidgetItem((*moduleIt).name,m_iconWidget);
     QLWItem->setIcon(m_monitorOnPix);
    }
  }


KNSModules::~KNSModules(){
}


void KNSModules::setMainButtons(bool enable) {

  if (enable) {
    m_addButton->show();
    m_editButton->show();
    m_deleteButton->show();
    if (m_iconWidget->currentItem() != 0) {
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
    }
  }

void KNSModules::setOkCancelButtons(bool enable) {

setMainButtons(!(enable));

}

//------------------ SLOTS -----------------------------------------
void KNSModules::iconCurrentSlot( QListWidgetItem * newCurrentItem, QListWidgetItem * ){
modulesList::iterator moduleIt;

  if (newCurrentItem != 0) {
    setMainButtons(true);
    for (moduleIt=m_modulesData->begin();moduleIt !=m_modulesData->end();++moduleIt ) {
      if ((*moduleIt).name == newCurrentItem->text() ) break;
      }
    if (moduleIt == m_modulesData->end()) return;
    // we have got iterator for our structure
    m_modulesEdit->putData(moduleIt);
    }
  }

void KNSModules::addItemSlot(void) {


  KNSAddUPS* addUPS = new KNSAddUPS( m_commonData, this);
  connect (addUPS,SIGNAL(upsAdded (QString)),this,SLOT (addedNewItemSlot(QString)));
  addUPS->exec();
  disconnect (addUPS,SIGNAL(upsAdded (QString)),this,SLOT (addedNewItemSlot(QString)));
  delete addUPS;
  emit changed ();
  }



void KNSModules::editItemSlot(void) {

  if (m_iconWidget->currentItem()) {
    m_action = editAction;
    m_iconWidget->setEnabled(false);
    m_modulesEdit->setReadOnly (false);
    setOkCancelButtons(true);
    }
  }

void KNSModules::deleteItemSlot(void) {

  if (m_iconWidget->currentItem()) {
    m_iconWidget->setEnabled(false);
    m_action = deleteAction;
    setOkCancelButtons(true);
    }
  }

void KNSModules::okItemSlot(void) {

  modulesList::iterator moduleIt;

  switch (m_action) {
    case editAction:
      if (m_iconWidget->currentItem() != 0) {
        for (moduleIt=m_modulesData->begin();moduleIt !=m_modulesData->end();++moduleIt ) {
          if ((*moduleIt).name == m_iconWidget->currentItem()->text() ) break;
          }
        if (moduleIt == m_modulesData->end()) return;
        // we have got iterator for our structure
        m_modulesEdit->getData(moduleIt);
        m_modulesEdit->putData(moduleIt);
        }
      m_iconWidget->setEnabled(true);
      m_modulesEdit->setReadOnly (true);
      setMainButtons(true);  //sets main buttons
      emit changed();
      break;
    case deleteAction:

      if (m_iconWidget->currentItem() !=0) {
      QString upsName = m_iconWidget->currentItem()->text();
      
      m_iconWidget->takeItem(m_iconWidget->row(m_iconWidget->currentItem()));
      m_iconWidget->setEnabled(true);
      for (moduleIt=m_modulesData->begin(); moduleIt != m_modulesData->end();++moduleIt ){
        if ((*moduleIt).name == upsName) {
          (*moduleIt).items.clear();

          m_modulesData->erase(moduleIt);
          break;
          }
        }
      m_modulesEdit->clear();
      setMainButtons(true); //sets main buttons
      emit upsDeleted(upsName);
      emit changed();
      }
    break;
    case noneAction:
    break;
    case addAction:
    break;
    }
  m_action = noneAction;
  }


void KNSModules::cancelItemSlot(void) {

  if (m_iconWidget->currentItem() != 0) {
    modulesList::iterator moduleIt;
    for (moduleIt=m_modulesData->begin();moduleIt !=m_modulesData->end();++moduleIt ) {
      if ((*moduleIt).name == m_iconWidget->currentItem()->text() ) break;
      }
    if (moduleIt == m_modulesData->end()) return;
    // we have got iterator for our structure
    m_modulesEdit->putData(moduleIt);
    }
  m_iconWidget->setEnabled(true);
  m_modulesEdit->setReadOnly (true);

  setMainButtons(true);
  m_action = noneAction;

  }

void KNSModules::addedNewItemSlot(QString name) {

  /// new QIconViewItem(m_iconWidget,name,m_monitorOnPix);
  QListWidgetItem* iconWidgetItem = new QListWidgetItem(name,m_iconWidget);
  iconWidgetItem->setIcon(m_monitorOnPix) ;
  }

#include "knsmodules.moc"
