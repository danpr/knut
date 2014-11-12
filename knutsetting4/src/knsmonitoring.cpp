/***************************************************************************
                          knsmonitoring.cpp  -  description
                             -------------------
    begin                : Út ?íj 5 2004
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

#include "knsmonitoring.h"

#include <klocale.h>

#include <qlayout.h>
#include <qgroupbox.h>
#include <qwidget.h>
#include <qtabwidget.h>

//#include <iostream>


KNSMonitoring::KNSMonitoring(KNSCommonData* commonData, QWidget *parent)
              : QLabel(parent) {


  QTabWidget* monitorTab = new QTabWidget(this);

  m_systemWidget = new KNSMonSystems(commonData, monitorTab);
  monitorTab->addTab(m_systemWidget,i18n("Monitoring of UPS setting"));

  m_itemsWidget = new KNSMonValues(commonData, monitorTab);
  monitorTab->addTab(m_itemsWidget,i18n("Properties setting"));

  m_notifyWidget = new KNSMonNotify(commonData, monitorTab);
  monitorTab->addTab(m_notifyWidget,i18n("Notification setting"));

  KPushButton* standardButton = new KPushButton (i18n("Default of monitoring"),this);

  connect (m_notifyWidget,SIGNAL(changed()),this,SLOT(dataChangedSlot()));
  connect (m_itemsWidget,SIGNAL(changed()),this,SLOT(dataChangedSlot()));
  connect (m_systemWidget,SIGNAL(changed()),this,SLOT(dataChangedSlot()));

  connect (standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));

  QVBoxLayout* topLayout = new QVBoxLayout(this);
  QHBoxLayout* buttonLayout = new QHBoxLayout ();
  topLayout->setMargin(10);
  topLayout->setSpacing(10);
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);
  topLayout->addWidget(monitorTab);
  topLayout->addLayout(buttonLayout);
  topLayout->addStretch ( 20 );

  buttonLayout->addStretch( 10 );
  buttonLayout->addWidget(standardButton);
  }


KNSMonitoring::~KNSMonitoring(){
  }


void KNSMonitoring::loadItems( void ) {

  m_systemWidget->loadItems();
  m_itemsWidget->loadItems();
  m_notifyWidget->clear();
  }

void KNSMonitoring::deleteUpsMonitoring( const QString upsName ) {

  m_systemWidget->deleteUps(upsName);
  }


void KNSMonitoring::deleteUserMonitoring( const QString userName ) {

  m_systemWidget->deleteUser(userName);
  }

void KNSMonitoring::addUserMonitoring( const QString userName ) {

  m_systemWidget->addUser(userName);
  }

void KNSMonitoring::dataChangedSlot (void) {

  emit changed();
  }


void KNSMonitoring::setStandardSlot (void) {

  m_systemWidget->setStandardSlot();
  m_itemsWidget->setStandardSlot();
  m_notifyWidget->setStandardSlot();
  emit changed();
  }

void KNSMonitoring::changePasswordSlot(const QString userName, const QString password) {

  m_systemWidget->changePassword(userName, password);
  }

void KNSMonitoring::changeMonitoringSlot(const QString userName, const typeOfMonitoring typeMon) {

  m_systemWidget->changeTypeMon(userName, typeMon);
  }

void KNSMonitoring::typeUserChangeSlot(const QString userName, const bool isUser) {

  if (isUser)m_systemWidget->deleteUser(userName);
  }

#include "knsmonitoring.moc"
