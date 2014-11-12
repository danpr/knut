/***************************************************************************
                          knsmonvalues.cpp  -  description
                             -------------------
    begin                : ?t ?íj 21 2004
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

#include "knsmonvalues.h"

#include <KDE/KLocale>

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtCore/QString>



KNSMonValues::KNSMonValues(KNSCommonData* commonData, QWidget *parent)
             : QWidget(parent) {

  KPushButton* standardButton;

  m_monitorValues = commonData->monitorValuesDataAddr();

  QLabel* label1 = new QLabel (i18n("Minimum number of power supplies for running system")+" : ",this);
  m_minSupplies = new KIntNumInput(this);  
  label1->setBuddy(m_minSupplies);
  QLabel* label2 = new QLabel (i18n("Shutdown command")+" : ",this);
  m_shutDownCmd = new KLineEdit (this);
  label2->setBuddy(m_shutDownCmd);
  QLabel* label3 = new QLabel (i18n("Notification command")+" : ",this);
  m_notifyCmd = new KLineEdit (this);
  m_notifyCmdUsed = new QCheckBox(this);
  label3->setBuddy(m_notifyCmd);
  QLabel* label4 = new QLabel (i18n("Polling frequency")+" : ",this);
  m_pollFreq = new KIntNumInput(this);
  m_pollFreqUsed = new QCheckBox(this);
  label4->setBuddy(m_pollFreq);
  QLabel* label5 = new QLabel (i18n("Polling alert frequency")+" : ",this);
  m_pollAlertFreq = new KIntNumInput(this);
  m_pollAlertFreqUsed = new QCheckBox(this);
  label5->setBuddy(m_pollAlertFreq);
  QLabel* label6 = new QLabel (i18n("Interval to wait before disconnect slave host")+" :",this);
  m_hotSync = new KIntNumInput(this);
  label6->setBuddy(m_hotSync);
  QLabel* label7 = new QLabel (i18n("Interval to wait before declaring UPS like dead")+" :",this);
  m_deadTime = new KIntNumInput(this);
  label7->setBuddy(m_deadTime);
  QLabel* label8 = new QLabel (i18n("Name of powerdown flag")+" : ",this);
  m_powerDown = new KLineEdit (this);
  label8->setBuddy(m_powerDown);
  QLabel* label9 = new QLabel (i18n("Replace batt. warning time")+" : ",this);
  m_rbWarnTime = new KIntNumInput(this);
  m_rbWarnTimeUsed = new QCheckBox (this) ;
  label9->setBuddy(m_rbWarnTime);
  QLabel* label10 = new QLabel (i18n("No communications warning time")+" : ",this);
  m_noCommWarnTime = new KIntNumInput(this);
  m_noCommWarnTimeUsed = new QCheckBox (this) ;
  label10->setBuddy(m_noCommWarnTime);
  QLabel* label11 = new QLabel (i18n("Sleep interval before shutting down the system")+" : ",this);
  m_finalDelay = new KIntNumInput(this);
  label11->setBuddy(m_finalDelay);

  standardButton = new KPushButton(i18n("Default of monitoring's properties"),this);


  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QGridLayout* itemLayout = new QGridLayout();
  itemLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* buttonsLayout = new QHBoxLayout();
  buttonsLayout->setSizeConstraint(QLayout::SetMinimumSize);


  mainLayout->addLayout(itemLayout);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addStretch(20);

  itemLayout->addWidget(label1,1,1);
  itemLayout->addWidget(m_minSupplies,1,2);
  itemLayout->addWidget(label2,2,1);
  itemLayout->addWidget(m_shutDownCmd,2,2);
  itemLayout->addWidget(label3,3,1);
  itemLayout->addWidget(m_notifyCmd,3,2);
  itemLayout->addWidget(m_notifyCmdUsed,3,3);
  itemLayout->addWidget(label4,4,1);
  itemLayout->addWidget(m_pollFreq,4,2);
  itemLayout->addWidget(m_pollFreqUsed,4,3);
  itemLayout->addWidget(label5,5,1);
  itemLayout->addWidget(m_pollAlertFreq,5,2);
  itemLayout->addWidget(m_pollAlertFreqUsed,5,3);
  itemLayout->addWidget(label6,6,1);
  itemLayout->addWidget(m_hotSync,6,2);
  itemLayout->addWidget(label7,7,1);
  itemLayout->addWidget(m_deadTime,7,2);
  itemLayout->addWidget(label8,8,1);
  itemLayout->addWidget(m_powerDown,8,2);
  itemLayout->addWidget(label9,9,1);
  itemLayout->addWidget(m_rbWarnTime,9,2);
  itemLayout->addWidget(m_rbWarnTimeUsed,9,3);
  itemLayout->addWidget(label10,10,1);
  itemLayout->addWidget(m_noCommWarnTime,10,2);
  itemLayout->addWidget(m_noCommWarnTimeUsed,10,3);
  itemLayout->addWidget(label11,11,1);
  itemLayout->addWidget(m_finalDelay,11,2);

  buttonsLayout->addStretch(20);
  buttonsLayout->addWidget(standardButton);

  initValues();

  connect (m_notifyCmdUsed,SIGNAL (clicked()),this, SLOT(notifyCmdUsedSlot()));
  connect (m_pollFreqUsed,SIGNAL (clicked()),this, SLOT(pollFreqUsedSlot()));
  connect (m_pollAlertFreqUsed,SIGNAL (clicked()),this, SLOT(pollAlertFreqUsedSlot()));
  connect (m_rbWarnTimeUsed,SIGNAL (clicked()),this, SLOT(rbWarnTimeUsedSlot()));
  connect (m_noCommWarnTimeUsed,SIGNAL (clicked()),this, SLOT(noCommWarnTimeUsedSlot()));

  connect( standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));

  slotsConnect();
  }




void KNSMonValues::initValues(void){

  m_notifyCmd->setReadOnly(true);
  m_pollFreq->setEnabled(false);
  m_pollAlertFreq->setEnabled(false);
  m_rbWarnTime->setEnabled(false);
  m_noCommWarnTime->setEnabled(false);

  m_minSupplies->setMinimum(m_minSuppliesMin);
  m_pollFreq->setMinimum(m_pollFreqMin);
  m_pollFreqUsed->setChecked(false);
  m_pollAlertFreq->setMinimum(m_pollAlertFreqMin);
  m_pollAlertFreqUsed->setChecked(false);
  m_hotSync->setMinimum(m_hotSyncMin);
  m_deadTime->setMinimum(m_deadTimeMin);
  m_rbWarnTime->setMinimum(m_rbWarnTimeMin);
  m_rbWarnTimeUsed->setChecked(false);
  m_noCommWarnTime->setMinimum(m_noCommWarnTimeMin);
  m_noCommWarnTimeUsed->setChecked(false);
  m_finalDelay->setMinimum(m_finalDelayMin);

  m_shutDownCmd->setText(shutDownCmdDef);
  m_notifyCmd->setText(notifyCmdDef);
  m_notifyCmdUsed->setChecked(false);
  m_powerDown->setText(notifyCmdDef);

  }



KNSMonValues::~KNSMonValues(){
  }

void KNSMonValues::clear(void){

  initValues();
  }



void KNSMonValues::loadItems(void){


  slotsDisconnect();

  clear();

  if (m_monitorValues->minSuppliesFound) m_minSupplies->setValue(m_monitorValues->minSupplies);
  if (m_monitorValues->shutDownCmdFound) m_shutDownCmd->setText(m_monitorValues->shutDownCmd);
  if (m_monitorValues->notifyCmdFound) {
    m_notifyCmd->setText(m_monitorValues->notifyCmd);
    m_notifyCmd->setReadOnly(false);
    m_notifyCmdUsed->setChecked(true);
    }
  else {
    m_notifyCmd->setReadOnly(true);
    m_notifyCmdUsed->setChecked(false);
    }
  if (m_monitorValues->pollFreqFound) {
    m_pollFreq->setValue(m_monitorValues->pollFreq);
    m_pollFreqUsed->setChecked(true);
    m_pollFreq->setEnabled(true);
    }
  else {
    m_pollFreq->setEnabled(false);
    m_pollFreqUsed->setChecked(false);
    }
  if (m_monitorValues->pollAlertFreqFound) {
    m_pollAlertFreq->setValue(m_monitorValues->pollAlertFreq);
    m_pollAlertFreqUsed->setChecked(true);
    m_pollAlertFreq->setEnabled(true);
    }
  else {
    m_pollAlertFreq->setEnabled(false);
    m_pollAlertFreqUsed->setChecked(false);
    }
  if (m_monitorValues->hotSyncFound) m_hotSync->setValue(m_monitorValues->hotSync);
  else m_hotSync->setEnabled(false);
  if (m_monitorValues->deadTimeFound) m_deadTime->setValue(m_monitorValues->deadTime);
  else m_deadTime->setEnabled(false);
  if (m_monitorValues->powerDownFound) m_powerDown->setText(m_monitorValues->powerDown);
  if (m_monitorValues->rbWarnTimeFound) {
    m_rbWarnTime->setValue(m_monitorValues->rbWarnTime);
    m_rbWarnTimeUsed->setChecked(true);
    m_rbWarnTime->setEnabled(true);
    }
  else {
    m_rbWarnTime->setEnabled(false);
    m_rbWarnTimeUsed->setChecked(false);
    }

  if (m_monitorValues->noCommWarnTimeFound) {
    m_noCommWarnTime->setValue(m_monitorValues->noCommWarnTime);
    m_noCommWarnTimeUsed->setChecked(true);
    m_noCommWarnTime->setEnabled(true);
    }
  else {
    m_noCommWarnTime->setEnabled(false);
    m_noCommWarnTimeUsed->setChecked(false);
    }

  if (m_monitorValues->finalDelayFound) m_finalDelay->setValue(m_monitorValues->finalDelay);

  slotsConnect();
  }


void KNSMonValues::slotsConnect(void) {

  connect (m_minSupplies,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_pollFreq,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_pollAlertFreq,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_hotSync,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_deadTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_rbWarnTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_noCommWarnTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_finalDelay,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));

  connect (m_shutDownCmd,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));
  connect (m_notifyCmd,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));
  connect (m_powerDown,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));

  connect (m_notifyCmdUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_pollFreqUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_pollAlertFreqUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_rbWarnTimeUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  connect (m_noCommWarnTimeUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  }

void KNSMonValues::slotsDisconnect(void) {

  disconnect (m_minSupplies,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_pollFreq,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_pollAlertFreq,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_hotSync,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_deadTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_rbWarnTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_noCommWarnTime,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_finalDelay,SIGNAL(valueChanged(int)),this,SLOT(intValuesChangedSlot(int)));

  disconnect (m_shutDownCmd,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));
  disconnect (m_notifyCmd,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));
  disconnect (m_powerDown,SIGNAL(textChanged (const QString &)),this,SLOT(stringValuesChangedSlot(const QString &)));

  disconnect (m_notifyCmdUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_pollFreqUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_pollAlertFreqUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_rbWarnTimeUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  disconnect (m_noCommWarnTimeUsed,SIGNAL(stateChanged(int)),this,SLOT(intValuesChangedSlot(int)));
  }


//----------- SLOTS ------------------

void KNSMonValues::notifyCmdUsedSlot(void) {

  if (m_notifyCmdUsed->isChecked()) {
    m_notifyCmd->setReadOnly(false);
    m_monitorValues->notifyCmdFound =true;
    }
  else {
    m_notifyCmd->setReadOnly(true);
    m_monitorValues->notifyCmdFound = false;
    }
  }

void KNSMonValues::pollFreqUsedSlot(void) {

  if (m_pollFreqUsed->isChecked()) {
    m_pollFreq->setEnabled(true);
    m_monitorValues->pollFreqFound = true;
    }
  else {
    m_pollFreq->setEnabled(false);
    m_monitorValues->pollFreqFound = false;
    }
  }

void KNSMonValues::pollAlertFreqUsedSlot(void) {

  if (m_pollAlertFreqUsed->isChecked()) {
    m_pollAlertFreq->setEnabled(true);
    m_monitorValues->pollAlertFreqFound=true;
    }
  else{
    m_pollAlertFreq->setEnabled(false);
    m_monitorValues->pollAlertFreqFound=false;
    }
  }

void KNSMonValues::rbWarnTimeUsedSlot(void) {

  if (m_rbWarnTimeUsed->isChecked()) {
    m_rbWarnTime->setEnabled(true);
    m_monitorValues->rbWarnTimeFound=true;
    }
  else {
    m_rbWarnTime->setEnabled(false);
    m_monitorValues->rbWarnTimeFound = false;
    }
  }

void KNSMonValues::noCommWarnTimeUsedSlot(void) {

  if (m_noCommWarnTimeUsed->isChecked()) {
    m_noCommWarnTime->setEnabled(true);
    m_monitorValues->noCommWarnTimeFound=true;
    }
  else {
    m_noCommWarnTime->setEnabled(false);
    m_monitorValues->noCommWarnTimeFound=false;
    }
  }


void KNSMonValues::intValuesChangedSlot(int) {
  // sets values of KIntValues
  m_monitorValues->minSupplies = m_minSupplies->value();
  m_monitorValues->pollFreq = m_pollFreq->value();
  m_monitorValues->pollAlertFreq = m_pollAlertFreq->value();
  m_monitorValues->hotSync = m_hotSync->value();
  m_monitorValues->deadTime = m_deadTime->value();
  m_monitorValues->rbWarnTime = m_rbWarnTime->value();
  m_monitorValues->noCommWarnTime = m_noCommWarnTime->value();
  m_monitorValues->finalDelay = m_finalDelay->value();
  emit changed();
   }

void KNSMonValues::stringValuesChangedSlot(const QString &) {

  m_monitorValues->shutDownCmd = m_shutDownCmd->text();
  m_monitorValues->notifyCmd = m_notifyCmd->text();
  m_monitorValues->powerDown = m_powerDown->text();
  emit changed();
  }

void KNSMonValues::setStandardSlot (void) {

  m_minSupplies->setValue(minSuppliesDef);
  m_monitorValues->minSupplies = m_minSupplies->value();
  m_shutDownCmd->setText(shutDownCmdDef);
  m_monitorValues->shutDownCmd = m_shutDownCmd->text();

  m_notifyCmd->setText(notifyCmdDef);
  m_notifyCmd->setReadOnly(true);
  m_monitorValues->notifyCmd = m_notifyCmd->text();
  m_notifyCmdUsed->setChecked(false);
  m_monitorValues->notifyCmdFound = false;

  m_pollFreq->setValue(pollFreqDef);
  m_pollFreq->setEnabled(false);
  m_monitorValues->pollFreq = m_pollFreq->value();
  m_pollFreqUsed->setChecked(false);
  m_monitorValues->pollFreqFound = false;

  m_pollAlertFreq->setValue(pollAlertFreqDef);
  m_pollAlertFreq->setEnabled(false);
  m_monitorValues->pollAlertFreq = m_pollAlertFreq->value();
  m_pollAlertFreqUsed->setChecked(false);
  m_monitorValues->pollAlertFreqFound=false;

  m_hotSync->setValue(hotSyncDef);
  m_monitorValues->hotSync = m_hotSync->value();
  m_deadTime->setValue(deadTimeDef);
  m_monitorValues->deadTime = m_deadTime->value();
  m_powerDown->setText(powerDownDef);
  m_monitorValues->deadTime = m_deadTime->value();

  m_rbWarnTime->setValue(rbWarnTimeDef);
  m_rbWarnTime->setEnabled(false);
  m_monitorValues->rbWarnTime = m_rbWarnTime->value();
  m_rbWarnTimeUsed->setChecked(false);
  m_monitorValues->rbWarnTimeFound = false;

  m_noCommWarnTime->setValue(noCommWarnTimeDef);
  m_noCommWarnTime->setEnabled(false);
  m_monitorValues->noCommWarnTime = m_noCommWarnTime->value();
  m_noCommWarnTimeUsed->setChecked(false);
  m_monitorValues->noCommWarnTimeFound=false;

  m_finalDelay->setValue(finalDelayDef);
  m_monitorValues->finalDelay = m_finalDelay->value();

  emit changed();
  }

#include "knsmonvalues.moc"
